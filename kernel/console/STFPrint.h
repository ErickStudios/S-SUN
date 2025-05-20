/*+++
* STFPrint.h
+++*/

#include <efi.h>
#include <efilib.h>

#ifndef STFPrint
#define STFPrint

CHAR16*
FormatTextToSTF
(
    IN CONST CHAR16* text,
    VOID* e[]
)
{

    // Longitud del texto original
    UINTN length = StrLen(text);
    UINTN activateporcentage = 0; // Bandera para detectar `%`
    UINTN Constrictor = 0;        // �ndice de argumentos
    UINTN bufferSize = (length + 100) * sizeof(CHAR16); // Tama�o del buffer din�mico

    // Buffer para la cadena formateada
    CHAR16* FormattedStr = AllocateZeroPool(bufferSize);
    if (FormattedStr == NULL) {
        return NULL; // Error al asignar memoria
    }

    UINTN pos = 0; // Posici�n actual en FormattedStr

    for (UINTN i = 0; i < length; i++) {
        CHAR16 strl = text[i];

        // Manejar caracteres especiales
        switch (strl) {
        case L'%':
            activateporcentage = 1;
            break;
        case L's':
            if (activateporcentage == 1) {
                // Manejar marcador `%s`
                UINTN value = *(UINTN*)e[Constrictor];
                Constrictor++;

                // Convertir el n�mero a cadena
                CHAR16 buffer[20];
                UnicodeSPrint(buffer, sizeof(buffer), L"%s", value);

                // Concatenar al resultado
                StrCat(FormattedStr, buffer);
                pos += StrLen(buffer);

                activateporcentage = 0; // Reiniciar bandera
            }
            else {
                FormattedStr[pos++] = strl; // Si no es un marcador v�lido, tratar como texto normal
            }
            break;
        case L'd':
            if (activateporcentage == 1) {
                // Manejar marcador `%d`
                UINTN value = *(UINTN*)e[Constrictor];
                Constrictor++;

                // Convertir el n�mero a cadena
                CHAR16 buffer[20];
                UnicodeSPrint(buffer, sizeof(buffer), L"%d", value);

                // Concatenar al resultado
                StrCat(FormattedStr, buffer);
                pos += StrLen(buffer);

                activateporcentage = 0; // Reiniciar bandera
            }
            else {
                FormattedStr[pos++] = strl; // Si no es un marcador v�lido, tratar como texto normal
            }
            break;
        default:
            if (activateporcentage == 1) {
                // Si hay un `%` seguido de un car�cter no v�lido
                FormattedStr[pos++] = L'%';
                activateporcentage = 0;
            }
            FormattedStr[pos++] = strl; // Agregar car�cter al resultado
            break;
        }
    }

    // Terminar la cadena
    FormattedStr[pos] = L'\0';
    return FormattedStr;
}

#endif // !STFPrint
