#include "Memory.h"
#include <iostream>
#include <vector>
using namespace std;

template <typename T>
uintptr_t getFinalAddress(const Memory& memory, uintptr_t baseAddress, const vector<uintptr_t>& offsets) {
    uintptr_t finalAddress = baseAddress;
    for (const auto& offset : offsets) {
        finalAddress = memory.Read<uintptr_t>(finalAddress + offset);
    }
    return finalAddress;
}

int main() {
    cout << "--------------------------------------------------" << endl;
    cout << "Buscando y abriendo proceso de nombre Sunkenland.exe" << endl;
    
    const auto memory = Memory{ L"Sunkenland.exe" };
    if (memory.isProcessOpened()) {
        cout << "Proceso encontrado!" << endl;
        while (1) {

            const uintptr_t monoBase = memory.GetModuleAddress(L"mono-2.0-bdwgc.dll");
            const uintptr_t addressToRead = memory.Read<uintptr_t>(monoBase + 0x007160F0);

            vector<uintptr_t> offsets = { 0x70, 0x220, 0x10,0x180,0x80,0x30 };
            const uintptr_t finalAddress = getFinalAddress<uintptr_t>(memory, addressToRead, offsets);

            const int32_t metal = memory.Read<int32_t>(finalAddress + 0xAC);
            cout << metal << endl;

            if (metal != 0) {
                memory.Write(finalAddress + 0xAC, 15);
            }

            Sleep(500);
        }
    }
    else {
        cout << "No se encontró el proceso" << endl;
    }
}
