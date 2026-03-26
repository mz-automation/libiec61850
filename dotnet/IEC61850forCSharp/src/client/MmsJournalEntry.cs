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
        /// Represents an entry of a log
        /// </summary>
        public class MmsJournalEntry
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void MmsJournalEntry_destroy(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsJournalEntry_getEntryID(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsJournalEntry_getOccurenceTime(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr MmsJournalEntry_getJournalVariables(IntPtr self);

            /****************
        	* LinkedList
         	***************/
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LinkedList_getNext(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr LinkedList_getData(IntPtr self);

            private IntPtr self;
            private List<MmsJournalVariable> variables = null;

            internal MmsJournalEntry(IntPtr self)
            {
                this.self = self;
            }

            /// <summary>
            /// Gets the journal variables.
            /// </summary>
            /// <returns>The journal variables.</returns>
            public List<MmsJournalVariable> GetJournalVariables()
            {
                if (variables == null)
                {

                    IntPtr linkedList = MmsJournalEntry_getJournalVariables(self);

                    IntPtr element = LinkedList_getNext(linkedList);

                    variables = new List<MmsJournalVariable>();

                    while (element != IntPtr.Zero)
                    {
                        MmsJournalVariable journalVariable = new MmsJournalVariable(LinkedList_getData(element));

                        variables.Add(journalVariable);

                        element = LinkedList_getNext(element);
                    }
                }

                return variables;
            }

            /// <summary>
            /// Gets the entry identifier of the log entry
            /// </summary>
            /// <returns>The entry identifier.</returns>
            public byte[] GetEntryID()
            {
                IntPtr mmsValuePtr = MmsJournalEntry_getEntryID(self);

                MmsValue mmsValue = new MmsValue(mmsValuePtr);

                byte[] octetString = mmsValue.getOctetString();

                return octetString;
            }

            /// <summary>
            /// Gets the occurence time of the log entry
            /// </summary>
            /// <returns>The occurence time.</returns>
            public ulong GetOccurenceTime()
            {
                IntPtr mmsValuePtr = MmsJournalEntry_getOccurenceTime(self);

                MmsValue mmsValue = new MmsValue(mmsValuePtr);

                return mmsValue.GetBinaryTimeAsUtcMs();
            }

            /// <summary>
            /// Releases all resource used by the <see cref="T:IEC61850.Client.MmsJournalEntry"/> object.
            /// </summary>
            public void Dispose()
            {
                if (self != IntPtr.Zero)
                {
                    MmsJournalEntry_destroy(self);
                    self = IntPtr.Zero;
                }
            }

            ~MmsJournalEntry()
            {
                Dispose();
            }
        }
    }
}