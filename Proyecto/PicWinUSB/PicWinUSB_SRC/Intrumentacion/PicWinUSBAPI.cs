using System;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;
using Microsoft.Win32.SafeHandles;
using System.Runtime.InteropServices;

namespace Intrumentacion
{
        unsafe public class PicWinUSBAPI
        {
            #region Imported functions from winusb.dll ... some functions has not been checked yet.
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_AbortPipe(IntPtr InterfaceHandle, byte PipeID);
            //[DllImport("winusb.dll")]
            //private static extern bool WinUsb_ControlTransfer(IntPtr InterfaceHandle, WINUSB_SETUP_PACKET SetupPacket, byte* Buffer, ulong BufferLength, out ulong* LengthTransferred); // retocar
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_FlushPipe(IntPtr InterfaceHandle, byte PipeID);
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_Free(IntPtr InterfaceHandle);
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_GetAssociatedInterface(IntPtr InterfaceHandle, byte AssociatedInterfaceIndex, out IntPtr AssociatedInterfaceHandle);
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_GetCurrentAlternateSetting(IntPtr InterfaceHandle, out byte* AlternateSetting);
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_GetDescriptor(IntPtr InterfaceHandle, byte DescriptorType, byte Index, ushort LanguageID, out byte* Buffer, ulong BufferLength, out ulong* LengthTransferred);
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_GetInterfacePowerPolicy(IntPtr InterfaceHandle, ulong PolicyType, out ulong* ValueLength, out void* Value); // raro
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_GetPipePolicy(IntPtr DeviceHandle, byte PipeID, ulong PolicyType, out ulong* ValueLength, out void* Value); // raroo
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_Initialize(IntPtr DeviceHandle, out IntPtr InterfaceHandle); // out inter
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_QueryDeviceInformation(IntPtr DeviceHandle, ulong InformationType, out ulong BufferLength, out ulong Buffer); // raro
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_QueryInterfaceSettings(IntPtr InterfaceHandle, byte AlternateSettingNumber, IntPtr UsbAltInterfaceDescriptor); // falta lo ultimo
                                                                                                                                                             //[DllImport("winusb.dll")]
                                                                                                                                                             //private static extern bool WinUsb_QueryPipe(IntPtr InterfaceHandle, byte AlternateInterfaceNumber, byte PipeIndex, out WINUSB_PIPE_INFORMATION PipeInformation);
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_ReadPipe(IntPtr InterfaceHandle, byte PipeID, byte[] Buffer, int BufferLength, out uint LengthTransferred, IntPtr Overlapped);
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_ResetPipe(IntPtr InterfaceHandle, byte PipeID);
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_SetCurrentAlternateSetting(IntPtr InterfaceHandle, byte AlternateSetting);
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_SetInterfacePowerPolicy(IntPtr InterfaceHandle, ulong PolicyType, ulong ValueLength, void* Value); // creo q mal
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_SetPipePolicy(IntPtr InterfaceHandle, byte PipeID, ulong PolicyType, ulong ValueLength, void* Value);
            [DllImport("winusb.dll")]
            private static extern bool WinUsb_WritePipe(IntPtr InterfaceHandle, byte PipeID, byte[] Buffer, int BufferLength, out uint LengthTransferred, IntPtr Overlapped);
            #endregion
            static void Main()
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new Instru() );
            }

            //***********************************************************************
            //Metodo para iniciar la comunicación con el driver del dispositivo USB
            //***********************************************************************
            public IntPtr Init_PicWinUSB(Guid InterfaceGuid)
            {
                bool bResult = false;
                UInt32 requiredLength = 0;
                UInt32 Instance = 0;  // Instance > 0 if various PicWinUSB are attached
                IntPtr iHandle;
                IntPtr hDev;

                // [1] Get a handle to the device information set by passing the device interface GUID that you defined in the INF to SetupDiGetClassDevs.
                IntPtr deviceInfo = DeviceInterfaceImports.SetupDiGetClassDevs(ref InterfaceGuid, null, IntPtr.Zero, (DeviceInterfaceImports.DIGCF_PRESENT | DeviceInterfaceImports.DIGCF_DEVICEINTERFACE));

                // [2] Call SetupDiEnumDeviceInterfaces to enumerate the system’s device interfaces and obtain information on your device interface
                DeviceInterfaceImports.SP_DEVICE_INTERFACE_DATA interfaceData = new DeviceInterfaceImports.SP_DEVICE_INTERFACE_DATA();
                interfaceData.cbSize = Marshal.SizeOf(interfaceData);

                bResult = DeviceInterfaceImports.SetupDiEnumDeviceInterfaces(deviceInfo, IntPtr.Zero, ref InterfaceGuid, Instance, ref interfaceData);

                // [3] Call SetupDiGetDeviceInterfaceDetail to get detailed data for the device interface.
                DeviceInterfaceImports.SetupDiGetDeviceInterfaceDetail(deviceInfo, ref interfaceData, IntPtr.Zero, 0, out requiredLength, IntPtr.Zero);

                DeviceInterfaceImports.SP_DEVICE_INTERFACE_DETAIL_DATA detailData = new DeviceInterfaceImports.SP_DEVICE_INTERFACE_DETAIL_DATA();

                if (IntPtr.Size == 8) detailData.cbSize = 8;// x64 OS
                else detailData.cbSize = 5; // x86 OS

                bResult = DeviceInterfaceImports.SetupDiGetDeviceInterfaceDetail(deviceInfo, ref interfaceData, ref detailData, requiredLength, out requiredLength, IntPtr.Zero);

                // [4] Destroy non used anymore info
                DeviceInterfaceImports.SetupDiDestroyDeviceInfoList(deviceInfo);

                // [5] Pass the device path to CreateFile to obtain a file handle for the device.
                hDev = DeviceInterfaceImports.CreateFile(detailData.DevicePath, FileAccess.ReadWrite, FileShare.ReadWrite, IntPtr.Zero, FileMode.Open, (DeviceInterfaceImports.FILE_ATTRIBUTE_NORMAL | DeviceInterfaceImports.FILE_FLAG_OVERLAPPED), IntPtr.Zero);
                Debug.WriteLine(detailData.DevicePath);

                // [6] Call WinUsb_Initialize to obtain the WinUSB handle for the device.
                bResult = WinUsb_Initialize(hDev, out iHandle);
                //if(bResult)Debug.WriteLine("PicWinUSB found!!!!!");

                return iHandle;
            }

            //*****************************************************************
            //Metodo de escritura al PIC
            //*****************************************************************
            public bool Write_PicWinUSB(IntPtr iHandle, byte[] dBuffer)
            {
                byte PipeID = 0x01;     // send pipe
                int szBuffer = dBuffer.GetLength(0);
                uint bytesWritten;
                bool bResult;

                bResult = WinUsb_WritePipe(iHandle, PipeID, dBuffer, szBuffer, out bytesWritten, IntPtr.Zero);
                //Debug.WriteLine(bytesWritten);

                return bResult;
            }

            //*****************************************************************
            //Metodo de lectura del PIC
            //*****************************************************************
            public bool Read_PicWinUSB(IntPtr iHandle, byte[] dBuffer)
            {
                byte PipeID = 0x81;     // recieve pipe
                int szBuffer = dBuffer.GetLength(0);
                uint bytesRead;
                bool bResult;

                bResult = WinUsb_ReadPipe(iHandle, PipeID, dBuffer, szBuffer, out bytesRead, IntPtr.Zero);
                //Debug.WriteLine(bytesRead);

                return bResult;
            }
        }
    }
