#ifndef _INTH_
#define _INTH_

#include <efi.h>
#include <efilib.h>

INTN
Ctoi
(
	CHAR16* c
)
{
	INTN i = 0;
	if (c == L"0") {
		i = 0;
	}
	else if (c[0] == L'-') {
		i -= Atoi(c + 1);
	}
	else
	{
		i = Atoi(c);
	}
	return i;
}
#endif // !_INTH_
