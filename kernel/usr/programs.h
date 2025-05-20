/*
programs.h

abstract:
	aditional customizable programs list for S-SUN mods
*/
#ifndef THRIDY_PARTY_PROGRAMS
#define THRIDY_PARTY_PROGRAMS

// include your programs manually

#include "./sources/program_list.h"

INT16
GetThridyPartyProgramsCount
(
)
{
	INT16 count = 0;
	while (ProgramsList[count].Name != NULL)
	{
		count++;
	}

	return count;
}

#endif