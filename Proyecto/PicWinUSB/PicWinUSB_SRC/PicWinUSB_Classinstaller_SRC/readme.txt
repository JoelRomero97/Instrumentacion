ClassInstaller Compiled with:
WDK - Windows Server 2008 RC0
6001.16659.070916-1443


Can be downloaded here:
https://connect.microsoft.com/Downloads/DownloadDetails.aspx?SiteID=148&DownloadID=8748



To compile picusb86.dll, You must copy picusb86 folder supplied into:
C:\WINDDK\6001\src\ and execute this:

1st) C:\WINDOWS\system32\cmd.exe /k C:\WINDDK\6001\bin\setenv.bat C:\WINDDK\6001 fre WXP
2nd) cd src/picusb86
3rd) build



To compile picusb64.dll, You must copy picusb86 folder supplied into:
C:\WINDDK\6001\src\ and execute this:

1st) C:\WINDOWS\system32\cmd.exe /k C:\WINDDK\6001\bin\setenv.bat C:\WINDDK\6001 fre x64
2nd) cd src/picusb64
3rd) build



NOTE: ClassInstaller (picusb86.dll and picusb64.dll) are only needed to put a
Control Panel Class ICON. It is not a Driver, only an ICON in a DLL...