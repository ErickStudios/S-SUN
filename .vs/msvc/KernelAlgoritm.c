#include "KernelAlgoritm.h"

// Random
/**
Summary:
this function make a random worcking for
the other random functions
**/
UINT32 Random(UINT32 seed)
{
    // Algoritmo de generación simple (Linear Congruential Generator - LCG)
    return (seed * 1103515245 + 12345) & 0x7FFFFFFF;
}

// RandomInRangeINWORCK_MANUALLY
/**
Summary:
this functions is worck of RandomInRangeFINALLY
**/
UINT32 RandomInRangeINWORCK_MANUALLY(UINT32 seed, UINT32 min, UINT32 max)
{
    UINT32 rand = Random(seed);
    return min + (rand % (max - min + 1));
}

// RandomInRange
/**
Summary:
this function gets the system time second and use
it for get a random number in a range
**/
UINTN RandomInRange(UINT32 min, UINT32 max)
{
    // ALGORITMMMMMMMM is the algoritm tttttttttttttttt
    EFI_TIME ALGORITMMMMMMMM;
    globalsystemtable->RuntimeServices->GetTime(&ALGORITMMMMMMMM, NULL);

    // siiiiid is the seed
    UINT32 siiiiid = ALGORITMMMMMMMM.Second;

    return RandomInRangeINWORCK_MANUALLY(siiiiid, min, max);
}

// SumList
/**
Summary:
this functions sums a list
**/
INTN SumList(INTN list[], UINTN size)
{
    INTN sum = 0;
    for (size_t i = 0; i < size; i++)
    {
        sum += list[i];
    }

    return sum;
}
