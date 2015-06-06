.pos 0x100

	# a[i] = a[i+1] + b[i+2]
	ld $a, r0			# r0 = address of a
	ld $i, r1			# r1 = address of i
	ld 0(r1) r1         # r1 = i
	inc r1				# r1 = i + 1
	ld (r0,r1,4), r1	# r1 = a[i+1]
	ld $b, r4			# r4 = address of b
	ld $i, r3			# r3 = address of i
	ld 0(r3) r3         # r3 = i
	ld $2, r5			# r5 = 2
	add r3, r5			# r5 = i + 2
	ld (r4,r5,4), r5	# r5 = b[i+2]
	add r3, r5			# r5 = a[i+1] + b[i+2]
	st r5, (r0,r3,4)	# a[i] = a[i+1] + b[i+2]

	# d[i] = a[i] + b[i]
	ld $i, r1			# r1 = address of i
	ld 0(r1) r1         # r1 = i
	ld (r0,r1,4) r3		# r3 = a[i]
	ld (r4,r1,4) r5		# r5 = b[i]
	add r5, r3			# r3 = a[i] + b[i]
	ld $d, r6			# r6 = address of d
	st r3, (r6,r1,4)	# d[i] = a[i] + b[i]

	# d[i] = a[b[i]] + b[a[i]]
	ld (r0,r1,4) r3		# r3 = a[i]
	ld (r4,r1,4) r5		# r5 = b[i]
	ld (r0,r5,4) r5		# r5 = a[b[i]]
	ld (r4,r3,4) r3		# r3 = b[a[i]]
	add r3, r5			# r5 = a[b[i]] + b[a[i]]
	st r5, (r6,r1,4)	# d[i] = a[b[i]] + b[a[i]]

	# d[b[i]] = b[a[i & 3] & 3] - a[b[i & 3] & 3] + d[i]
	ld $3, r5			# r5 = 3
	and r1, r5			# r5 = i & 3
	ld(r0,r5,4) r3		# r3 = a[i & 3]
	ld $3, r2			# r2 = 3
	and r2, r3			# r3 = a[i & 3] & 3
	ld(r4,r3,4) r3		# r3 = b[a[i & 3] & 3]
	ld(r4,r5,4) r5		# r5 = b[i & 3]
	and r2, r5			# r5 = b[i & 3] & 3
	ld(r0,r5,4) r5		# r5 = a[b[i & 3] & 3]
	not r5				# r5 = -(a[b[i & 3] & 3])
	ld $d, r6			# r6 = address of d
	ld (r6,r1,4) r7		# r7 = d[i]
	add r5, r3			# r3 = b[a[i & 3] & 3] - a[b[i & 3]
	add r7, r3			# r3 = b[a[i & 3] & 3] - a[b[i & 3] & 3] + d[i]
	ld (r4,r1,4) r4		# r4 = b[i]
	st r3, (r6,r4,4)	# d[b[i]] = b[a[i & 3] & 3] - a[b[i & 3] & 3] + d[i]








.pos 0x200

a:	.long 0 		# a[0]
	.long 0 		# a[1]
	.long 0 		# a[2]
	.long 0 		# a[3]
	.long 0 		# a[4]
	.long 0 		# a[5]
	.long 0 		# a[6]
	.long 0 		# a[7]
b:	.long 0 		# b[0]
	.long 0 		# b[1]
	.long 0 		# b[2]
	.long 0 		# b[3]
	.long 0 		# b[4]
	.long 0 		# b[5]
	.long 0 		# b[6]
	.long 0 		# b[7]
c:	.long 0 		# c[0]
	.long 0 		# c[1]
	.long 0 		# c[2]
	.long 0 		# c[3]
	.long 0 		# c[4]
	.long 0 		# c[5]
	.long 0 		# c[6]
	.long 0 		# c[7]
i: 	.long 0 		# i
d:	.long c			# d