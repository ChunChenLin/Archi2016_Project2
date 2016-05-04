addi  $1, $0, 0x7FFF

sll   $1, $1, 16

ori   $1, $1, 0xFFFF 

addi  $2, $0, 2      

add   $3, $1, $2     

addu  $4, $1, $2

addi  $5, $1, 0xF    

addiu $6, $1, 0xF
 
addi  $7, $0, 0xFFFF 

and   $1, $1, $2

andi  $2, $2, 0
         
add   $0, $0, $2     

lw $8 4($0) 
 
add $0 $0 $0

lw $9, 1025($0)

addi $9, $8, 1

addi $4, $zero, 2
	
addi $1, $1, 1	

beq $1, $4, 4

j 0x08	

addi $2, $zero, 1	

addi $3, $zero, 1

add $1, $2, $3	

add $2, $1, $3	

add $3, $2, $1		

add $1, $3, $3	

add $1, $2, $2	

add $2, $1, $1	

j 0x44				

j 0x60			

halt			

halt			

halt				

halt			

halt			

j 0x48				

j 0x48