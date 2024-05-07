# arrayFunction.asm
       .data 
array: .word 8, 2, 1, 6, 9, 7, 3, 5, 0, 4
newl:  .asciiz "\n"

       .text
main:
	# Print the original content of array
	la   $a0, array   # setup the parameter(s)
  la   $a1, newl
  sub  $a1, $a1, $a0
  srl  $a1, $a1, 2
	jal  printArray   # call the printArray function

	# Ask the user for two indices
	li   $v0, 5         	# System call code for read_int
	syscall           
	addi $t0, $v0, 0    	# first user input in $t0
 
	li   $v0, 5         	# System call code for read_int
	syscall           
	addi $t1, $v0, 0    	# second user input in $t1

	# Call the findMin function
	sll  $t0, $t0, 2      # setup the parameter(s)
	sll  $t1, $t1, 2  
  la   $a0, array     
  la   $a1, array
  add  $a0, $a0, $t0
  add  $a1, $a1, $t1
  addi  $a1, $a1, 4
	jal  findMin        # call the function

	# Print the min item
	or $t4, $v0, $zero # place the min address at $t4
	lw $t3, 0($v0) # place the min item in $t3 for printing

	# Print an integer followed by a newline
	li   $v0, 1   		# system call code for print_int
    addi $a0, $t3, 0    # print $t3
    syscall       		# make system call

	li   $v0, 4   		# system call code for print_string
    la   $a0, newl    	# 
    syscall       		# print newline

	#Calculate and print the index of min item
	# Place the min index in $t3 for printing	
  or  $t3, $t4, 0	
  la  $t4, array
  sub $t3, $t3, $t4
  srl $t3, $t3, 2

	# Print the min index
	# Print an integer followed by a newline
	li   $v0, 1   		# system call code for print_int
    addi $a0, $t3, 0    # print $t3
    syscall       		# make system call
	
	li   $v0, 4   		# system call code for print_string
    la   $a0, newl    	# 
    syscall       		# print newline
	
	# End of main, make a syscall to "exit"
	li   $v0, 10   		# system call code for exit
	syscall	       		# terminate program
	

#######################################################################
###   Function printArray   ### 
#Input: Array Address in $a0, Number of elements in $a1
#Output: None
#Purpose: Print array elements
#Registers used: $t0, $t1, $t2, $t3
#Assumption: Array element is word size (4-byte)
printArray:
	addi $t1, $a0, 0	# $t1 is the pointer to the item
	sll  $t2, $a1, 2	# $t2 is the offset beyond the last item
	add  $t2, $a0, $t2 	# $t2 is pointing beyond the last item
l1:	
	beq  $t1, $t2, e1
	lw   $t3, 0($t1)	# $t3 is the current item
	li   $v0, 1   		# system call code for print_int
    addi $a0, $t3, 0   	# integer to print
    syscall       		# print it
	addi $t1, $t1, 4
	j    l1				# Another iteration
e1:
	li   $v0, 4   		# system call code for print_string
    la   $a0, newl    	# 
    syscall       		# print newline
	jr   $ra			# return from this function


#######################################################################
###   Student Function findMin   ### 
#Input: Lower Array Pointer in $a0, Higher Array Pointer in $a1
#Output: $v0 contains the address of min item 
#Purpose: Find and return the minimum item 
#              between $a0 and $a1 (inclusive)
#Registers used: $t0, $t1, $t2, $t3, $t4, $t5
#Assumption: Array element is word size (4-byte), $a0 <= $a1
findMin:
  or  $t0, $a0, $zero # t0 is the pointer to the item
  or  $t1, $a1, $zero # t1 is the pointer beyond last item
  or  $t2, $t0, $zero # initialize minIndex=$t2 to t0
  lw  $t3, 0($t0)     # initialize min=$t3 to first element
loop:
  addi $t0, $t0, 4               
  beq  $t0, $t1, end

  lw   $t4, 0($t0)
  slt  $t5, $t3, $t4
  bne  $t5, $zero, finishLoop
  or   $t2, $t0, $t0  
  or   $t3, $t4, $t4
finishLoop:
  j     loop    # next iter
end:
  or $v0, $t2, $zero
	jr $ra			# return from this function
