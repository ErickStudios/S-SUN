/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

    KernelSpeaker.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

    the speaker file to make sounds
    postdata: is broken

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#ifndef KERNEL_SPEAKER_H
#define KERNEL_SPEAKER_H

#include <efi.h>
#include <efilib.h>
#include "../include/motor.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
// constants

#define SPK_IN_ASM                          1
#define beep_register                       0x300
#define prototype

// ----------------------------------------------------------------------------------------------
// variables

prototype EFI_PCI_IO_PROTOCOL* SoundCardPciIo =       NULL;

// ----------------------------------------------------------------------------------------------
// functions

prototype EFI_STATUS
FindSoundCard
(
    EFI_SYSTEM_TABLE*                       SystemTable
);

prototype EFI_STATUS
IsSoundCardReady
(
);
#if !SPK_IN_ASM
prototype EFI_STATUS
PlaySound
(
    UINT32                                  frequency,
    UINT32                                  duration
);
#endif

// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// functions
// ----------------------------------------------------------------------------------------------

#if SPK_IN_ASM
extern VOID SetFrequency(
    u16 frequency
);
extern VOID EnableSpeaker(
);
extern VOID DisableSpeaker(
);
extern VOID SetChannelMode(
    u8 mode
);

VOID
PlaySound
(
    u16 frequency,
    u32 duration
)
{
    //
    // configure speaker
    //

    EnableSpeaker();

    //
    // configure frequency
    //

    SetFrequency(
        frequency
    );

    //
    // waiting
    //
    
    gBS->Stall(duration);

    //
    // i finish
    //

    DisableSpeaker();
}
#endif

/*
FindSoundCard ( #Kernel_Boot_Function )

Summary:
    find a sound card
*/
EFI_STATUS
FindSoundCard
(
    EFI_SYSTEM_TABLE* SystemTable
)
{
    /*
    UINTN HandleCount;
    EFI_HANDLE* HandleBuffer;
    EFI_STATUS Status;

    Status = SystemTable->BootServices->LocateHandleBuffer(
        ByProtocol, &gEfiPciIoProtocolGuid, NULL, &HandleCount, &HandleBuffer);
    if (EFI_ERROR(Status)) return Status;

    for (UINTN i = 0; i < HandleCount; i++) {
        EFI_PCI_IO_PROTOCOL* PciIo;
        Status = SystemTable->BootServices->HandleProtocol(
            HandleBuffer[i], &gEfiPciIoProtocolGuid, (VOID**)&PciIo);
        if (EFI_ERROR(Status)) continue;

        UINT8 ClassCode;
        Status = PciIo->Pci.Read(PciIo, EfiPciIoWidthUint8, 0x0B, 1, &ClassCode);
        if (EFI_ERROR(Status)) continue;

        CHAR16 e[512];

        if (BootStage < 2) {
            SPrint(e, sizeof(e), L"Is ((EFI_PCI_IO_PROTOCOL*) index:i=%d) a Audio Card?", i);
            PrintWithStatus(e, ClassCode == 0x04 ? L"\x2a7" : L"X");
        }

        if (ClassCode == 0x04) {  // 0x04 = Multimedia (Audio)
            SoundCardPciIo = PciIo;
            if (BootStage < 2) {
                PrintWithStatus(L"sound card founded", L"\x2a7");
            }
            UINT32 enableHDA = 1;
            EFI_STATUS Status = SoundCardPciIo->Mem.Write(SoundCardPciIo, EfiPciIoWidthUint32, 0, 0x08, 1, &enableHDA);
            Status = SoundCardPciIo->Mem.Write(SoundCardPciIo, EfiPciIoWidthUint32, 0, 0x08, 1, &enableHDA);
            // AsciiPrint("enabling HDA Intel 0x08: %r\n", Status);

            UINT32 GCTL;
            SoundCardPciIo->Mem.Read(SoundCardPciIo, EfiPciIoWidthUint32, 0, 0x08, 1, &GCTL);

            CHAR16 e[10];

            SPrint(e, sizeof(e), L"%x", GCTL);

            PrintWithStatus(L"Global Control after the activation", e);

            Status = SoundCardPciIo->Mem.Write(SoundCardPciIo, EfiPciIoWidthUint32, 0, 0x20, 1, &enableHDA);
            SoundCardPciIo->Mem.Read(SoundCardPciIo, EfiPciIoWidthUint32, 0, 0x20, 1, &GCTL);

            SPrint(e, sizeof(e), L"%x", GCTL);
            PrintWithStatus(L"register 0x20 after the modification", e);

            Status = SoundCardPciIo->Mem.Write(SoundCardPciIo, EfiPciIoWidthUint32, 0, beep_register, 0, &enableHDA);

            return EFI_SUCCESS;
        }
    }

    if (BootStage < 2) {
        PrintWithStatus(L"sound card founded", L"X");
    }

    return EFI_NOT_FOUND;
    */
}

#if !SPK_IN_ASM
/*
IsSoundCardReady

Summary:
    check if the card is ready
*/
EFI_STATUS
IsSoundCardReady
(
)
{
    /*
    if (!SoundCardPciIo) return EFI_NOT_FOUND;

    UINT64 supportedMemRegions;
    EFI_STATUS Status = SoundCardPciIo->GetBarAttributes(SoundCardPciIo, 0, NULL, &supportedMemRegions);
    if (_DEBUG) {
        CHAR16 e[512];
        SPrint(e, sizeof(e), L"Audio card is ready: %r\n", Status);
        printc(e);
    }

    // UINT32 GCTL;
    // Status = SoundCardPciIo->Mem.Read(SoundCardPciIo, EfiPciIoWidthUint32, 0, 0x08, 1, &GCTL);
    // AsciiPrint("Global Control Register (GCTL): %x\n", GCTL);

    UINT32 GCTL, State;
    SoundCardPciIo->Mem.Read(SoundCardPciIo, EfiPciIoWidthUint32, 0, 0x08, 1, &GCTL);
    AsciiPrint("GCTL después de activación: %x\n", GCTL);

    SoundCardPciIo->Mem.Read(SoundCardPciIo, EfiPciIoWidthUint32, 0, beep_register, 1, &State);
    AsciiPrint("Estado del registro de beep: %x\n", State);

    UINT32 codecStatus;
    SoundCardPciIo->Mem.Read(SoundCardPciIo, EfiPciIoWidthUint32, 0, 0x20, 1, &codecStatus);
    AsciiPrint("Estado del codec de sonido: %x\n", codecStatus);
    return Status;
    */
}

/*
PlaySound

Summary:
    play a beep
*/
EFI_STATUS
PlaySound
(
    UINT32 frequency,
    UINT32 duration
)
{
    /*
    EFI_SYSTEM_TABLE* SystemTable = gST;

    if (!SoundCardPciIo) {
        if (_DEBUG) {
            printc(L"No sound card initialized. Searching...\n");
        }
        EFI_STATUS Status = FindSoundCard(SystemTable);
        if (EFI_ERROR(Status)) return Status;
    }

    if (_DEBUG) {
        CHAR16 e[512];
        SPrint(e, sizeof(e), L"frequency: %x , duration %x\n", frequency, duration);
        printc(e);
    }

    // Verificar si la tarjeta está lista
    EFI_STATUS Status = IsSoundCardReady();
    if (EFI_ERROR(Status)) return Status;

    UINT8 BarIndex = 0;

    // Configurar volumen
    UINT32 volume = 0xFFFF;
    // Status = SoundCardPciIo->Mem.Write(SoundCardPciIo, EfiPciIoWidthUint32, BarIndex, 0x80, 1, &volume);
    if (_DEBUG) {
        CHAR16 e[512];
        SPrint(e, sizeof(e), L"Writing volume: %r\n", Status);
        printc(e);
    }

    UINT32 beepFrequency = frequency;  // Frecuencia del tono (prueba con otro valor)
    Status = SoundCardPciIo->Mem.Write(SoundCardPciIo, EfiPciIoWidthUint32, BarIndex, beep_register, 1, &frequency);
    AsciiPrint("Beep activado: %r\n", Status);

    UINT32 GCTL, State;

    while (State != frequency) {
        Status = SoundCardPciIo->Mem.Write(SoundCardPciIo, EfiPciIoWidthUint32, BarIndex, beep_register, 1, &frequency);

        SoundCardPciIo->Mem.Read(SoundCardPciIo, EfiPciIoWidthUint32, 0, beep_register, 1, &State);
        AsciiPrint("Estado del registro de beep: %d\n", State);
    }

    // Esperar la duración del sonido
    globalsystemtable->BootServices->Stall(duration * 1000);

    // Apagar sonido
    UINT32 muteBeep = 0x0000;
    Status = SoundCardPciIo->Mem.Write(SoundCardPciIo, EfiPciIoWidthUint32, BarIndex, beep_register, 1, &muteBeep);
    AsciiPrint("Beep desactivado: %r\n", Status);

    return EFI_SUCCESS;
    */
}
#endif

// ----------------------------------------------------------------------------------------------
// END functions
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif