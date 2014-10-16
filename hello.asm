;YFOS
start:
    org 0x7c00    ;load myself
    jmp init
    db "CaoCaoCaoCaoCaoCaoCaoCaoCaoCao"
init:
    mov  ax,0     
    mov  ss,ax
    mov  sp,0x7c00 ;sp
    mov  ds,ax
    mov  es,ax
    ;%
    mov al,'%'
    mov ah,0x0e
    mov bh,0
    mov bl,1
    int 0x10
    call loaddisk
    ;call printMem
    ;call err
    ;jmp mark55aa
    ;jmp 0x8200
    call err
    jmp fan2
    jmp hlt
hlt:
    hlt
    jmp hlt

;Err Report
err:
    mov  si,errmsg    
errloop:
    mov  al,[si]   
    add  si,1
    cmp  al,0    
    je   errret       
    mov  ah,0x0e   ;print one char
    mov bl,4
    ;mov  bx,90     ;black color
    int  0x10      ;video bios
    jmp  errloop
errret:
    ;mov al,cl
    ;mov ah,0x0e
    ;mov bx,14
    ;int 0x10
    ret
errmsg:
    db   0x0a,0x0a ;换行
    db   "Yafacex OS boot error!!!?"
    db   0x0a      ;换行
    db   0

loaddisk:
    mov ax,0x0820;0x8200
    mov es,ax
    mov ah,0;cylinder
    mov dh,0;head
    mov cl,2;fan/sector
    
    mov ah,2;read disk
    mov al,1;one fan
    mov bx,0
    mov dl,0;floppyA
    int 0x13;bios disk
    jc err
    ;call err
    ret

printMem:
    ;@
    mov al,64
    mov ah,0x0e
    mov bh,0
    mov bl,1
    int 0x10
    ;\n
    mov al,0x0a
    mov ah,0x0e
    mov bh,0
    mov bl,1
    int 0x10
    ;print from mem 0x8200(cx -> dx) 8 character
    mov cx,0x8200
    mov si,cx
    mov dx,cx
    add dx,25
    ;from beginning
    ;mov ah,0x02
    ;mov bx,0
    ;int 0x10
printMemLoop:
    mov al,[si]
    add si,1
    cmp si,dx
    je printMemRet
    mov ah,0x0e
    mov bx,15
    int 0x10
    jmp printMemLoop
printMemRet:
    ret
mark55aa:
    resb 510-($-$$) ;fill to 510
    db   0x55,0xaa ;magic number
;fan 2
fan2:
    jmp fan2prog
fan2msg:
    db   0x0a,0x0a ;换行
    db   "Yafacex OS boot run in fan2!!!?"
    db   0x0a      ;换行
    db   0
;fan2 content
fan2prog:
    ;?
    mov al,'#'
    mov ah,0x0e
    mov bh,0
    mov bl,1
    int 0x10
    mov  ax,0     
    mov  ss,ax
    mov  sp,0x7c00;sp
    mov  ds,ax
    mov  es,ax
    ;mov ax,fan2
    ;cmp ax,510
    je fan2loop+0x8200-0x7c00
    mov  si,0x8202    
fan2loop:
    ;<
    mov al,'<'
    mov ah,0x0e
    mov bh,0
    mov bl,1
    int 0x10

    mov  al,[si]   
    add  si,1
    cmp  al,0    
    je   fan2ret+0x8200-0x7c00       
    mov  ah,0x0e   ;print one char
    mov  bx,15     ;black color
    int  0x10      ;video bios
    jmp  fan2loop+0x8200-0x7c00
fan2ret:
    jmp hlt2+0x8200-0x7c00
    ret
hlt2:
    hlt
    jmp hlt2+0x8200-0x7c00
    resb 1024-($-$$)
