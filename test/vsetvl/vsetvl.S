# test vsetvl
# format: vsetvl rd, rs1, rs2

.text
.global _start
_start:
    vsetvli a0, x0, e8, m2, ta, ma