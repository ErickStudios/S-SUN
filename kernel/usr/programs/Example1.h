// test program
// 
/*
Hello World
Example1
*/ 

// IMPORTANT bat params									 line 4: DisplayName, line 5: entry point
// !DONT CHANGE THE ORDER OF THE LINES 4 and 5!

/*
considerations:
	the param in line 5 must be exactly the expand of the macro ProgramEntryName
*/

// define ProgramEntryName before the inclution of ThridyP.h , please
#define ProgramEntryName Example1

// every time when you make a program for S-SUN please dont forget to 
#include "../ThridyP.h"

ProgramEntry(
	SetScreenAtribute(1, black);
	ClearScreen();
	SetScreenAtribute(0, gray);
	ClearScreen();
	printc(L"¡");
	SetScreenAtribute(0, brgreen);
	printc(L"hello ");
	SetScreenAtribute(0, brblue);
	printc(L"world");
	SetScreenAtribute(0, gray);
	printc(L"!");

	return;
)

#undef ProgramEntryName