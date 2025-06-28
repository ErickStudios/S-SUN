/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
File Name:

	KernelBinary.h

----------------------------------------------------------------------------------------------------

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Abstract:

	the binary format of S-SUN

----------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------------------*/

#ifndef KERNEL_BINARY_H
#define KERNEL_BINARY_H

#include "../sources/KernelDef.h"

//
// assembly opcodes
// 
// Note: edit this opcodes can break the compatibility with the "vanila" S-SUN , i recommanded you
// that the new opcodes make it a interruption or put a reference that the asm programs for your mod
// need to be assembled again
// 

// mov
//	opcode:1
//
// add
//	opcode:2
//
// sub
//	opcode:3
//
// div
//	opcode:4
//
// mul
//	opcode:5
//
// inc
//	opcode:6
//
// dec
//	opcode:7
//
// jmp
//	opcode:8
//
// ret
//	opcode:9
//
// section
//	opcode:10
//
// int
//	opcode:11
//
// jq
//	opcode:12
//
// jg
//	opcode:13
//
// jng
//	opcode:14
//
// jnq
//	opcode:15
//
// 16 to up , the values ,with a offset of +16

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// prototypes
// ----------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------
// constants

//
// the begimer
//
#define mov_instruction							1

//
// math
//

#define add_instruction							2
#define sub_instruction							3
#define div_instruction							4
#define imul_instruction						5

//
// basic functions
//

#define incr_instruction						6
#define decr_instruction						7
#define jump_instruction						8

//
// inprecindible
//

#define ret_instruction							9
#define section_instruction						10
#define interruption_instruction				11

//
// jumps instructions
//

#define jq_instruction							12
#define jg_instruction							13
#define jng_instruction							14
#define jnq_instruction							15

//
// for interact with extern program system parts
//

#define extern_ptr_write_instruction			16
#define extern_ptr_get_instruction				17

//
// other functions
//
#define lea_instruction							18
#define nop_instruction							19

#define safetynow_for_up						20
#define NULL_PARAM								safetynow_for_up

//
// asm keywords
//

#define asm_comment								L';'

typedef CHAR16									_BINARY;

typedef struct {
	CHAR16										Vendor[20];
	CHAR16										Name[20];

	double										version;

	_BINARY										AssemblyCode[512];
}
Driver32, Device32
;

typedef struct {
	CHAR16										registeru_from;
	CHAR16										count_used_registres;

	VOID(*Function)();
} custom_sams_interruption;

custom_sams_interruption CustomInterruptions[100];

// ----------------------------------------------------------------------------------------------
// variables

prototype __int16								memory_acces[2000]; // 2000 vars

// ----------------------------------------------------------------------------------------------
// functions

//
// assambler functions
//

prototype void
sasm_mov
(
	CHAR16										Register,
	CHAR16										NewValue
);

prototype void
sasm_lea
(
	CHAR16										Register,
	CHAR16										Register2
);

prototype void
sasm_add
(
	CHAR16										Register,
	CHAR16										NewValue
);

prototype void
sasm_sub
(
	CHAR16										Register,
	CHAR16										NewValue
);

prototype void
sasm_div
(
	CHAR16										Register,
	CHAR16										NewValue
);

prototype void
sasm_mul
(
	CHAR16										Register,
	CHAR16										NewValue
);

prototype BOOLEAN
sasm_cmp
(
	CHAR16										Register,
	CHAR16										Value
);

prototype VOID
sasm_cint_dec
(
	CHAR16										id,
	CHAR16										registeru_from,
	CHAR16										count_used_registres,
	VOID										(*Function)()
);

prototype void
BinaryEx
(
	_BINARY*									p,
	BOOLEAN										debug
);

prototype BOOLEAN
IsBinaryApp
(
	_BINARY*									p
);

prototype UINT16
AsmSyntax
(
	CHAR16*										s
);

prototype _BINARY*
BuildAsmCode
(
	CHAR16*										code,
	BOOLEAN										debug
);

// ----------------------------------------------------------------------------------------------
// END prototypes
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// assembler
// ----------------------------------------------------------------------------------------------

typedef struct {
	CHAR16 opcodecompiled;

	CHAR16 p1compiled;
	CHAR16 p2compiled;
} SASM_LEXER_READY_FOR_OBJETIZATION;

typedef struct
{
	CHAR16* KeyWord;
	CHAR16 OpcodeInBinary;

	UINTN attributesnumber;
} SASM_LEXER_KEYWORD;

SASM_LEXER_KEYWORD SasmKeywords[] = {
	{ L"mov" , mov_instruction , 2},
	{ L"add" , add_instruction , 2},
	{ L"sub" , sub_instruction , 2},
	{ L"mul" , imul_instruction , 2},
	{ L"div" , div_instruction , 2},
	{ L"inc" , incr_instruction , 1},
	{ L"dec" , decr_instruction , 1},
	{ L"jmp" , jump_instruction , 1},
	{ L"lea" , lea_instruction , 1},	
	{ L"label" , section_instruction , 1},
	{ NULL , NULL }
};

UINT16
AsmSyntax
(
	CHAR16* s
)
{
	if (
		s[0] == L'\'' && s[1] == L'\\' && s[2] == L'n' && s[3] == L'\''
		)
	{
		return L'\n' + safetynow_for_up;
	}
	if (
		s[0] == L'\'' && s[1] == L'\\' && s[2] == L'0' && s[3] == L'\''
		)
	{
		return L'\0' + safetynow_for_up;
	}
	if (
		s[0] == L'\'' && s[2] == L'\''
		)
	{
		return s[1] + safetynow_for_up;
	}
	else {
		return Atoi(s) + safetynow_for_up;
	}
}

SASM_LEXER_READY_FOR_OBJETIZATION
SolveSasmLine
(
	CHAR16* Line
)
{
	UINTN cnt = 0;
	SASM_LEXER_READY_FOR_OBJETIZATION outpud;

	while (
		Line[cnt] != L' '
		)
	{
		cnt++;
	}

	CHAR16 keyword[10];
	UINTN chkeyword = 0;

	while (
		Line[cnt] != L' '
		)
	{
		keyword[chkeyword] = Line[cnt];
		chkeyword++;
		cnt++;
	}

	keyword[chkeyword] = 0;

	UINTN keywordtocheck = 0;

	while (
		SasmKeywords[keywordtocheck].KeyWord != NULL && StrCmp(keyword, SasmKeywords[keywordtocheck].KeyWord)
		)
	{
		keywordtocheck++;
	}

	outpud.opcodecompiled = SasmKeywords[keywordtocheck].OpcodeInBinary;

	for (size_t i = 0; i < SasmKeywords[keywordtocheck].attributesnumber; i++)
	{
		CHAR16 arg[10];
		UINTN argch = 0;
		while (
			Line[cnt] != L',' && Line[cnt] != L';' && Line[cnt]
			)
		{
			arg[argch] = Line[cnt];
			argch++;
			cnt++;
		}

		if (
			i == 1
			) {
			outpud.p1compiled = AsmSyntax(arg);
		}
		else {
			outpud.p2compiled = AsmSyntax(arg);
		}
	}
}

CHAR16*
SasmParseBinaryIns
(
	SASM_LEXER_READY_FOR_OBJETIZATION ins
)
{
	CHAR16 outpud[4];

	outpud[0] = ins.opcodecompiled;
	outpud[1] = ins.p1compiled;
	outpud[2] = ins.p2compiled;

	outpud[3] = 0;

	return outpud;
}

// ----------------------------------------------------------------------------------------------
// END assembler
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// functions
// ----------------------------------------------------------------------------------------------

//
// not for asm , for external programs
//

void
sasm_mov
(
	CHAR16 Register,
	CHAR16 NewValue
)
{
	//
	// action
	//

	memory_acces[Register] = NewValue;
}

void
sasm_lea
(
	CHAR16 Register,
	CHAR16 Register2
)
{
	//
	// action
	//

	memory_acces[Register] = memory_acces[Register2];
}


void
sasm_add
(
	CHAR16 Register,
	CHAR16 NewValue
)
{
	//
	// action
	//

	memory_acces[Register] += NewValue;
}

void
sasm_sub
(
	CHAR16 Register,
	CHAR16 NewValue
)
{
	//
	// action
	//

	memory_acces[Register] -= NewValue;
}

void
sasm_div
(
	CHAR16 Register,
	CHAR16 NewValue
)
{
	//
	// action
	//

	memory_acces[Register] /= NewValue;
}

void
sasm_mul
(
	CHAR16 Register,
	CHAR16 NewValue
)
{
	//
	// action
	//

	memory_acces[Register] *= NewValue;
}

prototype BOOLEAN
sasm_cmp
(
	CHAR16 Register,
	CHAR16 Value
)
{
	return memory_acces[Register] == Value;
}

/*
BinaryEx

Summary:
	executes a S-SUN binary program
*/
void
BinaryEx
(
	_BINARY* p,
	BOOLEAN debug
)
{
	INT8 TratarloComo = 0;
					// 0 = program
					// 1 = device
		
	// the reader
	unsigned __int64 reader;

	// registers

	// check the program type
	if (!(p[0] == L'M' && p[1] == L'P' && p[2] == 3)) {
		if (!(p[0] == L'E' && p[1] == L'A' && p[2] == 3))
		{
			if (debug) {
			}
			return; // invalid binary
		}
	}
	else {
		TratarloComo = 1;
	}

	// set to skip the program type
	memory_acces[10] = 3;

	// the loop
	while (
		p[memory_acces[10]] // the reader
		) {

		unsigned __int64 r = memory_acces[10];

		__int16 ch = p[r]; // the character

		__int16 p1 = p[r + 1] - safetynow_for_up; // param 1
		__int16 p2 = p[r + 2] - safetynow_for_up; // param 2

		__int16 p1r = memory_acces[p1]; // param 1 in register
		__int16 p2r = memory_acces[p2]; // param 2 in register

		if (
			ch == mov_instruction // mov
			)
		{
			memory_acces[p1] = p2;
		}
		if (
			ch == add_instruction // add
			)
		{
			memory_acces[p1] += p2r;
		}
		if (
			ch == sub_instruction // sub
			)
		{
			memory_acces[p1] -= p2r;
		}
		if (
			ch == div_instruction // div
			)
		{
			memory_acces[p1] /= p2r;
		}
		if (
			ch == imul_instruction // multiplicate
			)
		{
			memory_acces[p1] *= p2r;
		}
		if (
			ch == incr_instruction // incrementase
			)
		{
			memory_acces[p1]++;
		}
		if (
			ch == decr_instruction // decrementase
			)
		{
			memory_acces[p1]--;
		}
		if (
			ch == jump_instruction // jump
			)
		{
			unsigned __int64 search_s;
			while (
				(
					p[search_s] == section_instruction &&
					p[search_s + 1] == p1
					)
				&&
				p[search_s]
				&&
				p[p2] != nop_instruction
				)
				search_s++
				;
			memory_acces[10] = search_s;
		}
		if (
			ch == ret_instruction // ret
			)
		{
			memory_acces[10] = p2r;
		}
		if (
			ch == interruption_instruction
			)
		{
			if (
				p1 == 1
				) {
				CHAR16 chain[2] = { p2r, 0};
				printc(chain);
			}
			if (
				p1 == 2
				)
				ClearScreen();
				;
			if (
				p1 == 3 || p1 == 4
				)
				{
					PIXELCOL e;
					switch (p2r)
					{
					case 1:
						e = black;
						break;
					case 2:
						e = lightblack;
						break;
					case 3:
						e = darkgray;
						break;
					case 4:
						e = gray;
						break;
					case 5:
						e = lightgray;
						break;
					case 6:
						e = white;
						break;

					case 7:
						e = darkred;
						break;
					case 8:
						e = red;
						break;
					case 9:
						e = brred;
						break;

					case 10:
						e = darkorange;
						break;
					case 11:
						e = orange;
						break;
					case 12:
						e = brorange;
						break;

					case 13:
						e = darkyellow;
						break;
					case 14:
						e = yellow;
						break;
					case 15:
						e = bryellow;
						break;

					case 16:
						e = darkgreen;
						break;
					case 17:
						e = green;
						break;
					case 18:
						e = brgreen;
						break;

					case 19:
						e = darkcyan;
						break;
					case 20:
						e = cyan;
						break;
					case 21:
						e = brcyan;
						break;

					case 22:
						e = darkteal;
						break;
					case 23:
						e = teal;
						break;
					case 24:
						e = brteal;
						break;

					case 25:
						e = darkblue;
						break;
					case 26:
						e = blue;
						break;
					case 27:
						e = brblue;
						break;
					default:
						e = gray;
						break;
					}
					SetScreenAtribute(p1 - 3, e);
				}
			if (
				p1 == 5
				)
			{
				gotoxy(p2r, cursory);
			}
			if (
				p1 == 7
				)
			{
				gotoxy(cursorx, p2r);
			}
			if (
				p1 == 8
				) {
				CHAR16 chain[2] = { p2r, 0 };
				printcu(chain); // print without update scree
			}
			if (
				p1 == 9
				)
			{
				DrawScreen();
			}
			if (
				p1 == 10
				)
			{
				gBS->Stall(p2r);
			}
			if (
				p1 == 11
				)
			{
				gBS->WaitForEvent(1, &gST->ConIn->WaitForKey, 1);
			}
			if (
				p1 == 12
				)
			{
				EFI_INPUT_KEY Key;

				gST->ConIn->ReadKeyStroke(gST->ConIn, &Key);

				memory_acces[p2] = Key.ScanCode;
				memory_acces[p2 + 1] = Key.UnicodeChar;
			}
		}
		if (
			ch == jq_instruction
			) {
			if (
				memory_acces[11] == memory_acces[12]
				)
			{
				unsigned __int64 search_s;
				while (
					(
						p[search_s] == section_instruction &&
						p[search_s + 1] == p1
						)
					&&
					p[search_s]
					)
					search_s++
					;
				memory_acces[10] = search_s;
			}
		}
		if (
			ch == jnq_instruction
			) {
			if (
				memory_acces[11] != memory_acces[12]
				)
			{
				unsigned __int64 search_s;
				while (
					(
						p[search_s] == section_instruction &&
						p[search_s + 1] == p1
						)
					&&
					p[search_s]
					)
					search_s++
					;
				memory_acces[10] = search_s;
			}
		}
		if (
			ch == jg_instruction
			) {
			if (
				memory_acces[11] > memory_acces[12]
				)
			{
				unsigned __int64 search_s;
				while (
					(
						p[search_s] == section_instruction &&
						p[search_s + 1] == p1
						)
					&&
					p[search_s]
					)
					search_s++
					;
				memory_acces[10] = search_s;
			}
		}
		if (
			ch == jng_instruction
			) {
			if (
				memory_acces[11] < memory_acces[12]
				)
			{
				unsigned __int64 search_s;
				while (
					(
						p[search_s] == section_instruction &&
						p[search_s + 1] == p1
						)
					&&
					p[search_s]
					)
					search_s++
					;
				memory_acces[10] = search_s;
			}
		}
		if (
			ch == extern_ptr_write_instruction
			)
		{
			if (
				p1r != NULL
				)
			{
				*((INT16*)p1r) = p2r;  // Asegura que se trata como un puntero de tipo co
			}
		}
		if (
			ch == extern_ptr_get_instruction
			)
		{
			memory_acces[p2] = *((INT16*)p1r);
		}

		memory_acces[10]++; // next instruction
	}
}

/*
IsBinaryApp

Summary:
	check if a text is a binary app
*/
BOOLEAN
IsBinaryApp
(
	_BINARY* p
)
{
	if (!(p[0] == L'E' && p[1] == L'A' && p[2] == 3)) 
	{
		return 0; // invalid program
	}

	return 1;
}

_BINARY hello_bin[] = {
	// the binary representation of the code											// asm equal																					// asm equal

	L'E',L'A',3,																		// xd?

	mov_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),				// mov 1,1

	interruption_instruction , ((2) + safetynow_for_up), NULL_PARAM,					// int 2

	mov_instruction , ((1) + safetynow_for_up), ((18) + safetynow_for_up),				// mov 1,18
	interruption_instruction , ((3) + safetynow_for_up), ((1) + safetynow_for_up),		// omt 3,1

	mov_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),				// mov 1,1
	interruption_instruction , ((4) + safetynow_for_up), ((1) + safetynow_for_up),		// int 4,1

	mov_instruction , ((1) + safetynow_for_up), ((L'h') + safetynow_for_up),			// mov 1,'h'
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	mov_instruction , ((1) + safetynow_for_up), ((L'e') + safetynow_for_up),			// mov 1,'e'
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	mov_instruction , ((1) + safetynow_for_up), ((L'l') + safetynow_for_up),			// mov 1,'l'
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	mov_instruction , ((1) + safetynow_for_up), ((L'l') + safetynow_for_up),			// mov 1,'l'
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	mov_instruction , ((1) + safetynow_for_up), ((L'o') + safetynow_for_up),			// mov 1,'o'
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	mov_instruction , ((1) + safetynow_for_up), ((L' ') + safetynow_for_up),			// mov 1,' '
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	mov_instruction , ((1) + safetynow_for_up), ((27) + safetynow_for_up),				// mov 1,27
	interruption_instruction , ((3) + safetynow_for_up), ((1) + safetynow_for_up),		// int 3,1

	mov_instruction , ((1) + safetynow_for_up), ((L'w') + safetynow_for_up),			// mov 1,'w'
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	mov_instruction , ((1) + safetynow_for_up), ((L'o') + safetynow_for_up),			// mov 1,'o'
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	mov_instruction , ((1) + safetynow_for_up), ((L'r') + safetynow_for_up),			// mov 1,'r'
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	mov_instruction , ((1) + safetynow_for_up), ((L'l') + safetynow_for_up),			// mov 1,'l'
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	mov_instruction , ((1) + safetynow_for_up), ((L'd') + safetynow_for_up),			// mov 1,'d'
	interruption_instruction , ((1) + safetynow_for_up), ((1) + safetynow_for_up),		// int 1,1

	L'\0'																				// ENDPROGRAM
};

_BINARY*
BuildAsmCode
(
	CHAR16* code,
	BOOLEAN debug
)
{
	_BINARY outpud[1024];
	UINTN ch = 3;
	UINTN asmch = 0;

	outpud[0] = code[0];
	outpud[1] = code[1];
	outpud[2] = 3;

	ch = 2;

	while (code[asmch])
	{
		if (code[asmch] != L'\n') {
			CHAR16 instruction[] = { code[asmch] , code[asmch + 1] , code[asmch + 2] , 0 };

			// instruction

			outpud[ch] =
				StrCmp(instruction, L"mov") == 0 ? mov_instruction :
				StrCmp(instruction, L"add") == 0 ? add_instruction :
				StrCmp(instruction, L"sub") == 0 ? sub_instruction :
				StrCmp(instruction, L"div") == 0 ? div_instruction :
				StrCmp(instruction, L"mlt") == 0 ? imul_instruction :
				StrCmp(instruction, L"inc") == 0 ? incr_instruction :
				StrCmp(instruction, L"dec") == 0 ? decr_instruction :
				StrCmp(instruction, L"int") == 0 ? interruption_instruction :
				StrCmp(instruction, L"jmp") == 0 ? jump_instruction :
				StrCmp(instruction, L"jgr") == 0 ? jg_instruction :
				StrCmp(instruction, L"jng") == 0 ? jng_instruction :
				StrCmp(instruction, L"jeq") == 0 ? jq_instruction :
				StrCmp(instruction, L"jnq") == 0 ? jnq_instruction : safetynow_for_up
				;
			ch++;
			asmch += 4;

			// syntax
			CHAR16 a[10];
			UINTN index = 0;

			while (
				code[asmch] != L','
				)
			{
				a[index] = code[asmch];
				index++;
				asmch++;
			}

			asmch++;
			a[index] = L'\0';

			// add syntax
			UINT16 s = AsmSyntax(a);
			outpud[ch] = s;

			// go to value input

			ch++;

			if (
				(
					StrCmp(instruction, L"inc") == 0 ? incr_instruction :
					StrCmp(instruction, L"dec") == 0 ? decr_instruction :
					StrCmp(instruction, L"jmp") == 0 ? jump_instruction :
					StrCmp(instruction, L"jgr") == 0 ? jg_instruction :
					StrCmp(instruction, L"jng") == 0 ? jng_instruction :
					StrCmp(instruction, L"jeq") == 0 ? jq_instruction :
					StrCmp(instruction, L"jnq") == 0 ? jnq_instruction : 0
					) == 0
				) {
				index = 0;

				while (
					code[asmch] != L'\n'
					)
				{
					a[index] = code[asmch];
					index++;
					asmch++;
				}

				// add syntax
				s = AsmSyntax(a);
				outpud[ch] = s;
			}

			ch++;
		}
		asmch++;
	}

	outpud[ch] = L'\0';

	return outpud;
}

// ----------------------------------------------------------------------------------------------
// END functions
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#endif // !KERNEL_BINARY_H
