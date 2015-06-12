; r3 register will set length of countdown
mov r3, #0            ; FOR TESTING ONLY DO NOT LEAVE IN
cmp r3, #0            ; if user has defined length of countdown....
bne initialization    ; use that number
ldr r3, =0xFFFFFF     ; else use default length of countdown
initialization:   
mov r2,  #9           ; mask set to enable outputting on pins 10 and 11
mov r6,  #73           
lsl r6,  #21          ; mask set to enable outputting on pins 7, 8 and 9
ldr r12, =0x20200004  ; loads pins 10-19
ldr r0,  =0x20200000  ; loads pins 0-9 
str r2,  [r12]        ; outputs 10 and 11 are now enabled
str r6,  [r0]         ; outputs 7, 8 and 9 are now enabled
mov r7,  #1            
lsl r7,  #7           ; mask set and ready to toggle pin 7 on and off
mov r8,  #1            
lsl r8,  #8           ; mask set and ready to toggle pin 8 on and off
mov r9,  #1            
lsl r9,  #9           ; mask set and ready to toggle pin 9 on and off
mov r10, #1            
lsl r10, #10          ; mask set and ready to toggle pin 10 on and off
mov r11, #1            
lsl r11, #11          ; mask set and ready to toggle pin 11 on and off
str r7,  [r0,#40]     ; clears pin 7
str r8,  [r0,#40]     ; clears pin 8
str r9,  [r0,#40]     ; clears pin 9
str r10, [r0,#40]     ; clears pin 10
str r11, [r0,#40]     ; clears pin 11
mov r6, r7            ; sets first pin to turn on
count:
mov r4,  r3
On:                   ; loop for about 1 second
sub r4,r4,#1
cmp r4,#1
bne On                 
str r6,  [r0,#28]     ; after a delay displays the current pin 
iteration1:           ; iterates the loop once for each of the four pins -
cmp r6, r7            ;-s in succession
bne iteration2
mov r6, r8
b count
iteration2:
cmp r6, r8
bne iteration3
mov r6, r9
b count
iteration3:
cmp r6, r9
bne iteration4
mov r6, r10
b count
iteration4:
cmp r6, r10
bne countdownend
mov r6, r11
b count 
countdownend:         ; after first four pins displayed.... 
flash:                ; blink all five pins at a rapid pace
mov r6, #30           ; sets the number of times to blink pins
reset:
mov r5,   #0          ; register to compare with counter
str r7,  [r0,#40]     ; clears pin 7
str r8,  [r0,#40]     ; clears pin 8
str r9,  [r0,#40]     ; clears pin 9
str r10, [r0,#40]     ; clears pin 10
str r11, [r0,#40]     ; clears pin 11
ldr r4,  =0x7FFFF     ; establishes counter for blink delay
waitOn:               ; loop for about 1 second
sub r4,r4,#1
cmp r4,#1
bne waitOn            ; proceeds when counter decremented to zero
str r7,  [r0,#28]     ; displays pin 7
str r8,  [r0,#28]     ; displays pin 8
str r9,  [r0,#28]     ; displays pin 9
str r10, [r0,#28]     ; displays pin 10
str r11, [r0,#28]     ; displays pin 11
ldr r4,  =0x7FFFF
waitOff:              ; loop for another second
sub r4,r4,#1
cmp r4,#1
bne waitOff           ; proceeds when counter decremented to zero
cmp r6, #0            
sub r6, r6, #1        ; decreases the remaining number of blinks
bne reset             ; reiterates blink if required otherwise resets pins
str r7,  [r0,#40]     ; clears pin 7 
str r8,  [r0,#40]     ; clears pin 8
str r9,  [r0,#40]     ; clears pin 9
str r10, [r0,#40]     ; clears pin 10
str r11, [r0,#40]     ; clears pin 11
andeq r0,r0,r0        ; halts program
