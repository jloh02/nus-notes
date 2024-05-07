# arrayCount.asm
  .data 
arrayA: .word 1, 1, 1, 1, 1, 1, 1, 1
count:  .word 0

  .text
main:
    la $t0, arrayA        #load arrayA addr into t0

    ori $t1, $zero, 8     #array storing counter
    countloop:
      addi $t1, $t1, -1
      li   $v0, 5    # system call code for read_int
      syscall        # read the integer to v0
      sw $v0, 0($t0)
      addi $t0, $t0, 4
      beq $t1, $zero, countexit
      j countloop
    countexit:

    # code to setup the variable mappings
    la $t0, arrayA
    la $t7, count
    lw $t8, 0($t7)

    # code for reading in the user value X
    li   $v0, 5             # system call code for read_int
    syscall                 # read the integer to v0
    or   $t5, $v0, $zero    # Save X to t5

    # code for counting multiples of X in arrayA
    addi $t5, $t5, -1        # Create bitmask
    loop:
      lw $t3, 0($t0)          # Read arrayA value into t3
      and $t4, $t3, $t5       # Write remainder to t4
      bne $t4, $zero, endloop # If remainder zero, continue to add 1 to counter
      addi $t8, $t8, 1
    endloop:
      addi $t0, $t0, 4      # Increment pointer
      beq $t0, $t7, exit
      j loop
    exit:

    sw $t8, 0($t7)

    # code for printing result
    li $v0, 1          # system call code for print_int
    or $a0, $t8, $zero # copy read int to a0 for printing
    syscall            # print the integer

    # code for terminating program
    li  $v0, 10
    syscall
