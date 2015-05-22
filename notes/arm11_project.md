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
	- Set PC register = *location 0
	- Execute instructions as per architecture.
- ARM system
	- Registers
		- 17 32-bit registers (5-bit Reg Code)
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

### Necessary functions of an emulator

- Execute the three-phase pipeline
- Set pipeline stages during branch: 

		if [[ decoded (starts with b) ]]; then 
			PC += offset 
			fetched = null 
		fi

- 

## Part II - Assembler



## Part III -

## Part IV -	
