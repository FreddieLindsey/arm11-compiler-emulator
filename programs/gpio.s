mov r2,#1
lsl r2,#9            ; mask for pin 3
ldr r0,=0x20200000   ; load pins 0-9
str r2,[r0]          ; sets up pin 3 as an output

mov r1,#1
lsl r1,#3            ; mask for pin 3
str r1,[r0,#40]      ; clear pin 3 

str r1,[r0,#28]      ; set pin 3
andeq r0, r0, r0     ; halt

