.equ STACK_SIZE, 2048

.global _start
.text
_start:
	li x1, 10
	li x2, 20
	# la sp, stacks + STACK_SIZE
	# j start_kernel

	
stacks:
	.skip STACK_SIZE