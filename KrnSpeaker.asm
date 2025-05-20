.CODE
PUBLIC PlaySound 
PlaySound PROC frequency:DWORD, duration:DWORD 

; Configurar temporizador PIT 

mov al, 182 

mov dx, 43h 
out dx, al

; Establecer la frecuencia recibida como par�metro
mov eax, frequency
mov dx, 42h
out dx, eax    ; Enviar byte bajo
; out dx, al    ; Enviar byte alto

; speakerman
mov al, 3
mov dx, 61h
out dx, al

; Esperar la duraci�n dada como par�metro
mov ecx, duration

; the loop
DelayLoop: 
loop DelayLoop

; bye speakerman
mov al, 0 ; stop the timer
mov dx, 61h ; goodbyte speaker jajajaa
out dx, al ; no se

ret

PlaySound ENDP 
END
