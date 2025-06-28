.CODE
PUBLIC SetFrequency
PUBLIC EnableSpeaker
PUBLIC DisableSpeaker
PUBLIC SetChannelMode

; Configurar frecuencia del PIT (Canal 2)
SetFrequency PROC frequency:WORD
    ;mov dx, 43h          ; Registro de control del PIT
    ;mov al, 182          ; Modo 3 (Onda cuadrada)
    ;out dx, al

    mov dx, 42h          ; Puerto de datos del canal 2
    mov ax, frequency    ; Cargar la frecuencia en AX (16 bits)
    out dx, al
    mov al, ah           ; Enviar byte bajo
    out dx, al           ; Enviar byte alto

    ret
SetFrequency ENDP

; Encender el PC Speaker
EnableSpeaker PROC
    in  al, 61h      ; Leer estado actual
    or  al, 3        ; Activar bits 0 y 1 (Encender sonido)
    out 61h, al

    ret
EnableSpeaker ENDP

; Apagar el PC Speaker
DisableSpeaker PROC
    in  al, 61h      ; Leer estado actual
    and al, 0FCh     ; Apagar bits 0 y 1 (Silencio)
    out 61h, al

    ret
DisableSpeaker ENDP

; Configurar modo de operación del canal 2
SetChannelMode PROC mode:BYTE
    mov dx, 43h   ; Registro de control del PIT
    mov al, mode  ; Configurar modo (0-5)
    out dx, al

    ret
SetChannelMode ENDP

END