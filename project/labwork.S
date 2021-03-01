  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
  
  .global delay
  .global hexasc
  .global time2string
  .global enable_interrupt
  
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #

hexasc:
	andi $a0, $a0, 0xf
	ble $a0, 9, firstcase
	nop
	addi $a0, $a0, 0x37
	
	move $v0, $a0
	jr $ra
	nop

firstcase:
	addi $a0, $a0, 0x30
	
	move $v0, $a0
	jr $ra
	nop
	
delay:
	addi $t0, $0, 1
	addi $t1, $0, 0
while:
	sub $a0, $a0, $t0
	
for:
	addi $t1, $t1, 1
	# do nothing
	bne $t1, 6700, for
	nop
	
	addi $t1, $0, 0
	ble $t0, $a0, while
	nop
	
	jr $ra
	nop
	
time2string:
	PUSH $ra
	PUSH $a0
	sra $a0, $a1, 12
	jal hexasc
	nop
	andi $t0, $t0, 0x0
	POP $a0 
	
	sb $t0, 0($a0) 
	sb $t0, 1($a0) 
	sb $t0, 2($a0) 
	sb $t0, 3($a0)
	sb $t0, 4($a0)
	sb $t0, 5($a0)
	 
	sb $v0, 0($a0)
	
	PUSH $a0
	sra $a0, $a1, 8
	jal hexasc
	nop
	POP $a0
	sb $v0, 1($a0)
	
        # Colon ;
	addi $t4, $0, 0x3A
	sb $t4, 2($a0)
	
	PUSH $a0
	sra $a0, $a1, 4
	jal hexasc
	nop
	POP $a0 
	sb $v0, 3($a0)
	
	PUSH $a0 
	sra $a0, $a1, 0
	jal hexasc
	nop
	POP $a0
	sb $v0, 4($a0)
	
	# Null ;
	addi $t0, $0, 0x00
	sb $t0, 5($a0)
	
	POP $ra
	jr $ra
	nop
	
enable_interrupt:
	ei
	jr $ra
	nop