ldr r3, =0xFFFFFF       ; r3 register will set length of countdown  
mov r5, #0              ; r5 register will set the blink mode 

userinputsetter:

  setcountdownlength:

    cmp r3, #0            ; if user has defined length of countdown....
    ble defaultcountdown  ; invalid or unset length uses a default
    bne setstartmode      ; use that number
    
      defaultcountdown:

        ldr r3, =0xFFFFFF      ; else use default length of countdown

  setstartmode:

    cmp r5, #3            
    bge defaultstart      ; if user enters invalid blink mode (>2) set a default
    cmp r5, #0
    blt defaultstart      ; if user enters invalid blink mode (<0) set a default
    b userinputset        ; jumps if valid blink mode selected (0, 1 or 2)

    defaultstart:
  
      mov r5, #0            ; sets a default blink mode of 0

userinputset:

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

pinsetup:

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

countdownend:         ; after five pins displayed.... 

choosestartmode:

  cmp r5, #0            
  beq flash             ; if blink mode is 0 flashing start
  cmp r5, #1           
  beq stayon            ; if blink mode is 1 constant pin displayed start
  cmp r5, #2           
  beq ripple            ; if blink mode is 2 ripple start

stayon:

  mov r6,  #60          ; sets the length of times to display pins

  keepdisplayed:

    ldr r4,  =0x7FFFF     ; establishes counter for display delay
  
    displayon:               ; loop for another second

      sub r4,r4,#1
      cmp r4,#1
      bne displayon            ; proceeds when counter decremented to zero

    cmp r6, #0            
    sub r6, r6, #1        ; decreases the remaining number of blinks
    bne keepdisplayed     ; reiterates blink if required otherwise resets pins

  b closeprogram        ; ends the program

ripple:

  str r7,  [r0,#40]     ; clears pin 7
  str r8,  [r0,#40]     ; clears pin 8
  str r9,  [r0,#40]     ; clears pin 9
  str r10, [r0,#40]     ; clears pin 10
  str r11, [r0,#40]     ; clears pin 11
  mov r3, #10

singleripple:           ; ripples through all five pins once
 
  mov r6,  r7

  ripplepin:            ; toggles on and off a single pin
    ldr r4, =0x7FFFF
    str r6,  [r0,#28]     ; displays the current pin 

  rippleon:             ; loops for a short time

    sub r4,r4,#1
    cmp r4,#1
    bne rippleon 
                
  str r6,  [r0,#40]     ; after a short delay clears the current pin 

  ripple1:           ; iterates the loop once for each of the five pins -

    cmp r6, r7            ;-s in succession
    bne ripple2
    mov r6, r8
    b ripplepin

  ripple2:

    cmp r6, r8
    bne ripple3
    mov r6, r9
    b ripplepin

  ripple3:

    cmp r6, r9
    bne ripple4
    mov r6, r10
    b ripplepin

  ripple4:

    cmp r6, r10
    bne rippleend
    mov r6, r11
    b ripplepin
  
  rippleend:

    cmp r3, #0            
    sub r3, r3, #1        ; decreases the remaining number of ripples
    bne singleripple      ; reiterates ripple if required or end program
    b closeprogram
 
countdownend:         ; after five pins displayed.... 
  

flash:                ; blink all five pins at a rapid pace

  mov r6, #30           ; sets the number of times to blink pins

  reset:

    str r7,  [r0,#40]     ; clears pin 7
    str r8,  [r0,#40]     ; clears pin 8
    str r9,  [r0,#40]     ; clears pin 9
    str r10, [r0,#40]     ; clears pin 10
    str r11, [r0,#40]     ; clears pin 11
    ldr r4,  =0x7FFFF     ; establishes counter for blink delay

    waitoff:              ; loop for about 1 second

      sub r4,r4,#1
      cmp r4,#1
      bne waitoff           ; proceeds when counter decremented to zero

    str r7,  [r0,#28]     ; displays pin 7
    str r8,  [r0,#28]     ; displays pin 8
    str r9,  [r0,#28]     ; displays pin 9
    str r10, [r0,#28]     ; displays pin 10
    str r11, [r0,#28]     ; displays pin 11
    ldr r4,  =0x7FFFF     ; resets the counter

    waiton:               ; loop for another second

      sub r4,r4,#1
      cmp r4,#1
      bne waiton            ; proceeds when counter decremented to zero

    cmp r6, #0            
    sub r6, r6, #1        ; decreases the remaining number of blinks
    bne reset             ; reiterates blink if required otherwise resets pins

closeprogram:

  str r7,  [r0,#40]     ; clears pin 7 
  str r8,  [r0,#40]     ; clears pin 8
  str r9,  [r0,#40]     ; clears pin 9
  str r10, [r0,#40]     ; clears pin 10
  str r11, [r0,#40]     ; clears pin 11
  andeq r0,r0,r0        ; halts program
