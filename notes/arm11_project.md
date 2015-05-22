# ARM11 Project

## Part I - Emulator

- Read an ARM binary file of object code
	- Object code consists of 32-bit 'words' that should be loaded into a C data structure
- Instructions
	- All instructions are 32-bits (4 bytes)
	- ARM memory is byte addressable, but the instructions are aligned on a 4-byte boundary, ensuring all instructions are in addresses that are multiples of 4.
	- Instructions may modify ARM memory and general purpose registers
	- A [branch instruction](http://www.johnloomis.org/NiosII/isa/isa4.html) may change the PC to a target address.
- Intialisation
	- All registers and memory locations should be initialised to 0.
- Running the program
	- Binary file loads
	- Initialise the processor/memory
	- Execute instructions as per architecture.
- ARM system
	- Registers
		- 17 32-bit registers (4-bit Reg Code - 17 not accessible)
		- Reg 0 - 12 are general purpose
		- Reg 13 - 14 are called SP, RP respectively and can be ignored for this exercise
		- Reg 15 is the PC register
		- Reg 16 is the CPSR register
			- Configures operating mode for ARM processor and checks conditionals
			- We are only concerned with it's top 4-bits. 
	
	| Flag bit | Flag | Semantic |
	| :-- | :-- | --- |
	| 31 | N | last result was negative |
	| 30 | Z | last result was zero |
	| 29 | C | carry bit |
	| 28 | V | overflow bit |
	
	- Instruction set
		- No `halt` instruction in ARM instruction set --> the processor will run forever
		- We will use an instruction `all-0` as a psuedo-halt instruction (corresponds to `andeq r0, r0, r0`)
		- Upon terminating, we will print the following
			- The value of each register
			- The contents of any non-zero memory location

### Functions of an emulator class

#### Program functions

- Execute the three-phase pipeline
- Set pipeline stages during branch: 

		if [[ decoded (starts with b) ]]; then 
			PC += offset 	; offset is calculated from the position 
							; of the instruction relative to PC + 8
			fetched = null 
		fi

- Check conditions in the instruction with the CPSR register

		if [[ condition_met || al ]]; then
			run instruction
		else
			pass
		fi

| Code | Suffix | CPSR flags | Interpretation |
| --- | --: | :-- | :-- |
| 0000 | eq | Z set | equal |
| 0001 | ne | Z clear | not equal |
| 1010 | ge | N equals V | greater or equal |
| 1011 | lt | N not equals to V | less than |
| 1100 | gt | Z clear AND (N equals V) | greater than |
| 1101 | le | Z set OR (N not equal to V) | less than or equal |
| 1110 | al | (ignored) | - |

#### Data processing instructions

Form

| | Bit from | Bit to | Semantic |
| :-- | --- | --- | :-- |
| *Cond* | 28 | 31 | Condition field |
| - | 26 | 27 | 0 |
| *Immediate Operand* | 25 | 25 | If set, *Operand2* is an immediate constant, otherwise it is a shifted register |
| *OpCode* | 21 | 24 | Minimal set of supported *OpCodes* details below. |
| *Set conditions* | 20 | 20 | If set, instruction should update CPSR flags |
| \\( R_{n} \\) | 16 | 19 | First operand is content of register \\( R_{n} \\) |
| \\( R_{d} \\) | 12 | 15 | Register to write result to \\( R_{d} \\) |
| *Operand2* | 0 | 11 | Either a rotated 8 bit *Immediate constant* or a shifted register (\\( R_{m} \\)). <br> Details on interpretation below. |

OpCodes

| OpCode | Mnemonic | Interpretation |
| --- | --- | :-- |
| 0000 | and | \\( R_{n} \\) AND *Operand2* |
| 0001 | eor | \\( R_{n} \\) EOR *Operand2* |
| 0010 | sub | \\( R_{n} \\) - *Operand2* |
| 0011 | rsb |  *Operand2* - \\( R_{n} \\) |
| 0100 | add | \\( R_{n} \\) + *Operand2* |
| 1000 | tst | as `and` but result not written |
| 1001 | teq | as `eor` but result not written |
| 1010 | cmp | as `sub` but result not written |
| 1100 | orr | \\( R_{n} \\) OR *Operand2* |
| 1101 | mov | *Operand2* |

*Operand2*

	Immediate constant

If *Operand2* is an immediate constant (I = 1), then bits 0 --> 7 are an unsigned 8-bit immediate value, and bits 8 --> 11 are the rotation.

Such that, the constants value is the 8-bit value, extended to 32-bit <br>(00000001 -> 00000001000000000000000000000000), <br> which is then rotated right an even number of positions as specified by the rotation bits (0, 2, 4, . . , 30)

Therefore, 

<center>
010100100110 <br> --> <br> 00000000000010011000000000000000
</center>

Note that is it not possible to load a 32-bit immediate value into a register; this must be done using load from memory.

	Register

If *Operand2* is another register (I = 0), then bits 0 --> 3 are the register, and bits 4 --> 11 specify the shift.
The shift has a type (2-bit) and is defined by a 5-bit integer, or by another register. 

If the shift is by an integer,

- Bit 4 is 0
- Bits 5 --> 6 give the shift type
- Bits 7 --> 11 give the 5-bit integer shift amount

If the shift if by a register value,

- Bit 4 is 1
- Bits 5 -- > 6 give the shift type
- Bit 7 is 0 (unused)
- Bits 8 --> 11 are the shift register where the last byte of the register value gives the shift amount

Shift types

| Code | Shift type | Interpretation |
| --- | :-- | :-- |
| 00 | logical left (lsl) | Moves each bit to the left by the specified amount |
| 01 | logical right (lsr) | Moves each bit to the right by the specified amount |
| 10 | arithmetic right (asr) | As lsr, but preserves the sign bit; used for 2's compliment operations. |
| 11 | rotate right (ror) | Rotate cyclically with bit 0 shifting into bit 31 |

Note that for logical left, if the instruction is to set the CPSR flags, the least significant discarded bit is the new carry flag. For logical right, this is the most significant discarded bit.
For arithmetic right, instead of filling the high bits with 0s, they are filled with bit 31 of the original value.

#### Multiply instructions

Form

| | Bit from | Bit to | Semantic |
| :-- | --- | --- | :-- |
| *Cond* | 28 | 31 | Condition field |
| - | 22 | 27 | 0 |
| *Accumulate* | 21 | 22 | If set, the instruction performs multiply and accumulate, otherwise just multiply. |
| *Set conditions* | 20 | 20 | If set, instruction should update CPSR flags |
| \\( R_{d} \\) | 16 | 19 | Destination register |
| \\( R_{n} \\) | 12 | 15 | \\( R_{n} \\) |
| \\( R_{s} \\) | 8 | 11 | \\( R_{s} \\) |
| - | 4 | 7 | 1001 |
| \\( R_{m} \\) | 0 | 3 | \\( R_{m} \\) |

Accumulate

	if [[ accumulate == 1 ]]; then
		R_d := R_m * R_s + R_n
	else
		R_d := R_m * R_s
	fi

The multiply instruction (with or without accumulate) truncates to the least significant 32-bits, and as such, it works the same for signed and unsigned integers.

#### Single Data Transfer

## Part II - Assembler



## Part III -

## Part IV -	
