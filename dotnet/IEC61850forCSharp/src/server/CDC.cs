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

        /// <summary>
        /// The CDC class contains helper functions to create DataObject instances for the
        /// most common Common Data Classes.
        /// </summary>
        public class CDC
        {
            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SPS_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_DPS_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_VSS_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SEC_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_CMV_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SAV_create(string name, IntPtr parent, uint options, [MarshalAs(UnmanagedType.I1)] bool isIntegerNotFloat);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ACD_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ACT_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SPG_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_VSG_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ENG_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ING_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ASG_create(string name, IntPtr parent, uint options, [MarshalAs(UnmanagedType.I1)] bool isIntegerNotFloat);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_WYE_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_DEL_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_HST_create(string name, IntPtr parent, uint options, ushort maxPts);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_INS_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_MV_create(string name, IntPtr parent, uint options, [MarshalAs(UnmanagedType.I1)] bool isIntegerNotFloat);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_INC_create(string name, IntPtr parent, uint options, uint controlOptions);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_LPL_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_DPL_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ENS_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SPC_create(string name, IntPtr parent, uint options, uint controlOptions);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_DPC_create(string name, IntPtr parent, uint options, uint controlOptions);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_BSC_create(string name, IntPtr parent, uint options, uint controlOptions, [MarshalAs(UnmanagedType.I1)] bool hasTransientIndicator);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_APC_create(string name, IntPtr parent, uint options, uint controlOptions, [MarshalAs(UnmanagedType.I1)] bool isIntegerNotFloat);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_BCR_create(string name, IntPtr parent, uint options);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ENC_create(string name, IntPtr parent, uint options, uint controlOptions);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_SPV_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasChaManRs);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_STV_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasOldStatus);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_CMD_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasOldStatus, [MarshalAs(UnmanagedType.I1)] bool hasCmTm, [MarshalAs(UnmanagedType.I1)] bool hasCmCt);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_ALM_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasOldStatus);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_CTE_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasHisRs);

            [DllImport("iec61850", CallingConvention = CallingConvention.Cdecl)]
            static extern IntPtr CDC_TMS_create(string name, IntPtr parent, uint options, uint controlOptions, uint wpOptions, [MarshalAs(UnmanagedType.I1)] bool hasHisRs);

            public const int CDC_OPTION_DESC = (1 << 2);
            public const int CDC_OPTION_DESC_UNICODE = (1 << 3);
            public const int CDC_OPTION_AC_DLNDA = (1 << 4);
            public const int CDC_OPTION_AC_DLN = (1 << 5);

            // options that are only valid for DPL CDC
            public const int CDC_OPTION_DPL_HWREV = (1 << 17);
            public const int CDC_OPTION_DPL_SWREV = (1 << 18);
            public const int CDC_OPTION_DPL_SERNUM = (1 << 19);
            public const int CDC_OPTION_DPL_MODEL = (1 << 20);
            public const int CDC_OPTION_DPL_LOCATION = (1 << 21);

            // mandatory data attributes for LLN0 (e.g. LBL configRef)
            public const int CDC_OPTION_AC_LN0_M = (1 << 24);
            public const int CDC_OPTION_AC_LN0_EX = (1 << 25);
            public const int CDC_OPTION_AC_DLD_M = (1 << 26);


            public static DataObject Create_CDC_SPS(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_SPS_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_DPS(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_DPS_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_VSS(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_VSS_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_SEC(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_SEC_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_CMV(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_CMV_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_SAV(ModelNode parent, string name, uint options, bool isIntegerNotFloat)
            {
                IntPtr self = CDC_SAV_create(name, parent.self, options, isIntegerNotFloat);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ACD(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_ACD_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ACT(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_ACT_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_SPG(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_SPG_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_VSG(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_VSG_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ENG(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_ENG_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ING(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_ING_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ASG(ModelNode parent, string name, uint options, bool isIntegerNotFloat)
            {
                IntPtr self = CDC_ASG_create(name, parent.self, options, isIntegerNotFloat);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_WYE(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_WYE_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_DEL(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_DEL_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_HST(ModelNode parent, string name, uint options, ushort maxPts)
            {
                IntPtr self = CDC_HST_create(name, parent.self, options, maxPts);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_INS(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_INS_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_MV(ModelNode parent, string name, uint options, bool isIntegerNotFloat)
            {
                IntPtr self = CDC_MV_create(name, parent.self, options, isIntegerNotFloat);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_INC(ModelNode parent, string name, uint options, uint controlOptions)
            {
                IntPtr self = CDC_INC_create(name, parent.self, options, controlOptions);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_LPL(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_LPL_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_DPL(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_DPL_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ENS(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_ENS_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_SPC(ModelNode parent, string name, uint options, uint controlOptions)
            {
                IntPtr self = CDC_SPC_create(name, parent.self, options, controlOptions);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_DPC(ModelNode parent, string name, uint options, uint controlOptions)
            {
                IntPtr self = CDC_DPC_create(name, parent.self, options, controlOptions);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_BSC(ModelNode parent, string name, uint options, uint controlOptions, bool hasTransientIndicator)
            {
                IntPtr self = CDC_BSC_create(name, parent.self, options, controlOptions, hasTransientIndicator);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_APC(ModelNode parent, string name, uint options, uint controlOptions, bool isIntegerNotFloat)
            {
                IntPtr self = CDC_APC_create(name, parent.self, options, controlOptions, isIntegerNotFloat);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_BCR(ModelNode parent, string name, uint options)
            {
                IntPtr self = CDC_BCR_create(name, parent.self, options);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ENC(ModelNode parent, string name, uint options, uint controlOptions)
            {
                IntPtr self = CDC_ENC_create(name, parent.self, options, controlOptions);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_SPV(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasChaManRs)
            {
                IntPtr self = CDC_SPV_create(name, parent.self, options, controlOptions, wpOptions, hasChaManRs);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_STV(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasOldStatus)
            {
                IntPtr self = CDC_STV_create(name, parent.self, options, controlOptions, wpOptions, hasOldStatus);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_CMD(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasOldStatus, bool hasCmTm, bool hasCmCt)
            {
                IntPtr self = CDC_CMD_create(name, parent.self, options, controlOptions, wpOptions, hasOldStatus, hasCmTm, hasCmCt);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_ALM(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasOldStatus)
            {
                IntPtr self = CDC_ALM_create(name, parent.self, options, controlOptions, wpOptions, hasOldStatus);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_CTE(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasHisRs)
            {
                IntPtr self = CDC_CTE_create(name, parent.self, options, controlOptions, wpOptions, hasHisRs);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }

            public static DataObject Create_CDC_TMS(ModelNode parent, string name, uint options, uint controlOptions, uint wpOptions, bool hasHisRs)
            {
                IntPtr self = CDC_TMS_create(name, parent.self, options, controlOptions, wpOptions, hasHisRs);

                if (self != IntPtr.Zero)
                    return new DataObject(self, parent);
                else
                    return null;
            }
        }
    }
}