#pragma once

////// !
////// ! crypto_pepe.h
////// !
////// ! pepe crypto system functions
////// !
////// !

#include "motor.h"

#include "../drivers/KernelAlgoritm.h"

t64
NumToMinamix
(
    t64                                             min,
    t64                                             max
)
{

    //
    // setuping
    //

    t64                                             result;

    t64                                             magic;
    t64                                             magic_result;

    t64                                             magic2;


    t64                                             random_result;

    //
    // operations (solo se que voy a estar minutos o horas aqui haciendo esto)
    //

    random_result   =                               RandomInRange(min,max);

    magic = magic + random_result / 10;

    //
    // big operator
    //

    magic2 = magic * (
                    (magic / 10) < 0.9 ?
                        (magic != 10 ? magic / 10 : magic)
                        :
                        (magic != 15 ? magic / 15 : magic)
                );

    //
    // que es esta mier-- O_-
    //

    magic = magic * (
            magic < 10 ?
                magic / magic2
                :
                (
                    (magic / (magic2 / 2) ^ 10) < 64 ?
                        (magic / (magic2 / 2) ^ 10) < 64
                    :
                    (magic / (magic2 / 2) ^ 10) < 64 +
                    (((magic / (magic2 / 2) ^ 10) < 64) * 10)
                )

    );

    //
    // more operations
    //

    magic_result = (magic * RandomInRange(min,max) / 10);

    result = magic_result / (1 + (RandomInRange(min,max) / 10));

    //
    // finally
    //

    while (result > max)
    {
        result /= 5;
    }

    return result;
}

u64
PepeEncryptV1
(
    u64                                             envid,
    u64                                             endrt
)
{

    //
    // setuping
    //

    EFI_TIME                                        TimeUse;

    u64                                             result;
    u64                                             end;

    u8                                              uyyy;


    //
    // action
    //

    end = endrt;

    result = envid;

    //
    // get crypto
    //

    gRT->GetTime(&TimeUse,0);

    uyyy = TimeUse.Second;

    if (
        uyyy
        >
        30
    )
    {
        uyyy = uyyy * TimeUse.Minute;
    }

    //
    // xors time
    //

    result = result << 1;

    result = (result * result) < 1024 ? (result * result) : 1024 * result;

    //
    // data parser
    //

    result = result < end ? result : end;

    return result;
}

u64
PepeEncryptV2
(
    u64                                             envid,
    u64                                             endrt
)
{

    //
    // setuping
    //

    u64                                             result;

    //
    // action
    //

    result = PepeEncryptV1(envid,endrt) * PepeEncryptV1(envid,endrt);

    if (
        result > endrt
    )
    {
        result = endrt;
    }

    return result;
}

CHAR16*
GenDevRandom
(
)
{
    CHAR16 result[970];

    result[0] = L'M';
    result[1] = L'P';
    result[2] = 3;

    for (size_t i = 3; i < RandomInRange(128,900); i++)
    {
        gBS->Stall(10);
        result[i] = RandomInRange(0,128);

        if (
            result[i] == 0
        )
        {
            break;
        }
    }

    return result;
}