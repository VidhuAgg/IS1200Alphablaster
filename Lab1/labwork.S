  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
  
.global delay
.global time2string


.macro  PUSH reg
      addi  $sp,$sp,-4
      sw    \reg,0($sp)
.endm

.macro  POP reg
      lw    \reg,0($sp)
      addi  $sp,$sp,4
.endm

.data
.align 2
.text
  #
  # Please add your own code below this line
  #
hexasc:
	andi $t0,$a0,0xf
	
   	ble $t0,9,numZeroToNine
   	nop
   	
   	ble $t0,15,alphaAtoF
   	nop
 
numZeroToNine:
	addi $v0,$t0,0x30
	jr $ra
	nop
alphaAtoF:
	addi $v0,$t0,0x37    #0x41 is the ASCII value of A but we subtract 10 and add $t0 to 			#get the value 
	jr $ra
	nop

delay:
	li 	$t1,3750		#assigning constant value
	move 	$t2,$a0		#parameter ms value to t2
		while:
		ble 	$t2,0,donewhile	#for exiting the while loop, if ms<=0
		sub 	$t2,$t2,1	# ms = ms - 1
		li 	$t0,0		#initialzing i = 0
	
		for:
			bge	$t0, $t1, while	#branches if i becomes 						#equal or greater than 4711
			nop
			addi 	$t0,$t0,1		#i = i + 1 
			
			j 	for
			nop
						
		j 	while
		nop
	
	donewhile:			#exits the while loop
		jr 	$ra
		nop

time2string:

	PUSH $ra			#pushing the registers we're gonna use 					#in the stack
	PUSH $s0
	PUSH $s1

	move 	$s0,$a0 		#moving the data from a0 to s0 to manipulate and 				#preserve the original value
	move 	$s1,$a1 		#same with a1, moving to s1

	andi	$t0,$s1,0x00ff
	blez	$t0,printDing
	nop
	
	andi 	$t0,$s1,0xf000 		#getting bits from 12-16th position
	srl 	$a0,$t0,12 		#moving the bits by 12 position and storing value in 				#a0
	jal 	hexasc			#calling hexasc
	nop
	
	move	 $t1,$v0		#copying value recieved from hexasc to t1	
	sb 	$t1,0($s0)		#storing the  byte at 1st position of s0
	
	andi 	$t0,$s1,0xf00		#getting bits from 8-12th position
	srl 	$a0,$t0,8 		#moving bits by 8 position and storing in a0
	jal 	hexasc			#caling hexasc
	nop
	
	move 	$t1,$v0			#moving value recieved from hexasc to 					#t1
	sb 	$t1,1($s0)		#storing the  byte at 2nd position of s0
	
	li 	$t1,0x3a		#loading the immediate value of 0x3a ":" to t1
	sb 	$t1,2($s0)		#storing the  byte at 3rd position of s0
	
	andi 	$t0,$s1,0xf0 		#getting bits from 4-8th position 				#==========LINES USED FOR 5 IN 16:53=====
	srl 	$a0,$t0,4 		#moving bits by 4 position and storing in a0
	jal 	hexasc
	nop
	
	move 	$t1,$v0			#moving value recieved from hexasc to 					#t1
	sb 	$t1,3($s0)		#storing the  byte at 4th position of s0
	
	andi 	$a0,$s1,0xf 		#getting bits from 1-4th position
	jal 	hexasc
	nop
	
	move 	$t1,$v0			#moving value recieved from hexasc to 					#t1
	sb 	$t1,4($s0)		#storing the  byte at 5th position of s0
	
	li 	$t1,0x00		#loading the immediate value of 0x00 "Null" to t1, 				#determines end of the string
	sb 	$t1,5($s0) 		#storing the  byte at 6th position of s0

	POP $s1			#popping the registers
	POP $s0
	POP $ra
	jr $ra				#returning back
	nop

printDing:
	li 	$t1,0x44		
	sb 	$t1,0($s0)		
	
	li 	$t1,0x49		
	sb 	$t1,1($s0)		
	
	li 	$t1,0x4e		
	sb 	$t1,2($s0)		

	li 	$t1,0x47		
	sb 	$t1,3($s0)		
	
	li 	$t1,0x0			
	sb 	$t1,4($s0)		
		
	jr	$ra
	nop


	

