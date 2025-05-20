/*
users please help me to devlop this or implement it in your S-SUN distribution
*/
#ifndef KERNEL_NET_H
#define KERNEL_NET_H

#include "../sources/KernelDef.h"

EFI_SIMPLE_NETWORK_PROTOCOL* Net = NULL;

#ifndef NET_PRINCIPIES

/*
SERVER
	-ROOMS
		-ROOM_DATA
		-ROOM_CHAT
		-ROOM_SITES
*/

#define EFI_ROOM_MAX_USERS 10

typedef struct {
	CHAR8 RoomName[32];       // Nombre de la sala
	UINT8 IPv4Address[4];     // Dirección IP en formato IPv4
	VOID* RoomData;           // Datos asociados
	BOOLEAN IsActive;         // Indicador de actividad
} EFI_ROOM_USER;

typedef struct {
	UINTN Host;
	UINTN Server;
	UINTN Room;
} EFI_ROOM_DIRECTION;

typedef struct {
	EFI_ROOM_USER User;
	CHAR16* Message;
	EFI_TIME MessageSendTime;
} EFI_ROOM_CHAT_ENTRY;

typedef EFI_ROOM_USER EFI_ROOM_BOUNCH[EFI_ROOM_MAX_USERS];

typedef struct {
	// the users
	EFI_ROOM_BOUNCH RoomList;

	// this has been cleaned
	EFI_ROOM_CHAT_ENTRY Chat[20];

	EFI_ROOM_DIRECTION direction;
} EFI_ROOM;

#endif // !NET_PRINCIPIES

EFI_STATUS
InitializeNetwork
(

)
{
	EFI_STATUS Status;

	CHAR16 buffera[256];

	// Localizar el protocolo
	Status = gBS->LocateProtocol(
		&gEfiSimpleNetworkProtocolGuid,
		NULL,
		(VOID**)&Net
	);
	SetScreenAtribute(0, orange);
	printc(L"status locating the networck protocol");

	StatusToString(buffera, Status);
	NormalizeStatus(buffera);
	printc(L"\n");

	if (EFI_ERROR(Status)) {
		gBS->Stall(1000000);
	}

	// Iniciar el adaptador de red
	if (Net != NULL)
	Status = Net->Start(Net);

	SetScreenAtribute(0, orange);
	printc(L"status starting the net");
	gBS->Stall(1000000);

	NormalizeStatus(buffera);
	printc(L"\n");

	// Inicializar la red
	if (Net != NULL)
	Status = Net->Initialize(Net, 0, 0);
	SetScreenAtribute(0, orange);
	printc(L"status initializing the net");
	gBS->Stall(1000000);
	NormalizeStatus(buffera);
	printc(L"\n");

	SetScreenAtribute(0, orange);

	printc(L"net initialized\n");

	gBS->Stall(1000000);

	return EFI_SUCCESS;

}

VOID
EFI_ROOM_SEND_MESSGAE
(
	IN struct EFI_ROOM_DIRECTION* Room,
	IN EFI_ROOM_CHAT_ENTRY* Message
)
{

}

#endif // !KERNEL_NET_H
