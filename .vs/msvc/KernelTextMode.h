﻿/*
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
* KernelTextMode.h
+++*/
#include <efi.h>
#include <efilib.h>
#include "KernelProcess.h"
#include "STFPrint.h"

// ITS MY FONT :)
#include "TextModeMapBits.h"

// S-SUN kernel textmode
#ifndef _KERNEL_TEXTMODE_
#define _KERNEL_TEXTMODE_

#define SCREEN_WIDTH 430
#define SCREEN_HEIGHT 360

#define BUFFER_HEIGHT 1200  // Tamaño del buffer mayor que la pantalla

#define BUFFERSCREEN_MEM_Dir 0x000000032
#define SEGC_PROTOCOL 0x002

typedef enum KERNEL_STATUS;

UINTN DRAWING_CURSOR_POSX;
UINTN DRAWING_CURSOR_POSY;
UINTN DRAWING_CURSOR_SIZEX;
UINTN DRAWING_CURSOR_SIZEY;

EFI_GRAPHICS_OUTPUT_BLT_PIXEL DRAWING_CURSOR_COLOR;

#define DRW_X DRAWING_CURSOR_POSX
#define DRW_Y DRAWING_CURSOR_POSY
#define DRW_SX DRAWING_CURSOR_SIZEX
#define DRW_SY DRAWING_CURSOR_SIZEY
#define DRW_COL DRAWING_CURSOR_COLOR

#define DRW_DOWN DRW_DOWN_F()

Pixels bufferscreen[999999];

#define int UINTN
#define void VOID
#define string CHAR16
#define bool BOOLEAN
#define true TRUE
#define false FALSE

int bufferlines;

int SMODE;

int pixels;
int globMaxColumns;
int globMaxRows;

EFI_HANDLE globalimagehandle;
EFI_SYSTEM_TABLE* globalsystemtable;

#define PIXELCOL EFI_GRAPHICS_OUTPUT_BLT_PIXEL
#define RESERVED_SYSTEM_PROCESS -1

EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;

// Función para dibujar un rectángulo
EFI_STATUS DrawRectangle(EFI_GRAPHICS_OUTPUT_PROTOCOL* GraphicsOutput, UINT32 X, UINT32 Y, UINT32 Width, UINT32 Height, EFI_GRAPHICS_OUTPUT_BLT_PIXEL Color) {
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color = Color;

    gop->Blt(gop, &color, EfiBltVideoFill, 0, 0, X, Y, Width, Height, 0);
    /*
    for (UINT32 i = 0; i < Height; i++) {
        for (UINT32 j = 0; j < Width; j++) {
            EFI_GRAPHICS_OUTPUT_BLT_PIXEL color = Color;
            gop->Blt(gop, &color, EfiBltVideoFill, 0, 0, X + j, Y + i, Width, Height, 0);
        }
    }
    */
    return EFI_SUCCESS;
};

UINTN horizontalResolution;
UINTN verticalResolution;

// define the colors
PIXELCOL black = { 0, 0, 0, 0 };
PIXELCOL gray = { 200, 200, 200, 0 };
PIXELCOL lightgray = { 225, 225, 225, 0 };
PIXELCOL darkgray = { 100, 100, 100, 0 };
PIXELCOL white = { 255, 255, 255, 0 };
PIXELCOL red = { 0, 0, 255, 0 };
PIXELCOL brred = { 100, 100, 255, 0 };
PIXELCOL darkred = { 0, 0, 100, 0 };
PIXELCOL orange = { 0, 150, 255, 0 };
PIXELCOL brorange = { 50, 150, 255, 0 };
PIXELCOL darkorange = { 0, 50, 155, 0 };
PIXELCOL yellow = { 0, 210, 235, 0 };
PIXELCOL bryellow = { 0, 230, 255, 0 };
PIXELCOL darkyellow = { 0, 110, 135, 0 };
PIXELCOL green = { 0, 255, 0, 0 };
PIXELCOL brgreen = { 100, 255, 100, 0 };
PIXELCOL darkgreen = { 0, 155, 0, 0 };
PIXELCOL cyan = { 255, 255, 0, 0 };
PIXELCOL darkcyan = { 155, 155, 0, 0 };
PIXELCOL brcyan = { 255, 255, 100, 0 };
PIXELCOL blue = { 200, 0, 0, 0 };
PIXELCOL brblue = { 255, 100, 100, 0 };
PIXELCOL darkblue = { 100, 0, 0, 0 };

typedef struct
{
    struct
    {
        PIXELCOL
            TEXT;
        PIXELCOL
            BG;
        UINTN
            size;
    } *atributes;
} MoonScreemConio;

Pixels Empty;

MoonScreemConio* Conio;

        int
            cursorx;
        int
            cursory;

        int
            screenscroll;

        void
            printc
            (
                string* a
            );

void
ClearScreen
(
)
{
     cursorx = 0;
     cursory = 0;
     bufferlines = 0;
     // Limpiar el buffer
     int totalpixels = pixels;
     for (UINTN y = 0; y < totalpixels; y++) {
         bufferscreen[--pixels] = Empty;  // Asignar color negro o el valor deseado para "vacío"
     }
     pixels = 0;
     screenscroll = 0;
     DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Conio->atributes->BG);
}

PIXELCOL
AtoC
(
    CHAR16* TheString
)
{
    if (StrCmp(TheString, L"black") == 0) {
        return black;
    }
    else if (StrCmp(TheString, L"white") == 0) {
        return white;
    }
    else if (StrCmp(TheString, L"gray") == 0) {
        return gray;
    }
    else if (StrCmp(TheString, L"red") == 0) {
        return red;
    }
    else if (StrCmp(TheString, L"darkred") == 0) {
        return darkred;
    }
    else if (StrCmp(TheString, L"brred") == 0) {
        return brred;
    }
    else if (StrCmp(TheString, L"orange") == 0) {
        return orange;
    }
    else if (StrCmp(TheString, L"darkorange") == 0) {
        return darkorange;
    }
    else if (StrCmp(TheString, L"brorange") == 0) {
        return brorange;
    }
    else if (StrCmp(TheString, L"yellow") == 0) {
        return yellow;
    }
    else if (StrCmp(TheString, L"darkyellow") == 0) {
        return darkyellow;
    }
    else if (StrCmp(TheString, L"bryellow") == 0) {
        return bryellow;
    }
    else if (StrCmp(TheString, L"green") == 0) {
        return green;
    }
    else if (StrCmp(TheString, L"darkgreen") == 0) {
        return darkgreen;
    }
    else if (StrCmp(TheString, L"brgreen") == 0) {
        return green;
    }
    else if (StrCmp(TheString, L"blue") == 0) {
        return blue;
    }
    else if (StrCmp(TheString, L"darkblue") == 0) {
        return darkblue;
    }
    else if (StrCmp(TheString, L"brblue") == 0) {
        return brblue;
    }
    else if (StrCmp(TheString, L"cyan") == 0) {
        return cyan;
    }
    else if (StrCmp(TheString, L"darkcyan") == 0) {
        return darkcyan;
    }
    else if (StrCmp(TheString, L"brcyan") == 0) {
        return brcyan;
    }
    else {
        return white;
    }
}

CHAR16
CtoA
(
    PIXELCOL* TheCol
)
{
    if (TheCol == &black) return L"black";
    if (TheCol == &white) return L"white";
    if (TheCol == &gray) return L"gray";
    if (TheCol == &darkgray) return L"darkgray";
    if (TheCol == &darkred) return L"darkred";
    if (TheCol == &red) return L"red";
    if (TheCol == &brred) return L"brred";
    if (TheCol == &darkorange) return L"darkorange";
    if (TheCol == &orange) return L"orange";
    if (TheCol == &brorange) return L"brorange";
    if (TheCol == &darkyellow) return L"darkyellow";
    if (TheCol == &yellow) return L"yellow";
    if (TheCol == &bryellow) return L"bryellow";
    if (TheCol == &darkgreen) return L"darkgreen";
    if (TheCol == &green) return L"green";
    if (TheCol == &brgreen) return L"brgreen";
    if (TheCol == &darkcyan) return L"darkcyan";
    if (TheCol == &cyan) return L"cyan";
    if (TheCol == &brcyan) return L"brcyan";
    if (TheCol == &darkblue) return L"darkblue";
    if (TheCol == &blue) return L"blue";
    if (TheCol == &brblue) return L"brblue";

    return L"white";
}

void
initializeMoonScreen
(
)
{
    horizontalResolution = gop->Mode->Info->HorizontalResolution;
    verticalResolution = gop->Mode->Info->VerticalResolution;

    globalsystemtable->ConOut->QueryMode(globalsystemtable->ConOut, globalsystemtable->ConOut->Mode->Mode, &globMaxColumns, &globMaxRows);
    horizontalResolution = gop->Mode->Info->HorizontalResolution;
    verticalResolution = gop->Mode->Info->VerticalResolution;
    Conio->atributes->TEXT = white;
    Conio->atributes->BG = black;
    Conio->atributes->size = 1;
    cursorx = 0;
    cursory = 0;
    screenscroll = 0;

    bufferlines = 0;
    pixels = 0;
    Empty.x = NULL;
    Empty.y = NULL;
    Empty.color = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){ 0, 0, 0, 0 };
}

void
SetScreenAtribute
(
    int* atribute,
    PIXELCOL value
)
{
    PIXELCOL* e = &value;
    if (atribute == 0) {
        Conio->atributes->TEXT = (*e);
    }
    else if (atribute == ((UINTN) 1)) {
        Conio->atributes->BG = (*e);
    }
}

// Función para dibujar un píxel en la pantalla
VOID
draw_pixel
(
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    int x,
    int y,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color
) {
    bool pixel_found = false;
    int pixel_id = -1;

    // Buscar el píxel en bufferscreen
    if (true) {
        for (UINTN i = 0; i < pixels; i++) {
            if ((bufferscreen[i].x == x) == (bufferscreen[i].y == y)) {
                if ((bufferscreen[i].x == x) == true) {
                    pixel_found = true;
                    pixel_id = i;
                    break;
                }
            }
            if ((bufferscreen[i].x == NULL)) {
                break;
            }
        }
    }

    if (!pixel_found) {
        // Crear un nuevo píxel
        if (pixels >= 999999) {
            Print(L"you cant write in a buffer that is filled");
            return;
        }

        Pixels new_pixel;
        new_pixel.color = color;
        new_pixel.x = x;
        new_pixel.y = y;

        bufferscreen[pixels++] = new_pixel;

        // Actualizar bufferlines y manejar scroll
        if (y > bufferlines) {
            bufferlines = y;
        }

        if (bufferlines > verticalResolution) {
            screenscroll = bufferlines - verticalResolution;
        }
    }
    else {
        // Actualizar color y posición del píxel existente
        bufferscreen[pixel_id].color = color;
        bufferscreen[pixel_id].x = x;
        bufferscreen[pixel_id].y = y;
    }
}

void
draw_pixelp
(
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    UINTN x,
    UINTN y,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color,
    UINTN process
)
{
    //EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel = color;
    //gop->Blt(gop, &pixel, EfiBltVideoFill, 0, 0, x, y, 1, 1, 0);
    Pixels a;
    a.color = color;
    a.x = x;
    a.y = y;
    if (y > ALLPROCESS[process].bufferlines) {
        ALLPROCESS[process].bufferlines = y;
    }
    ALLPROCESS[process].pixels++;
    ALLPROCESS[process].CONTENT[ALLPROCESS[process].pixels] = a;
    if (ALLPROCESS[process].bufferlines > ALLPROCESS[process].verticalResolution)
    {
        ALLPROCESS[process].screenscroll = a.y - ALLPROCESS[process].verticalResolution;
    }
}

void
DRW_DOWN_F
(
)
{
    for (size_t j = 0; j < DRW_SY; j++)
    {
        for (size_t i = 0; i < DRW_SX; i++)
        {
            draw_pixel(gop, DRW_X + i, DRW_Y + j, DRW_COL);
        }
    }
}

// Función para dibujar una letra usando el mapa de píxeles
void 
draw_bitmaprt
(
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    int x,
    int y,
    CHAR16** bitmap,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color,
    UINTN size
)
{
    for (int i = 0; i < 8; i++) { // Itera filas del bitmap (eje y)
        for (int j = 0; j < 8; j++) { // Itera columnas del bitmap (eje x)
            if (bitmap[i][j] == L'1') { // Verifica si se debe dibujar un "píxel"
                for (int dy = 0; dy < size; dy++) { // Escala en el eje y
                    for (int dx = 0; dx < size; dx++) { // Escala en el eje x
                        draw_pixel(gop, (x + j * size) + dx, (y + i * size) + dy, color);
                    }
                }
            }
        }
    }
}

// Función para dibujar una letra usando el mapa de píxeles
void
draw_bitmaprtp
(
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    int x,
    int y,
    CHAR16** bitmap,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color,
    UINTN size,
    INTN process
)
{
    for (int i = 0; i < 8; i++) { // Itera filas del bitmap (eje y)
        for (int j = 0; j < 8; j++) { // Itera columnas del bitmap (eje x)
            if (bitmap[i][j] == L'1') { // Verifica si se debe dibujar un "píxel"
                for (int dy = 0; dy < size; dy++) { // Escala en el eje y
                    for (int dx = 0; dx < size; dx++) { // Escala en el eje x
                        draw_pixelp(gop, (x + j * size) + dx, (y + i * size) + dy, color, process);
                    }
                }
            }
        }
    }
}

void
draw_bitmap2rt
(
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    int x,
    int y,
    CHAR16** bitmap,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color,
    UINTN size
)
{
    for (int i = 0; i < 12; i++) { // Itera filas del bitmap (eje y)
        for (int j = 0; j < 10; j++) { // Itera columnas del bitmap (eje x)
            if (bitmap[i][j] == L'1') { // Verifica si se debe dibujar un "píxel"
                for (int dy = 0; dy < size; dy++) { // Escala en el eje y
                    for (int dx = 0; dx < size; dx++) { // Escala en el eje x
                        draw_pixel(gop, (x + j * size) + dx, (y + i * size) + dy, color);
                    }
                }
            }
        }
    }
}

void
draw_bitmap2rtp
(
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    int x,
    int y,
    CHAR16** bitmap,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color,
    UINTN size,
    INTN process
)
{
    for (int i = 0; i < 12; i++) { // Itera filas del bitmap (eje y)
        for (int j = 0; j < 10; j++) { // Itera columnas del bitmap (eje x)
            if (bitmap[i][j] == L'1') { // Verifica si se debe dibujar un "píxel"
                for (int dy = 0; dy < size; dy++) { // Escala en el eje y
                    for (int dx = 0; dx < size; dx++) { // Escala en el eje x
                        draw_pixelp(gop, (x + j * size) + dx, (y + i * size) + dy, color,process);
                    }
                }
            }
        }
    }
}
#define draw_bitmap(gop,x,y,bitmap,color) if (process == RESERVED_SYSTEM_PROCESS) { draw_bitmaprt(gop,x,y,bitmap,color, size); } else { draw_bitmaprtp(gop,x,y,bitmap,color, size, process); }
#define draw_bitmap2(gop,x,y,bitmap,color) if (process == RESERVED_SYSTEM_PROCESS) { draw_bitmap2rt(gop,x,y,bitmap,color, size); } else { draw_bitmap2rtp(gop,x,y,bitmap,color, size , process); }

// Función para dibujar una letra
void
DrawLetter
(
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color,
    int x,
    int y,
    CHAR16* letter,
    UINT8 size,
    INT8 process
)
{
    int realx = (x * (8 * size));
    int realy = (y * (12 * size));
    if (StrCmp(letter, L"_") == 0) {
        draw_bitmap(gop, realx, realy, GuionBajo_bitmap, color);
    }
    else if (StrCmp(letter, L"+") == 0) {
        draw_bitmap(gop, realx, realy, Plus_bitmap, color);
    }
    else if (StrCmp(letter, L"[") == 0) {
        draw_bitmap(gop, realx, realy, corcheteab_bitmap, color);
    }
    else if (StrCmp(letter, L"]") == 0) {
        draw_bitmap(gop, realx, realy, corchetecerr_bitmap, color);
    }
    else if (StrCmp(letter, L"\x2a1") == 0) {
        draw_bitmap(gop, realx, realy, happyface_bitmap, color);
    }
    else if (StrCmp(letter, L"\x2a2") == 0) {
        draw_bitmap(gop, realx, realy, sadface_bitmap, color);
    }
    else if (StrCmp(letter, L"\x2a3") == 0) {
        draw_bitmap(gop, realx, realy, neutralface_bitmap, color);
    }
    else if (StrCmp(letter, L"\x2a4") == 0) {
        draw_bitmap(gop, realx, realy, rombo_bitmap, color);
    }
    else if (StrCmp(letter, L"\x2a5") == 0) {
        draw_bitmap(gop, realx, realy, corazon_bitmap, color);
    }
    else if (StrCmp(letter, L"\x2a6") == 0) {
        draw_bitmap(gop, realx, realy, coin_bitmap, color);
    }
    else if (StrCmp(letter, L"-") == 0) {
        draw_bitmap(gop, realx, realy, subtrac_bitmap, color);
    }
    else if (StrCmp(letter, L"*") == 0) {
        draw_bitmap(gop, realx, realy, Multipy_bitmap, color);
    }
    else if (StrCmp(letter, L"^") == 0) {
        draw_bitmap(gop, realx, realy, cosa_para_caritas_bitmap, color);
    }
    else if (StrCmp(letter, L"/") == 0) {
        draw_bitmap(gop, realx, realy, Diagonal_bitmap, color);
    }
    else if (StrCmp(letter, L"\\") == 0) {
        draw_bitmap(gop, realx, realy, Diagonal2_bitmap, color);
    }
    else if (StrCmp(letter, L":") == 0) {
        draw_bitmap(gop, realx, realy, DubbleDot_bitmap, color);
    }
    else if (StrCmp(letter, L";") == 0) {
        draw_bitmap(gop, realx, realy, DubbleComa_bitmap, color);
    }
    else if (StrCmp(letter, L".") == 0) {
        draw_bitmap(gop, realx, realy, Dot_bitmap, color);
    }
    else if (StrCmp(letter, L",") == 0) {
        draw_bitmap(gop, realx, realy, Coma_bitmap, color);
    }
    else if (StrCmp(letter, L"A") == 0) {
        draw_bitmap(gop, realx, realy, A_bitmap, color);
    }
    else if (StrCmp(letter, L"a") == 0) {
        draw_bitmap(gop, realx, realy, a_bitmap, color);
    }
    else if (StrCmp(letter, L"B") == 0) {
        draw_bitmap(gop, realx, realy, B_bitmap, color);
    }
    else if (StrCmp(letter, L"b") == 0) {
        draw_bitmap(gop, realx, realy, b_bitmap, color);
    }
    else if (StrCmp(letter, L"C") == 0) {
        draw_bitmap(gop, realx, realy, C_bitmap, color);
    }
    else if (StrCmp(letter, L"c") == 0) {
        draw_bitmap(gop, realx, realy, c_bitmap, color);
    }
    else if (StrCmp(letter, L"D") == 0) {
        draw_bitmap(gop, realx, realy, D_bitmap, color);
    }
    else if (StrCmp(letter, L"d") == 0) {
        draw_bitmap(gop, realx, realy, d_bitmap, color);
    }
    else if (StrCmp(letter, L"E") == 0) {
        draw_bitmap(gop, realx, realy, E_bitmap, color);
    }
    else if (StrCmp(letter, L"e") == 0) {
        draw_bitmap(gop, realx, realy, e_bitmap, color);
    }
    else if (StrCmp(letter, L"F") == 0) {
        draw_bitmap(gop, realx, realy, F_bitmap, color);
    }
    else if (StrCmp(letter, L"f") == 0) {
        draw_bitmap(gop, realx, realy, f_bitmap, color);
    }
    else if (StrCmp(letter, L"G") == 0) {
        draw_bitmap(gop, realx, realy, G_bitmap, color);
    }
    else if (StrCmp(letter, L"g") == 0) {
        draw_bitmap(gop, realx, realy, g_bitmap, color);
    }
    else if (StrCmp(letter, L"H") == 0) {
        draw_bitmap(gop, realx, realy, H_bitmap, color);
    }
    else if (StrCmp(letter, L"h") == 0) {
        draw_bitmap(gop, realx, realy, h_bitmap, color);
    }
    else if (StrCmp(letter, L"I") == 0) {
        draw_bitmap(gop, realx, realy, I_bitmap, color);
    }
    else if (StrCmp(letter, L"i") == 0) {
        draw_bitmap(gop, realx, realy, i_bitmap, color);
    }
    else if (StrCmp(letter, L"J") == 0) {
        draw_bitmap(gop, realx, realy, J_bitmap, color);
    }
    else if (StrCmp(letter, L"j") == 0) {
        draw_bitmap(gop, realx, realy, j_bitmap, color);
    }
    else if (StrCmp(letter, L"K") == 0) {
        draw_bitmap(gop, realx, realy, K_bitmap, color);
    }
    else if (StrCmp(letter, L"k") == 0) {
        draw_bitmap(gop, realx, realy, k_bitmap, color);
    }
    else if (StrCmp(letter, L"L") == 0) {
        draw_bitmap(gop, realx, realy, L_bitmap, color);
    }
    else if (StrCmp(letter, L"l") == 0) {
        draw_bitmap(gop, realx, realy, l_bitmap, color);
    }
    else if (StrCmp(letter, L"M") == 0) {
        draw_bitmap(gop, realx, realy, M_bitmap, color);
    }
    else if (StrCmp(letter, L"m") == 0) {
        draw_bitmap(gop, realx, realy, m_bitmap, color);
    }
    else if (StrCmp(letter, L"N") == 0) {
        draw_bitmap(gop, realx, realy, N_bitmap, color);
    }
    else if (StrCmp(letter, L"n") == 0) {
        draw_bitmap(gop, realx, realy, n_bitmap, color);
    }
    else if (StrCmp(letter, L"O") == 0) {
        draw_bitmap(gop, realx, realy, O_bitmap, color);
    }
    else if (StrCmp(letter, L"o") == 0) {
        draw_bitmap(gop, realx, realy, o_bitmap, color);
    }
    else if (StrCmp(letter, L"P") == 0) {
        draw_bitmap(gop, realx, realy, P_bitmap, color);
    }
    else if (StrCmp(letter, L"p") == 0) {
        draw_bitmap(gop, realx, realy, p_bitmap, color);
    }
    else if (StrCmp(letter, L"Q") == 0) {
        draw_bitmap(gop, realx, realy, Q_bitmap, color);
    }
    else if (StrCmp(letter, L"q") == 0) {
        draw_bitmap(gop, realx, realy, q_bitmap, color);
        }
    else if (StrCmp(letter, L"R") == 0) {
        draw_bitmap(gop, realx, realy, R_bitmap, color);
    }
    else if (StrCmp(letter, L"r") == 0) {
        draw_bitmap(gop, realx, realy, r_bitmap, color);
        }
    else if (StrCmp(letter, L"S") == 0) {
        draw_bitmap(gop, realx, realy, S_bitmap, color);
    }
    else if (StrCmp(letter, L"s") == 0) {
        draw_bitmap(gop, realx, realy, s_bitmap, color);
        }
    else if (StrCmp(letter, L"T") == 0) {
        draw_bitmap(gop, realx, realy, T_bitmap, color);
    }
    else if (StrCmp(letter, L"t") == 0) {
        draw_bitmap(gop, realx, realy, t_bitmap, color);
        }
    else if (StrCmp(letter, L"U") == 0) {
        draw_bitmap(gop, realx, realy, U_bitmap, color);
    }
    else if (StrCmp(letter, L"u") == 0) {
        draw_bitmap(gop, realx, realy, u_bitmap, color);
        }
    else if (StrCmp(letter, L"V") == 0) {
        draw_bitmap(gop, realx, realy, V_bitmap, color);
    }
    else if (StrCmp(letter, L"v") == 0) {
        draw_bitmap(gop, realx, realy, v_bitmap, color);
        }
    else if (StrCmp(letter, L"W") == 0) {
        draw_bitmap(gop, realx, realy, W_bitmap, color);
    }
    else if (StrCmp(letter, L"w") == 0) {
        draw_bitmap(gop, realx, realy, w_bitmap, color);
        }
    else if (StrCmp(letter, L"X") == 0) {
        draw_bitmap(gop, realx, realy, X_bitmap, color);
    }
    else if (StrCmp(letter, L"x") == 0) {
        draw_bitmap(gop, realx, realy, x_bitmap, color);
        }
    else if (StrCmp(letter, L"Y") == 0) {
        draw_bitmap(gop, realx, realy, Y_bitmap, color);
    }
    else if (StrCmp(letter, L"y") == 0) {
        draw_bitmap(gop, realx, realy, y_bitmap, color);
        }
    else if (StrCmp(letter, L"Z") == 0) {
        draw_bitmap(gop, realx, realy, Z_bitmap, color);
    }
    else if (StrCmp(letter, L"z") == 0) {
        draw_bitmap(gop, realx, realy, z_bitmap, color);
    }
    else if (StrCmp(letter, L"1") == 0) {
            draw_bitmap(gop, realx, realy, one_bitmap, color);
    }
    else if (StrCmp(letter, L"2") == 0) {
        draw_bitmap(gop, realx, realy, two_bitmap, color);
    }
    else if (StrCmp(letter, L"3") == 0) {
            draw_bitmap(gop, realx, realy, tree_bitmap, color);
    }
    else if (StrCmp(letter, L"4") == 0) {
                draw_bitmap(gop, realx, realy, four_bitmap, color);
    }
    else if (StrCmp(letter, L"5") == 0) {
        draw_bitmap(gop, realx, realy, five_bitmap, color);
    }
    else if (StrCmp(letter, L"6") == 0) {
            draw_bitmap(gop, realx, realy, six_bitmap, color);
    }
    else if (StrCmp(letter, L"7") == 0) {
        draw_bitmap(gop, realx, realy, seven_bitmap, color);
    }
    else if (StrCmp(letter, L"8") == 0) {
        draw_bitmap(gop, realx, realy, eight_bitmap, color);
    }
    else if (StrCmp(letter, L"9") == 0) {
        draw_bitmap(gop, realx, realy, nive_bitmap, color);
    }
    else if (StrCmp(letter, L"0") == 0) {
        draw_bitmap(gop, realx, realy, cero_bitmap, color);
    }
    else if (StrCmp(letter, L">") == 0) {
        draw_bitmap(gop, realx, realy, mayorque_bitmap, color);
    }
    else if (StrCmp(letter, L"<") == 0) {
        draw_bitmap(gop, realx, realy, menorque_bitmap, color);
    }
    else if (StrCmp(letter, L"\"") == 0) {
        draw_bitmap(gop, realx, realy, comillas_bitmap, color);
    }
    else if (StrCmp(letter, L"#") == 0) {
        draw_bitmap(gop, realx, realy, hashtag_bitmap, color);
    }
    else if (StrCmp(letter, L"$") == 0) {
        draw_bitmap(gop, realx, realy, dollar_bitmap, color);
    }
    else if (StrCmp(letter, L"%") == 0) {
        draw_bitmap(gop, realx, realy, porcentage_bitmap, color);
    }
    else if (StrCmp(letter, L"&") == 0) {
        draw_bitmap(gop, realx, realy, and_bitmap, color);
    }
    else if (StrCmp(letter, L"(") == 0) {
        draw_bitmap(gop, realx, realy, parenteses1_bitmap, color);
    }
    else if (StrCmp(letter, L")") == 0) {
            draw_bitmap(gop, realx, realy, parentesis2_bitmap, color);
    }
    else if (StrCmp(letter, L"¿") == 0) {
        draw_bitmap(gop, realx, realy, interrogation1_bitmap, color);
        }
    else if (StrCmp(letter, L"?") == 0) {
            draw_bitmap(gop, realx, realy, interrogration2_bitmap, color);
            }
    else if (StrCmp(letter, L"¡") == 0) {
        draw_bitmap(gop, realx, realy, exclamacion1_bitmap, color);
    }
    else if (StrCmp(letter, L"!") == 0) {
        draw_bitmap(gop, realx, realy, exclamacion2_bitmap, color);
    }
    else if (StrCmp(letter, L"=") == 0) {
        draw_bitmap(gop, realx, realy, iguala_bitmap, color);
    }
    else if (StrCmp(letter, L"\a") == 0) {
        draw_bitmap2(gop, realx, realy - 2, clleno_bitmap, color);
    }
    else if (StrCmp(letter, L"\n") == 0) {
        draw_bitmap2(gop, realx, realy - 2, space_bitmap, color);
    }
    else if (StrCmp(letter, L" ") == 0) {
        draw_bitmap(gop, realx, realy, space_bitmap, color);
    }
    else if (StrCmp(letter, L"\t") == 0) {
        draw_bitmap(gop, realx, realy, space_bitmap, color);
    }
    else
    {
        draw_bitmap(gop, realx, realy, UnkNown_bitmap, color);

    }
}

void
SetScreenColor
(
    PIXELCOL text,
    PIXELCOL bg
)
{
    globalsystemtable->ConOut->QueryMode(globalsystemtable->ConOut, globalsystemtable->ConOut->Mode->Mode, &globMaxColumns, &globMaxRows);
    horizontalResolution = gop->Mode->Info->HorizontalResolution;
    verticalResolution = gop->Mode->Info->VerticalResolution;
    Conio->atributes->TEXT = text;
    Conio->atributes->BG = bg;
    Empty.x = NULL;
    Empty.y = NULL;
    Empty.color = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){ 0, 0, 0, 0 };
}

void
gotoxy
(
    int x,
    int y
)
{
    cursorx = x;
    cursory = y;
}

void
ChangeToGrapichalMode
(
)
{
    SMODE = 1;
}

void
ChangeToTextMode
(
)
{
    SMODE = 0;
}

void 
DrawScreen
(
) 
{
    if (screenscroll < verticalResolution) {
        if (SMODE == 0) {
            DrawRectangle(gop, 0, 0, horizontalResolution, verticalResolution, Conio->atributes->BG);
        }
    }
        for (UINTN i = 0; i < pixels; i++) {
            Pixels pixel = bufferscreen[i];
            if (pixel.x != NULL) {
                EFI_GRAPHICS_OUTPUT_BLT_PIXEL color = pixel.color;
                UINT16 xpos = pixel.x;
                UINT16 ypos = pixel.y;

                if (screenscroll < verticalResolution) {
                    if (SMODE == 0) {
                        gop->Blt(gop, &color, EfiBltVideoFill, 0, 0, xpos, ypos - screenscroll - 5, 1, 1, 0);
                    }
                    else
                    {
                        gop->Blt(gop, &color, EfiBltVideoFill, 0, 0, xpos, ypos + 2, 1, 1, 0);
                    }
                } else {
                    gop->Blt(gop, &color, EfiBltVideoFill, 0, 0, xpos, ypos + 2, 1, 1, 0);
                }
            }
    }
}

void
DrawProcess
(
    INTN process
)
{


    // Verificación adicional de coordenadas y tamaños
    INTN x = ALLPROCESS[process].WindowsState->x;
    INTN y = ALLPROCESS[process].WindowsState->y;
    UINTN width = ALLPROCESS[process].horizontalResolution;
    UINTN height = ALLPROCESS[process].verticalResolution;

    EFI_GRAPHICS_OUTPUT_BLT_PIXEL borderColor = { 255, 255, 255, 0 }; // Color blanco para el borde
    // Dibujar el borde del proceso
    gop->Blt(
        gop,
        &borderColor,
        EfiBltVideoFill,
        0,
        0,
        x - 6,
        y - 16,
        width + 2,
        height + 12,
        0
    );

    // Dibujar el fondo del proceso
    gop->Blt(
        gop,
        &ALLPROCESS[process].atributes->BG,
        EfiBltVideoFill,
        0,
        0,
        x - 5,
        y - 5,
        width,
        height,
        0
    );

    // Dibujar los píxeles del contenido del proceso
    for (UINTN i = 0; i < ALLPROCESS[process].pixels; i++) {
        Pixels pixel = ALLPROCESS[process].CONTENT[i];
        if (pixel.x != 0) { // Verificar que pixel.x no sea 0
            EFI_GRAPHICS_OUTPUT_BLT_PIXEL color = pixel.color;
             gop->Blt(
                gop,
                &color,
                EfiBltVideoFill,
                0,
                0,
                x + pixel.x,
                y + pixel.y,
                1,
                1,
                0
            );

        }
    }
}

void
printf
(
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL color,
    int* x,
    int* y,
    string* TEXT,
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL bgcolor,
    int size,
    int process
) 
{
    int currentx = x;
    int currenty = y;
    string FullMessage[100];

    int Lenght = StrLen(TEXT);

    for (UINTN TEXTLETTER = 0; TEXTLETTER < Lenght; TEXTLETTER++) {
        SPrint(FullMessage, sizeof(FullMessage), L"%c", TEXT[TEXTLETTER]);
        if (TEXT[TEXTLETTER] == L'\n') {
            currenty++; 
            currentx = 0;
        }
        else {
            DrawLetter(gop, bgcolor, currentx, currenty, L"\a",size ,process);
            DrawLetter(gop, color, currentx, currenty, FullMessage,size ,process);
            currentx++;
        }
    }
    return TEXT;
}

UINTN
CountOccurrencesf
(
    CHAR16* TEXT,
    CHAR16 character
)
{
    UINTN count = 0;
    for (UINTN i = 0; i < StrLen(TEXT); i++) {
        if (TEXT[i] == character) {
            count++;
        }
    }
    return count;
}

void
_printc
(
    CHAR16* ar,
    bool update,
    ...
)
{
    string* a = ar;

    printf(gop, Conio->atributes->TEXT, cursorx, cursory, a, Conio->atributes->BG, Conio->atributes->size,-1);
    UINTN linescount = CountOccurrencesf(a, L'\n');
    cursorx = cursorx + StrLen(a) - linescount;
    if (linescount != 0) {
        bufferlines += linescount;
        cursorx = 0;
        cursory = cursory + linescount;
    }
    if (update == true) {
        DrawScreen();
    }
}

void
printc
(
    CHAR16* a
)
{
    _printc(a,true);
}

void
printcu
(
    string* a
)
{
    _printc(a, false);
}

void
printp
(
    string* a,
    INTN process
)
{
    printf(gop, ALLPROCESS[process].atributes->TEXT, ALLPROCESS[process].Conoutpud->cursorx, ALLPROCESS[process].Conoutpud->cursory, a, ALLPROCESS[process].atributes->BG, ALLPROCESS[process].atributes->size, process);
    ALLPROCESS[process].Conoutpud->cursorx = ALLPROCESS[process].Conoutpud->cursorx + StrLen(a) - CountOccurrencesf(a, L'\n');
    if (CountOccurrencesf(a, L'\n') != 0) {
        ALLPROCESS[process].bufferlines += CountOccurrencesf(a, L'\n');
        ALLPROCESS[process].Conoutpud->cursorx = 0;
        ALLPROCESS[process].Conoutpud->cursory = ALLPROCESS[process].Conoutpud->cursory + CountOccurrencesf(a, L'\n');
    }
    DrawProcess(process);
}

CHAR16* ReadLine(CHAR16* prompt) {
    static CHAR16 Buffer[512]; // Estático para mantener la vida útil más allá del scope de la función
    SetScreenAtribute(0, brgreen);

    printc(prompt);
    UINTN Index = 0;
    UINTN Event;
    EFI_INPUT_KEY Key;
    SetScreenAtribute(0, brblue);
    gotoxy(cursorx + StrLen(prompt), cursory);
    InitializeLib(globalimagehandle, globalsystemtable);

    while (true) {
        uefi_call_wrapper(globalsystemtable->ConIn->ReadKeyStroke, 2, globalsystemtable->ConIn, &Key);
        if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
            Buffer[Index] = L'\0';
            break;
        }
        else if (Key.ScanCode == SCAN_ESC) {
            return NULL;
        }
        else if (Key.UnicodeChar == CHAR_BACKSPACE) {
            if (Index > 0) {
                gotoxy(cursorx - 1, cursory);
                printc(L" ");
                gotoxy(cursorx - 1, cursory);
                Buffer[--Index] = L'\0';
            }
        }
        else if (Key.UnicodeChar != 0) {
            Buffer[Index++] = Key.UnicodeChar;
            CHAR16 a[2] = { Key.UnicodeChar, '\0' };
            SetScreenAtribute(0, brblue);
            printc(a);
        }

        Buffer[Index] = L'\0';
        globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
    }
    printc(L"\n");
    return Buffer;
}

VOID
ReadLineSerius
(
    OUT CHAR16* prompt
)
{
    static CHAR16 Buffer[512]; // Estático para mantener la vida útil más allá del scope de la función

    UINTN Index = 0;
    UINTN Event;
    EFI_INPUT_KEY Key;

    InitializeLib(globalimagehandle, globalsystemtable);

    while (true) {
        uefi_call_wrapper(globalsystemtable->ConIn->ReadKeyStroke, 2, globalsystemtable->ConIn, &Key);
        if (Key.UnicodeChar == CHAR_CARRIAGE_RETURN) {
            Buffer[Index] = L'\0';
            break;
        }
        else if (Key.ScanCode == SCAN_ESC) {
            *prompt = NULL;
            return;
        }
        else if (Key.UnicodeChar == CHAR_BACKSPACE) {
            if (Index > 0) {
                gotoxy(cursorx - 1, cursory);
                printc(L" ");
                gotoxy(cursorx - 1, cursory);
                Buffer[--Index] = L'\0';
            }
        }
        else if (Key.UnicodeChar != 0) {
            Buffer[Index++] = Key.UnicodeChar;
            string a[100];
            SPrint(a, sizeof(a), L"%c", Key.UnicodeChar);
            printc(a);
            Buffer[Index] = L'\0';
        }

        Buffer[Index] = L'\0';
        globalsystemtable->BootServices->WaitForEvent(1, &globalsystemtable->ConIn->WaitForKey, &Event);
    }
    printc(L"\n");
    *prompt = Buffer;
}

#endif // !_KERNEL_TEXTMODE_