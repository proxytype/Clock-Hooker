# Clock-Hooker

## Payload
Hook GetLocalTime(...) in kernelbase.dll for manipulate system time steps.

## Payload 2
Hook GetTimeFormatEx(...) in kernelbase.dll to overwrite clock string in process (ex: Explorer.exe)

## Requierments:
Microsoft Detours Library - https://github.com/microsoft/Detours

**Compile:**
1. Unzip source code, open command line and enter to source directory
2. SET DETOURS_TARGET_PROCESSOR=X64
3. C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat
4. NMAKE

Add detours.lib to Linker additional libraries.

**Hooked Function:**
- GetLocalTime <br>
- GetTimeFormatEx(...) <br>
