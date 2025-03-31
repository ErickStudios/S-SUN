/*
Copyright (c) 1998  Intel Corporation
Todos los derechos reservados.

Copyright (c) 2019, TianoCore and contributors. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions de código fuente deben conservar el aviso de copyright anterior, esta lista de condiciones y el siguiente descargo de responsabilidad.
2. Las redistribuciones en forma binaria deben reproducir el aviso de copyright anterior, esta lista de condiciones y el siguiente descargo de responsabilidad en la documentación y/u otros materiales proporcionados con la distribución.

Sujeto a los términos y condiciones de esta licencia, cada titular de derechos de autor y colaborador concede por la presente a aquellos que reciben derechos bajo esta licencia una licencia perpetua, mundial, no exclusiva, sin cargo, libre de regalías, irrevocable (excepto por incumplimiento de las condiciones de esta licencia) de patente para fabricar, hacer fabricar, usar, ofrecer a la venta, vender, importar y transferir de otro modo este software, donde dicha licencia se aplica únicamente a las reclamaciones de patentes que:
(a) sus Contribuciones (los derechos de autor con licencia de los titulares de derechos de autor y las adiciones no protegibles por derechos de autor de los colaboradores, en forma de fuente o binaria) solas; o
(b) combinación de sus Contribuciones con la obra de autoría a la que se añadieron tales Contribuciones por dicho titular de derechos de autor o colaborador, si, en el momento en que se añadió la Contribución, dicha adición hace que tal combinación sea necesariamente infringida. La licencia de patente no se aplicará a ninguna otra combinación que incluya la Contribución.

DESCARGO DE RESPONSABILIDAD
AUNQUE ESTOY UTILIZANDO GNU-EFI Y QUEMU

       ESPERO QUE CON ESTO NO ME HAGAN NADA y pueda hacer el sistema operativo
*/
// Seriously, please don't do anything to me.

/*+++
* KernelAlgoritm.h
+++*/

#include <efi.h>
#include <efilib.h>

// S-SUN kernel Algoritm
#ifndef _KERNEL_ALGORITM_
#define _KERNEL_ALGORITM_

// this is SOOO COOL

extern EFI_SYSTEM_TABLE* globalsystemtable;

// Random
/**
Summary:
    this function make a random worcking for
    the other random functions
**/
UINT32
Random
(
    UINT32 seed
);

// RandomInRangeINWORCK_MANUALLY
/**
Summary:
    this functions is worck of RandomInRangeFINALLY
**/
UINT32
RandomInRangeINWORCK_MANUALLY
(
    UINT32 seed,
    UINT32 min, 
    UINT32 max
);

// RandomInRange
/**
Summary:
    this function gets the system time second and use
    it for get a random number in a range
**/
UINTN
RandomInRange
(
    UINT32 min,
    UINT32 max
);

// SumList
/**
Summary:
    this functions sums a list
**/
INTN
SumList
(
    INTN list[],
    UINTN size
);
#endif // !_KERNEL_ALGORITM_