using IEC61850.Common;
using IEC61850.Model;
using IEC61850.TLS;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

// IEC 61850 API for the libiec61850 .NET wrapper library
namespace IEC61850
{
    // IEC 61850 server API.
    namespace Server
    {
        public class DataAttribute : ModelNode
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr DataAttribute_create(string name, IntPtr parent, int type, int fc,
                byte triggerOptions, int arrayElements, UInt32 sAddr);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern void DataAttribute_setValue(IntPtr self, IntPtr mmsValue);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int DataAttribute_getType(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern byte DataAttribute_getTrgOps(IntPtr self);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern int DataAttribute_getFC(IntPtr self);

            internal DataAttribute(IntPtr self, ModelNode parent) : base(self)
            {
                this.parent = parent;
            }

            /// <summary>
            /// Create a new data attribute and add it to a parent model node
            /// </summary>
            /// The parent model node has to be of type DataObject or DataAttribute
            /// <param name="name">the name of the data attribute (e.g. "stVal")</param>
            /// <param name="parent">the parent model node (of type DataObject or DataAttribute)</param>
            /// <param name="type">the type of the data attribute (CONSTRUCTED if the type contains sub data attributes)</param>
            /// <param name="fc">the functional constraint (FC) of the data attribute</param>
            /// <param name="trgOps">the trigger options (dupd, dchg, qchg) that cause an event notification</param>
            /// <param name="arrayElements">the number of array elements if the data attribute is an array or 0</param>
            /// <param name="sAddr">an optional short address (deprecated)</param>
            public DataAttribute(string name, ModelNode parent, DataAttributeType type, FunctionalConstraint fc, TriggerOptions trgOps,
                int arrayElements, UInt32 sAddr)
            {
                this.parent = parent;

                self = DataAttribute_create(name, parent.self, (int)type, (int)fc, (byte)trgOps, arrayElements, sAddr);
            }

            /// <summary>
            /// Create a new data attribute and add it to a parent model node
            /// </summary>
            /// The parent model node has to be of type DataObject or DataAttribute
            /// <param name="name">the name of the data attribute (e.g. "stVal")</param>
            /// <param name="parent">the parent model node (of type DataObject or DataAttribute)</param>
            /// <param name="type">the type of the data attribute (CONSTRUCTED if the type contains sub data attributes)</param>
            /// <param name="fc">the functional constraint (FC) of the data attribute</param>
            /// <param name="trgOps">the trigger options (dupd, dchg, qchg) that cause an event notification</param>
            /// <param name="arrayElements">the number of array elements if the data attribute is an array or 0</param>
            public DataAttribute(string name, ModelNode parent, DataAttributeType type, FunctionalConstraint fc, TriggerOptions trgOps,
                int arrayElements)
            {
                this.parent = parent;

                self = DataAttribute_create(name, parent.self, (int)type, (int)fc, (byte)trgOps, arrayElements, 0);
            }

            /// <summary>
            /// Get IEC 61850 data attribute type of the data attribute
            /// </summary>
            public DataAttributeType Type
            {
                get
                {
                    return (DataAttributeType)DataAttribute_getType(self);
                }
            }

            /// <summary>
            /// The trigger options (dchg, qchg, dupd) of the data attribute
            /// </summary>
            public TriggerOptions TrgOps
            {
                get
                {
                    return (TriggerOptions)DataAttribute_getTrgOps(self);
                }
            }

            /// <summary>
            /// The functional constraint (FC) of the data attribute
            /// </summary>
            public FunctionalConstraint FC
            {
                get
                {
                    return (FunctionalConstraint)DataAttribute_getFC(self);
                }
            }

            /// <summary>
            /// Set the value of the data attribute (can be used to set default values before server is created)
            /// </summary>
            /// <param name="value">New value for the data attribute</param>
            public void SetValue(MmsValue value)
            {
                DataAttribute_setValue(self, value.valueReference);
            }
        }
    }
}