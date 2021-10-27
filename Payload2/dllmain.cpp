// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include "detours.h"

typedef int(WINAPI* realGetTimeFormatEx) (
      LPCWSTR          lpLocaleName,
      DWORD            dwFlags,
      const SYSTEMTIME* lpTime,
      LPCWSTR          lpFormat,
      LPWSTR           lpTimeStr,
      int              cchTime
);

realGetTimeFormatEx  originalrealGetTimeFormatEx = (realGetTimeFormatEx)GetProcAddress(GetModuleHandleA("kernelbase.dll"), "GetTimeFormatEx");

int WINAPI _GetTimeFormatEx(
    LPCWSTR          lpLocaleName,
    DWORD            dwFlags,
    const SYSTEMTIME* lpTime,
    LPCWSTR          lpFormat,
    LPWSTR           lpTimeStr,
    int              cchTime
) {

    const int LENGTH = 10;

    WCHAR timeString[MAX_PATH] = L"GO HOME!\0";


    originalrealGetTimeFormatEx(lpLocaleName, dwFlags, lpTime, lpFormat, lpTimeStr, cchTime);
    
    memcpy(lpTimeStr, timeString, LENGTH * sizeof(wchar_t));

    return 10;

}

void attachDetour() {

    DetourRestoreAfterWith();
    DetourTransactionBegin();

    DetourUpdateThread(GetCurrentThread());

    DetourAttach((PVOID*)&originalrealGetTimeFormatEx, _GetTimeFormatEx);

    DetourTransactionCommit();
}

void deAttachDetour() {

    DetourTransactionBegin();

    DetourUpdateThread(GetCurrentThread());

    DetourDetach((PVOID*)&originalrealGetTimeFormatEx, _GetTimeFormatEx);

    DetourTransactionCommit();
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        attachDetour();
        break;
    case DLL_PROCESS_DETACH:
        deAttachDetour();
        break;
    }
    return TRUE;
}


