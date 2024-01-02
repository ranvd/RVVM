# test vsetvl
# format: vsetvl rd, uimm, ex [,mx][,ta][,ma]

.text
.global _start
_start:
    vsetvli a0, x0, e8, m2, ta, ma