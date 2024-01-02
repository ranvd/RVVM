# test vsetvli
# format:
#     vsetvl rd, rs1, ex [,mx][,ta][,ma]
#  e8    # SEW=8b
#  e16   # SEW=16b
#  e32   # SEW=32b
#  e64   # SEW=64b

#  mf8  # LMUL=1/8
#  mf4  # LMUL=1/4
#  mf2  # LMUL=1/2
#  m1   # LMUL=1, assumed if m setting absent
#  m2   # LMUL=2
#  m4   # LMUL=4
#  m8   # LMUL=8

# Examples:
#     vsetvli t0, a0, e8, ta, ma          # SEW= 8, LMUL=1
#     vsetvli t0, a0, e8, m2, ta, ma      # SEW= 8, LMUL=2
#     vsetvli t0, a0, e32, mf2, ta, ma    # SEW=32, LMUL=1/2

.text
.global _start
_start:
    vsetvli a0, x0, e8, m2, ta, ma