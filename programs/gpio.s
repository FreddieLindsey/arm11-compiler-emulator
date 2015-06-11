mov r2,#1
lsl r2,#18           ; mask for pin 16
ldr r0,=0x20200004   ; load pins 10-19
str r2,[r0]          ; sets up pin 16 as an output

mov r1,#1
lsl r1,#16           ; mask for pin 16

mov r4,#2000000      ; store a very big number
reset:
mov r5,#0            ; store counter
str r1,[r0,#40]      ; clear pin 16
loopBody:
add r5,#1            ; increment counter by 1
cmp r4, r5           ; compare big number and counter
blt loopBody         ; if counter is not equal to big number, loop again
mov r5,#0            ; resets counter
str r1,[r0,#28]      ; set pin 16
loopBody2:
add r5,#1	     ; increments counter by 1
cmp r4, r5           ; compares big number and counter
blt loopBody2        ; if counter is not equal to big number, loop again
b reset              ; jump to reset
andeq r0, r0, r0     ; halt

