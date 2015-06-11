mov r2,#1
lsl r2,#18           ; mask for pin 16
ldr r0,=0x20200004   ; load pins 10-19
str r2,[r0]          ; sets up pin 16 as an output

mov r1,#1
lsl r1,#16           ; mask for pin 16

reset:

  mov r5,#0          ; store counter
  str r1,[r0,#36]    ; clear pin 16

  ldr r3, =0xFFFFFF

  waitOn:            ; loop for about 1 second
    sub r3,r3,#1
    cmp r3,#1
  bne waitOn

  mov r5,#0          ; resets counter
  str r1,[r0,#24]    ; set pin 16
  ldr r4, =0xFFFFFF

  waitOff:           ; loop for another second
    sub r4,r4,#1
    cmp r4,#1
  bne waitOff

b reset            ; jump to reset
