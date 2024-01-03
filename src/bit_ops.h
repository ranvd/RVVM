/*
bit_ops.h - Bit operations
Copyright (C) 2021  LekKit <github.com/LekKit>

This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at https://mozilla.org/MPL/2.0/.

Alternatively, the contents of this file may be used under the terms
of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or any later version.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef RVVM_BIT_OPS_H
#define RVVM_BIT_OPS_H

#include "rvvm_types.h"

// Simple bit operations (sign-extend, etc) for internal usage

/*
* Sign-extend bits in the lower part of val into signed long
* usage:
*     long ext = sign_extend(val, 20);
*
*     [ext is now equal to signed lower 20 bits of val]
*/
static inline int64_t sign_extend(uint64_t val, bitcnt_t bits)
{
    return ((int64_t)(val << (64 - bits))) >> (64 - bits);
}

// Generate bitmask of given size
static inline uint64_t bit_mask(bitcnt_t count)
{
    return (1ULL << count) - 1;
}

// Cut bits from val at given position (from lower bit)
static inline uint64_t bit_cut(uint64_t val, bitcnt_t pos, bitcnt_t bits)
{
    return (val >> pos) & bit_mask(bits);
}

// Replace bits in val at given position (from lower bit) by rep
static inline uint64_t bit_replace(uint64_t val, bitcnt_t pos, bitcnt_t bits, uint64_t rep)
{
    return (val & (~(bit_mask(bits) << pos))) | ((rep & bit_mask(bits)) << pos);
}

// Check if Nth bit of val is 1
static inline bool bit_check(uint64_t val, bitcnt_t pos)
{
    return (val >> pos) & 0x1;
}

static inline uint32_t bit_clz(uint64_t val){
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    x |= (x >> 32);
	
    /* count ones (population count) */
    x -= ((x >> 1) & 0x5555555555555555);
    x = ((x >> 2) & 0x3333333333333333) + (x & 0x3333333333333333);
    x = ((x >> 4) + x) & 0x0f0f0f0f0f0f0f0f;
    x += (x >> 8);
    x += (x >> 16);
    x += (x >> 32);
    
    return (64 - (x & 0x7f));
}

static inline uint32_t bit_log2(uint64_t val){
    return bit_clz(val);
}

// Normalize the value to nearest next power of two
static inline uint64_t bit_next_pow2(uint64_t val)
{
    // Fast path for proper pow2 values
    if (!(val & (val - 1))) return val;
    // Bit twiddling hacks
    val -= 1;
    val |= (val >> 1);
    val |= (val >> 2);
    val |= (val >> 4);
    val |= (val >> 8);
    val |= (val >> 16);
    val |= (val >> 32);
    return val + 1;
}

static inline uint32_t bit_rotl32(uint32_t val, bitcnt_t bits)
{
    return (val << bits) | (val >> (32 - bits));
}

static inline uint64_t bit_rotl64(uint64_t val, bitcnt_t bits)
{
    return (val << bits) | (val >> (64 - bits));
}

// Reverse bits in val (from lower bit), remaining bits are zero
static inline uint64_t bit_reverse(uint64_t val, bitcnt_t bits)
{
    uint64_t ret = 0;

    for (bitcnt_t i=0; i<bits; ++i) {
        ret <<= 1;
        ret |= val & 0x1;
        val >>= 1;
    }

    return ret;
}

// Bswap 32-bit value (From BE to LE or vice versa)
static inline uint32_t byteswap_uint32(uint32_t val)
{
    return (((val & 0xFF000000) >> 24) |
            ((val & 0x00FF0000) >> 8)  |
            ((val & 0x0000FF00) << 8)  |
            ((val & 0x000000FF) << 24));
}

// Bswap 64-bit value (From BE to LE or vice versa)
static inline uint64_t byteswap_uint64(uint64_t val)
{
    return (((val & 0xFF00000000000000) >> 56) |
            ((val & 0x00FF000000000000) >> 40) |
            ((val & 0x0000FF0000000000) >> 24) |
            ((val & 0x000000FF00000000) >> 8)  |
            ((val & 0x00000000FF000000) << 8)  |
            ((val & 0x0000000000FF0000) << 24) |
            ((val & 0x000000000000FF00) << 40) |
            ((val & 0x00000000000000FF) << 56));
}

// Get high 64 bits from signed i64 x i64 -> 128 bit multiplication
static inline uint64_t mulh_uint64(int64_t a, int64_t b)
{
#ifdef INT128_SUPPORT
    return ((int128_t)a * (int128_t)b) >> 64;
#else
    int64_t lo_lo = (a & 0xFFFFFFFF) * (b & 0xFFFFFFFF);
    int64_t hi_lo = (a >> 32)        * (b & 0xFFFFFFFF);
    int64_t lo_hi = (a & 0xFFFFFFFF) * (b >> 32);
    int64_t hi_hi = (a >> 32)        * (b >> 32);
    int64_t cross = (lo_lo >> 32) + (hi_lo & 0xFFFFFFFF) + lo_hi;
    return (hi_lo >> 32) + (cross >> 32) + hi_hi;
#endif
}

// Get high 64 bits from unsigned u64 x u64 -> 128 bit multiplication
static inline uint64_t mulhu_uint64(uint64_t a, uint64_t b)
{
#ifdef INT128_SUPPORT
    return ((uint128_t)a * (uint128_t)b) >> 64;
#else
    uint64_t lo_lo = (a & 0xFFFFFFFF) * (b & 0xFFFFFFFF);
    uint64_t hi_lo = (a >> 32)        * (b & 0xFFFFFFFF);
    uint64_t lo_hi = (a & 0xFFFFFFFF) * (b >> 32);
    uint64_t hi_hi = (a >> 32)        * (b >> 32);
    uint64_t cross = (lo_lo >> 32) + (hi_lo & 0xFFFFFFFF) + lo_hi;
    return (hi_lo >> 32) + (cross >> 32) + hi_hi;
#endif
}

// Get high 64 bits from signed * unsigned i64 x u64 -> 128 bit multiplication
static inline uint64_t mulhsu_uint64(int64_t a, uint64_t b)
{
#ifdef INT128_SUPPORT
    return ((int128_t)a * (uint128_t)b) >> 64;
#else
    int64_t lo_lo = (a & 0xFFFFFFFF) * (b & 0xFFFFFFFF);
    int64_t hi_lo = (a >> 32)        * (b & 0xFFFFFFFF);
    int64_t lo_hi = (a & 0xFFFFFFFF) * (b >> 32);
    int64_t hi_hi = (a >> 32)        * (b >> 32);
    uint64_t cross = (lo_lo >> 32) + (hi_lo & 0xFFFFFFFF) + lo_hi;
    return (hi_lo >> 32) + (cross >> 32) + hi_hi;
#endif
}

#endif
