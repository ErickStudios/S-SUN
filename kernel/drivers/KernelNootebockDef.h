/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

	KernelNootebockDef.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

	definitions of noteboocks

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#include <efi.h>
#include <efilib.h>

#ifndef KernelNootebockDef_h
#define KernelNootebockDef_h

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
// constants

//
// brightness
//
#define MAX_BRIGHTNESS				255
#define MIN_BRIGHTNESS				0

//
// keys manager
//
#define BRIGHT_UP_KEY				L'a'
#define BRIGHT_DOWN_KEY				L'b'

#define prototype

// ----------------------------------------------------------------------------------------------
// variables

prototype INT16						laptop_birghtness = 0;
prototype BOOLEAN					IsLactuc = FALSE;				// me prestas tu lactuc
									// true for development tests

// ----------------------------------------------------------------------------------------------
// functions

prototype EFI_GRAPHICS_OUTPUT_BLT_PIXEL
PixelPlusBrightNess
(
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL	Color
)
;

prototype VOID
LaptopFunctionsBr
(
)
;

// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// functions
// ----------------------------------------------------------------------------------------------

EFI_GRAPHICS_OUTPUT_BLT_PIXEL
PixelPlusBrightNess
(
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL	Color
)
{
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL RETVAL = Color;

	if (
		!Color.Blue && !Color.Red && !Color.Green
		)
	{
		return Color;
	}

	RETVAL.Red = Color.Red;
	RETVAL.Green = Color.Green;
	RETVAL.Blue = Color.Blue;

	if (
		Color.Blue
		) {
		RETVAL.Blue = Color.Blue - (laptop_birghtness);
	}
	if (
		Color.Red
		) {
		RETVAL.Red = Color.Red - (laptop_birghtness);
	}
	if (
		Color.Green
		) {
		RETVAL.Green = Color.Green - (laptop_birghtness);
	}
	RETVAL.Reserved = Color.Reserved;

	return RETVAL;
}

// ----------------------------------------------------------------------------------------------
// END functions
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // !KernelNootebockDef_h
