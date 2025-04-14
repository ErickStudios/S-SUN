#include <efi.h>
#include <efilib.h>

#ifndef _KEYBOARD_DRIVER_
#define _KEYBOARD_DRIVER_

typedef struct {
	CHAR16 UnicodeCharacterDown;
	CHAR16 UnicodeCharacterUp;
} KeyKeyb;

KeyKeyb key_map[] = {
    { (CHAR16)'a', (CHAR16)'A' },
    { (CHAR16)'b', (CHAR16)'B' },
    { (CHAR16)'c', (CHAR16)'C' },
    { (CHAR16)'d', (CHAR16)'D' },
    { (CHAR16)'e', (CHAR16)'E' },
    { (CHAR16)'f', (CHAR16)'F' },
    { (CHAR16)'g', (CHAR16)'G' },
    { (CHAR16)'h', (CHAR16)'H' },
    { (CHAR16)'i', (CHAR16)'I' },
    { (CHAR16)'j', (CHAR16)'J' },
    { (CHAR16)'k', (CHAR16)'K' },
    { (CHAR16)'l', (CHAR16)'L' },
    { (CHAR16)'m', (CHAR16)'M' },
    { (CHAR16)'n', (CHAR16)'N' },
    { (CHAR16)'o', (CHAR16)'O' },
    { (CHAR16)'p', (CHAR16)'P' },
    { (CHAR16)'q', (CHAR16)'Q' },
    { (CHAR16)'r', (CHAR16)'R' },
    { (CHAR16)'s', (CHAR16)'S' },
    { (CHAR16)'t', (CHAR16)'T' },
    { (CHAR16)'u', (CHAR16)'U' },
    { (CHAR16)'v', (CHAR16)'V' },
    { (CHAR16)'w', (CHAR16)'W' },
    { (CHAR16)'x', (CHAR16)'X' },
    { (CHAR16)'y', (CHAR16)'Y' },
    { (CHAR16)'z', (CHAR16)'Z' },
    { (CHAR16)'1', (CHAR16)'!' },
    { (CHAR16)'2', (CHAR16)'"' },
    { (CHAR16)'3', (CHAR16)'#' },
    { (CHAR16)'4', (CHAR16)'$' },
    { (CHAR16)'5', (CHAR16)'%' },
    { (CHAR16)'6', (CHAR16)'&' },
    { (CHAR16)'7', (CHAR16)'/' },
    { (CHAR16)'8', (CHAR16)'(' },
    { (CHAR16)'9', (CHAR16)')' },
    { (CHAR16)'0', (CHAR16)'=' },
};

#endif // !_KEYBOARD_DRIVER_
