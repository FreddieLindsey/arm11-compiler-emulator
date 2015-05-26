defa:
defb:  ; comment
mov r0,#1 ;0   ; instruction with comment
bne def1  ;1  ; comment with : colon and ; another comment on the same line
             ; line with only a comment, followed by an empty line

mov r0,#2 ;2
defc:
bne def2  ;3
mov r3,r1 ;4


defd:
bne defc  ;5 
;this is the end for now

