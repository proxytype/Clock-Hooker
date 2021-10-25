// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include "detours.h"

typedef void(WINAPI* realGetLocalTime) (
    LPSYSTEMTIME lpSystemTime
);

realGetLocalTime  originalRealGetLocalTime = (realGetLocalTime)GetProcAddress(GetModuleHandleA("kernelbase.dll"), "GetLocalTime");

void WINAPI _GetLocalTime(
    LPSYSTEMTIME lpSystemTime
) {

    originalRealGetLocalTime(lpSystemTime);

    //https://en.wikipedia.org/wiki/Charlie_Chaplin
    lpSystemTime->wYear = 1889;
    lpSystemTime->wDay = 16;
    lpSystemTime->wMonth = 4;

}

void attachDetour() {

    DetourRestoreAfterWith();
    DetourTransactionBegin();

    DetourUpdateThread(GetCurrentThread());

    DetourAttach((PVOID*)&originalRealGetLocalTime, _GetLocalTime);

    DetourTransactionCommit();
}

void deAttachDetour() {

    DetourTransactionBegin();

    DetourUpdateThread(GetCurrentThread());

    DetourDetach((PVOID*)&originalRealGetLocalTime, _GetLocalTime);

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


