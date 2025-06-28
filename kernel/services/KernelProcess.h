/*
Copyright (c) 1998  Intel Corporation
Todos los derechos reservados.

Copyright (c) 2019, TianoCore and contributors. All rights reserved.

SPDX-License-Identifier: BSD-2-Clause-Patent

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions de c�digo fuente deben conservar el aviso de copyright anterior, esta lista de condiciones y el siguiente descargo de responsabilidad.
2. Las redistribuciones en forma binaria deben reproducir el aviso de copyright anterior, esta lista de condiciones y el siguiente descargo de responsabilidad en la documentaci�n y/u otros materiales proporcionados con la distribuci�n.

Sujeto a los t�rminos y condiciones de esta licencia, cada titular de derechos de autor y colaborador concede por la presente a aquellos que reciben derechos bajo esta licencia una licencia perpetua, mundial, no exclusiva, sin cargo, libre de regal�as, irrevocable (excepto por incumplimiento de las condiciones de esta licencia) de patente para fabricar, hacer fabricar, usar, ofrecer a la venta, vender, importar y transferir de otro modo este software, donde dicha licencia se aplica �nicamente a las reclamaciones de patentes que:
(a) sus Contribuciones (los derechos de autor con licencia de los titulares de derechos de autor y las adiciones no protegibles por derechos de autor de los colaboradores, en forma de fuente o binaria) solas; o
(b) combinaci�n de sus Contribuciones con la obra de autor�a a la que se a�adieron tales Contribuciones por dicho titular de derechos de autor o colaborador, si, en el momento en que se a�adi� la Contribuci�n, dicha adici�n hace que tal combinaci�n sea necesariamente infringida. La licencia de patente no se aplicar� a ninguna otra combinaci�n que incluya la Contribuci�n.

DESCARGO DE RESPONSABILIDAD
AUNQUE ESTOY UTILIZANDO GNU-EFI Y QUEMU

       ESPERO QUE CON ESTO NO ME HAGAN NADA y pueda hacer el sistema operativo
*/
// Seriously, please don't do anything to me.

/*+++
* KernelProcess.h
+++*/

#include <efi.h>
#include <efilib.h>

// S-SUN kernel process
#ifndef _KERNEL_PROCES_
#define _KERNEL_PROCES_

// Pixels
#pragma pack(1)
typedef struct {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color;
    UINT16 x;
    UINT16 y;
} Pixels;
#pragma pack()



#endif // !_KERNEL_PROCES_
