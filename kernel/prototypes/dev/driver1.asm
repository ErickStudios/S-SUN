MP
; MP means MyPackage
; dont edit the first 2 letters this indicates to the compiler what executable is

; this is assambled with SASM later

label 1
nop

label 2
nop

label 3
nop

label 4
nop

mov 10,1

; Speaker tone
label 1
    ; cx = tone
    mov cx,cl

    mov bx,ax

    int 14,0

int 33,0

; Speaker off
label 2
    ; cx = tone , 0 = off
    mov cx,0

    mov bx,ax

int 33,0

; Speaker install
label 3
    ; cx = driver
    ; cl = path
    ; ch = attributes path
    ; ax = attributes path end

    mov cx, 13

    mov cl,0

    mov ch,44
    mov ax,100

    ; install
    int 41

int 33,0

; ax: duration
; cx, tone
label 4
    lea 56,10

    ; cx = tone
    mov cl,cx

    int 14,0

    int 10,ax

    mov cl,0

    int 14,0
int 33,0