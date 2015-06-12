; r4 register will set length of countdown
;cmp r4, #0            
;bne userlength       
ldr r4, =0xFFFFFF     ;
ldr r2,  =0x9         ; mask set to output 10 and 11
ldr r6,  =0x49        ; 
lsl r6,  #21          ; mask set to output 7, 8 and 9
ldr r12, =0x20200004  ; loads pins 10-19
ldr r0,  =0x20200000  ; loads pins 0-9 
str r2,  [r12]        ; output for 10 and 11 enabled
str r6,  [r0]         ; output for 7, 8 and 9 enabled
mov r7,  #7           ; masks for pin 7   on off
mov r8,  #8           ; masks for pin 8   on off
mov r9,  #9           ; masks for pin 9   on off
mov r10, #10          ; masks for pin 10  on off
mov r11, #11          ; masks for pin 11  on off
mov r6, r7
count:
mov r5,   #0          ; store counter
str r6,  [r0,#40]     ; clears pin 7
ldr r3,  =0xFFFFF
On:            ; loop for about 1 second
sub r3,r3,#1
cmp r3,#1
bne On
mov r5,#0          ; resets counter
str r6,  [r0,#28]     ; displays pin 7
ldr r4,  =0xFFFFF
Off:           ; loop for another second
sub r4,r4,#1
cmp r4,#1
bne Off
it1:
cmp r6, r7
bne it2
mov r6, r8
b count
it2:
cmp r6, r8
bne it3
mov r6, r9
b count
it3:
cmp r6, r9
bne it4
mov r6, r10
b count
it4:
cmp r6, r10
bne countend
mov r6, r11
b count
countend:
flash:
mov r6, #6
reset:
mov r5,   #0          ; store counter
str r7,  [r0,#40]     ; clears pin 7
str r8,  [r0,#40]     ; clears pin 8
str r9,  [r0,#40]     ; clears pin 9
str r10, [r0,#40]     ; clears pin 10
str r11, [r0,#40]     ; clears pin 11
ldr r3,  =0xFFFFF
waitOn:            ; loop for about 1 second
sub r3,r3,#1
cmp r3,#1
bne waitOn
mov r5,#0          ; resets counter
str r7,  [r0,#28]     ; displays pin 7
str r8,  [r0,#28]     ; displays pin 8
str r9,  [r0,#28]     ; displays pin 9
str r10, [r0,#28]     ; displays pin 10
str r11, [r0,#28]     ; displays pin 11
ldr r4,  =0xFFFFF
waitOff:           ; loop for another second
sub r4,r4,#1
cmp r4,#1
bne waitOff
cmp r6, #0
sub r6, r6, #1
bne reset            ; jump to reset
andeq r0,r0,r0        ; halt
