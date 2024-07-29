
#include <cwchar>

void writeColor(HANDLE& hMBAAHandle, DWORD& dwBaseAddress, DWORD dwAnimPtr, const std::array<uint8_t, 3> arrColor) {
    WriteProcessMemory(hMBAAHandle, (LPVOID)(dwAnimPtr + 0x18), arrColor.data(), 3, 0);
}

void updateAnimation(HANDLE hMBAAHandle, DWORD dwBaseAddress, DWORD dwAnimPtr, uint8_t bBlocking, DWORD dwPattern) {
    
    // passing this many params to this func is overkill

    if (bBlocking == 1)
    {
        writeColor(hMBAAHandle, dwBaseAddress, dwAnimPtr, { 255, 0, 0 });
    }
    else
    {
        switch (dwPattern)
        {
        case 0:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 17:
        case 18:
        case 19:
        case 35:
        case 36:
        case 37:
        case 38:
        case 39:
        case 40:
        case 360:
            writeColor(hMBAAHandle, dwBaseAddress, dwAnimPtr, { 0, 255, 0 });
            break;
        default:
            break;
        }
    }

}


void HighlightThread(HANDLE& hMBAAHandle, DWORD& dwBaseAddress, std::atomic_bool& bStopHighlightThread)
{
	wchar_t buffer[256];

	uint8_t bP1Blocking;
    uint8_t bP2Blocking;
    DWORD dwP1Pattern;
    DWORD dwP2Pattern;
    
    DWORD dwP1AnimPtr;
    DWORD dwP2AnimPtr;

    DWORD dwP1AnimPtrPrev = -1;
    DWORD dwP2AnimPtrPrev = -1;

	while (!bStopHighlightThread)
	{
		if (hMBAAHandle == 0) {
			Sleep(100);
			continue;
		}

        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Struct + 0x320), &dwP1AnimPtr, 4, 0);
        if (dwP1AnimPtr != dwP1AnimPtrPrev) {
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Blocking), &bP1Blocking, 1, 0);
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternRead - 0xAFC), &dwP1Pattern, 4, 0);
            updateAnimation(hMBAAHandle, dwBaseAddress, dwP1AnimPtr, bP1Blocking, dwP1Pattern);
            dwP1AnimPtrPrev = dwP1AnimPtr;
        }


        ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Struct + 0xAFC + 0x320), &dwP2AnimPtr, 4, 0);
        if (dwP2AnimPtr != dwP2AnimPtrPrev) {
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1Blocking + 0xAFC), &bP2Blocking, 1, 0);
            ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2PatternRead), &dwP2Pattern, 4, 0);
            updateAnimation(hMBAAHandle, dwBaseAddress, dwP2AnimPtr, bP2Blocking, dwP2Pattern);
            dwP2AnimPtrPrev = dwP2AnimPtr;
        }

        

        

		//swprintf(buffer, 256, L"test: %08X %08X\n", nP1Blocking, nP2Blocking);
        //Sleep(0.5);
		//OutputDebugString(buffer);
	}
}