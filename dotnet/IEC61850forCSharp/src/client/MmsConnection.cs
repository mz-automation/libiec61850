using IEC61850.Common;
using IEC61850.TLS;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    // IEC 61850 client API.
    namespace Client
    {
        /// <summary>
        /// Represents an MmsConnection object (a single connection to an MMS server)
        /// </summary>
        public class MmsConnection
        {

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern IntPtr MmsConnection_create();

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void MmsConnection_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern IntPtr MmsConnection_identify(IntPtr self, out int mmsError);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void MmsServerIdentity_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void MmsConnection_setLocalDetail(IntPtr self, Int32 localDetail);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern Int32 MmsConnection_getLocalDetail(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern Int32 MmsConnection_setRequestTimeout(IntPtr self, UInt32 timeoutInMs);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern IntPtr MmsConnection_readMultipleVariables(IntPtr self, out int mmsError,
                                                                             string domainId, IntPtr items);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void MmsValue_delete(IntPtr self);

            [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
            private delegate void LinkedListValueDeleteFunction(IntPtr pointer);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void LinkedList_destroyDeep(IntPtr list, LinkedListValueDeleteFunction valueDeleteFunction);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern IntPtr LinkedList_create();

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            private static extern void LinkedList_add(IntPtr self, IntPtr data);


            private IntPtr self = IntPtr.Zero;
            private bool selfDestroy = false;

            public MmsConnection()
            {
                selfDestroy = true;

                self = MmsConnection_create();
            }

            internal MmsConnection(IntPtr mmsConnection)
            {
                self = mmsConnection;
            }

            ~MmsConnection()
            {
                if (selfDestroy)
                    if (self != IntPtr.Zero)
                        MmsConnection_destroy(self);
            }

            private void FreeHGlobaleDeleteFunction(IntPtr pointer)
            {
                Marshal.FreeHGlobal(pointer);
            }

            /// <summary>
            /// Requests the server identity information
            /// </summary>
            /// <returns>The server identity.</returns>
            public MmsServerIdentity GetServerIdentity()
            {
                int mmsError;

                if (self == IntPtr.Zero)
                {
                    throw new IedConnectionException("Pointer is Zero!");
                }

                IntPtr identity = MmsConnection_identify(self, out mmsError);

                if (mmsError != 0)
                    throw new IedConnectionException("Failed to read server identity", mmsError);

                if (identity == IntPtr.Zero)
                    throw new IedConnectionException("Failed to read server identity");

                MmsServerIdentity serverIdentity = (MmsServerIdentity)
                    Marshal.PtrToStructure(identity, typeof(MmsServerIdentity));

                MmsServerIdentity_destroy(identity);

                return serverIdentity;
            }

            /// <summary>
            /// Sets the local detail (maximum MMS PDU size)
            /// </summary>
            /// <param name="localDetail">maximum accepted MMS PDU size in bytes</param>
            public void SetLocalDetail(int localDetail)
            {
                MmsConnection_setLocalDetail(self, localDetail);
            }

            /// <summary>
            /// Gets the local detail (maximum MMS PDU size)
            /// </summary>
            /// <returns>maximum accepted MMS PDU size in bytes</returns>
            public int GetLocalDetail()
            {
                return MmsConnection_getLocalDetail(self);
            }

            /// <summary>
            /// Sets the request timeout
            /// </summary>
            /// <param name="timeoutMs">request timeout in milliseconds</param>
            public void SetRequestTimeout(uint timeoutMs)
            {
                MmsConnection_setRequestTimeout(self, timeoutMs);
            }

            /// <summary>
            /// Reads multipe MMS variables from the same domain
            /// </summary>
            /// <returns>MmsValue of type MMS_ARRAY containing the access results for the requested variables.</returns>
            /// <param name="domainName">the domain name (logical device)</param>
            /// <param name="variables">list of variable names (in MMS notation e.g. "GGIO$ST$Ind1")</param>
            public MmsValue ReadMultipleVariables(string domainName, List<string> variables)
            {
                IntPtr linkedList = LinkedList_create();

                foreach (string variableName in variables)
                {
                    IntPtr handle = System.Runtime.InteropServices.Marshal.StringToHGlobalAnsi(variableName);

                    LinkedList_add(linkedList, handle);
                }

                int error;

                IntPtr mmsValue = MmsConnection_readMultipleVariables(self, out error, domainName, linkedList);

                LinkedList_destroyDeep(linkedList, new LinkedListValueDeleteFunction(FreeHGlobaleDeleteFunction));

                if (error != 0)
                {
                    if (mmsValue != IntPtr.Zero)
                        MmsValue_delete(mmsValue);

                    throw new IedConnectionException("ReadMultipleVariables failed", error);
                }

                return new MmsValue(mmsValue, true);
            }
        }
    }
}