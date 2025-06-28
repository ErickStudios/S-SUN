/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

	KernelNet.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

	the networck functions

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#ifndef KERNEL_NET_H
#define KERNEL_NET_H

#include <efi.h>
#include <efilib.h>

#include "../sources/KernelDef.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
// variables


EFI_SIMPLE_NETWORK_PROTOCOL*								Net = NULL;

//
// ip config
//

EFI_MAC_ADDRESS*											MyIp;

//
// net settings
//

UINTN														NetHeaderSize;

// ----------------------------------------------------------------------------------------------
// functions

prototype EFI_MAC_ADDRESS*
StringToIp
(
	CHAR16*													Ip
);

prototype EFI_STATUS
InitializeNetwork
(
);

prototype EFI_STATUS
SendDataToIp
(
	EFI_MAC_ADDRESS*										Dest,
	VOID*													Buffer
);

prototype EFI_STATUS
ReciveDataFromIp
(
	EFI_MAC_ADDRESS*										Src,
	VOID*													Buffer,
	UINTN*													BufferSize
);

// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// functions
// ----------------------------------------------------------------------------------------------

/*
InitializeNetwork

Summary:
	initializes the network
*/
EFI_STATUS
InitializeNetwork
(
)
{
	//
	// variables
	//
	EFI_STATUS Status;
	CHAR16 buffera[256];

	//
	// locating protocols
	//
	
	Status = gBS->LocateProtocol(
		&gEfiSimpleNetworkProtocolGuid,
		NULL,
		(VOID**)&Net
	);

	//
	// print the status on protocol loc
	//
	SetScreenAtribute(0, orange);
	printc(L"status locating the networck protocol");

	StatusToString(buffera, Status);
	NormalizeStatus(buffera);
	printc(L"\n");

	if (EFI_ERROR(Status)) {
		gBS->Stall(1000000);
	}

	//
	// start adapter
	//

	if (Net != NULL)
	Status = Net->Start(Net);

	//
	// shows the starting adapter status
	//

	SetScreenAtribute(0, orange);
	printc(L"status starting the net");
	gBS->Stall(1000000);

	NormalizeStatus(buffera);
	printc(L"\n");

	//
	// starts the internet
	//

	if (Net != NULL)
	Status = Net->Initialize(Net, 0, 0);

	//
	// prints the status
	//

	SetScreenAtribute(0, orange);
	printc(L"status initializing the net");
	gBS->Stall(1000000);

	NormalizeStatus(buffera);
	printc(L"\n");

	SetScreenAtribute(0, orange);

	printc(L"net initialized\n");

	gBS->Stall(1000000);
	
	//
	// setting the ip
	//

	MyIp = AllocatePool(sizeof(EFI_MAC_ADDRESS));

	*MyIp->Addr = Net->Mode->PermanentAddress.Addr;

	return EFI_SUCCESS;

}

/*
SendDataToIp

Summary:
	send a data
*/
EFI_STATUS
SendDataToIp
(
	EFI_MAC_ADDRESS* Dest,
	VOID* Buffer
)
{
	//
	// check if the ip is spefic
	//

	if (
		!Dest
		)
		return EFI_UNSUPPORTED
		;

	//
	// check if the remitent is not send the data to troll
	//

	if (
		!Buffer
		)
		return EFI_UNSUPPORTED
		;

	//
	// variables
	//

	UINTN													BufferSize;

	UINT16													Protocol;

	//
	// init variables
	//

	BufferSize =											sizeof(Buffer);
	Protocol =												0;

	//
	// send the data
	//
	return Net->Transmit(
		&Net,												// the network
		NetHeaderSize,										// the header size
		BufferSize,											// the buffer size
		Buffer,												// the data
		MyIp,												// your ip
		Dest,												// the person that recive the data
		Protocol											// the protocol , is 0 because it says
															// "OPTIONAL"
	);
}

/*
ReciveDataFromIp

Summary:
	...
*/
EFI_STATUS
ReciveDataFromIp
(
	EFI_MAC_ADDRESS* Src,
	VOID* Buffer,
	UINTN* BufferSize
)
{
	//
	// check if the ip is spefic
	//

	if (
		!Src
		)
		return EFI_UNSUPPORTED
		;

	//
	// variables
	//

	UINT16													Protocol;
	EFI_SIMPLE_NETWORK_RECEIVE								Recive;

	//
	// init variables
	//

	Protocol =														0;

	//
	// recive it
	//

	return Net->Receive(
		&Net,
		0,
		BufferSize,
		Buffer,
		Src,
		MyIp,
		Protocol
	);
}

EFI_MAC_ADDRESS*
StringToIp
(
	CHAR16* Ip
)
{
	EFI_MAC_ADDRESS* IpR = AllocatePool(sizeof(EFI_MAC_ADDRESS));

	UINTN ch = 0;

	UINTN ip_part = 0;

	UINTN subch = 0;

	CHAR16 PartToInt[10];

	while (
		Ip[ch]
		)
	{
		if (
			Ip[ch] == L'.'
			)
		{
			subch = 0;
			IpR->Addr[ip_part] = Atoi(PartToInt);
			ip_part++;
		}
		else {
			PartToInt[subch] = Ip[ch];
			subch++;
		}

		ch++;
	}

	return IpR;
}

// ----------------------------------------------------------------------------------------------
// END functions
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // !KERNEL_NET_H
