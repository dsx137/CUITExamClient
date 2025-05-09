// cham_simd.cpp - written and placed in the public domain by Jeffrey Walton
//
//    This source file uses intrinsics and built-ins to gain access to
//    SSSE3, ARM NEON and ARMv8a, and Power7 Altivec instructions. A separate
//    source file is needed because additional CXXFLAGS are required to enable
//    the appropriate instructions sets in some build configurations.

#include "pch.h"
#include "config.h"

#include "cham.h"
#include "misc.h"

// Uncomment for benchmarking C++ against SSE or NEON.
// Do so in both simon.cpp and simon_simd.cpp.
// #undef CRYPTOPP_SSSE3_AVAILABLE
// #undef CRYPTOPP_ARM_NEON_AVAILABLE

#if (CRYPTOPP_SSSE3_AVAILABLE)
#include "adv_simd.h"
# include <pmmintrin.h>
# include <tmmintrin.h>
#endif

#if defined(__XOP__)
# include <ammintrin.h>
# if defined(__GNUC__)
#  include <x86intrin.h>
# endif
#endif

// Clang intrinsic casts, http://bugs.llvm.org/show_bug.cgi?id=20670
#define DOUBLE_CAST(x) ((double*)(void*)(x))
#define CONST_DOUBLE_CAST(x) ((const double*)(const void*)(x))

// Squash MS LNK4221 and libtool warnings
extern const char CHAM_SIMD_FNAME[] = __FILE__;

ANONYMOUS_NAMESPACE_BEGIN

using CryptoPP::word16;
using CryptoPP::word32;

#if (CRYPTOPP_SSSE3_AVAILABLE)

//////////////////////////////////////////////////////////////////////////

NAMESPACE_BEGIN(W32)  // CHAM128, 32-bit word size

template <unsigned int R>
inline __m128i RotateLeft32(const __m128i& val)
{
#if defined(__XOP__)
	return _mm_roti_epi32(val, R);
#else
	return _mm_or_si128(
		_mm_slli_epi32(val, R), _mm_srli_epi32(val, 32 - R));
#endif
}

template <unsigned int R>
inline __m128i RotateRight32(const __m128i& val)
{
#if defined(__XOP__)
	return _mm_roti_epi32(val, 32 - R);
#else
	return _mm_or_si128(
		_mm_slli_epi32(val, 32 - R), _mm_srli_epi32(val, R));
#endif
}

// Faster than two Shifts and an Or. Thanks to Louis Wingers and Bryan Weeks.
template <>
inline __m128i RotateLeft32<8>(const __m128i& val)
{
#if defined(__XOP__)
	return _mm_roti_epi32(val, 8);
#else
	const __m128i mask = _mm_set_epi8(14, 13, 12, 15, 10, 9, 8, 11, 6, 5, 4, 7, 2, 1, 0, 3);
	return _mm_shuffle_epi8(val, mask);
#endif
}

// Faster than two Shifts and an Or. Thanks to Louis Wingers and Bryan Weeks.
template <>
inline __m128i RotateRight32<8>(const __m128i& val)
{
#if defined(__XOP__)
	return _mm_roti_epi32(val, 32 - 8);
#else
	const __m128i mask = _mm_set_epi8(12, 15, 14, 13, 8, 11, 10, 9, 4, 7, 6, 5, 0, 3, 2, 1);
	return _mm_shuffle_epi8(val, mask);
#endif
}

template <unsigned int IDX>
inline __m128i UnpackXMM(const __m128i& a, const __m128i& b, const __m128i& c, const __m128i& d)
{
	// Should not be instantiated
	CRYPTOPP_UNUSED(a); CRYPTOPP_UNUSED(b);
	CRYPTOPP_UNUSED(c); CRYPTOPP_UNUSED(d);
	CRYPTOPP_ASSERT(0);
	return _mm_setzero_si128();
}

template <>
inline __m128i UnpackXMM<0>(const __m128i& a, const __m128i& b, const __m128i& c, const __m128i& d)
{
	// The shuffle converts to and from little-endian for SSE. A specialized
	// CHAM implementation can avoid the shuffle by framing the data for
	// encryption, decryption and benchmarks. The library cannot take the
	// speed-up because of the byte oriented API.
	const __m128i r1 = _mm_unpacklo_epi32(a, b);
	const __m128i r2 = _mm_unpacklo_epi32(c, d);
	return _mm_shuffle_epi8(_mm_unpacklo_epi64(r1, r2),
		_mm_set_epi8(12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3));
}

template <>
inline __m128i UnpackXMM<1>(const __m128i& a, const __m128i& b, const __m128i& c, const __m128i& d)
{
	// The shuffle converts to and from little-endian for SSE. A specialized
	// CHAM implementation can avoid the shuffle by framing the data for
	// encryption, decryption and benchmarks. The library cannot take the
	// speed-up because of the byte oriented API.
	const __m128i r1 = _mm_unpacklo_epi32(a, b);
	const __m128i r2 = _mm_unpacklo_epi32(c, d);
	return _mm_shuffle_epi8(_mm_unpackhi_epi64(r1, r2),
		_mm_set_epi8(12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3));
}

template <>
inline __m128i UnpackXMM<2>(const __m128i& a, const __m128i& b, const __m128i& c, const __m128i& d)
{
	// The shuffle converts to and from little-endian for SSE. A specialized
	// CHAM implementation can avoid the shuffle by framing the data for
	// encryption, decryption and benchmarks. The library cannot take the
	// speed-up because of the byte oriented API.
	const __m128i r1 = _mm_unpackhi_epi32(a, b);
	const __m128i r2 = _mm_unpackhi_epi32(c, d);
	return _mm_shuffle_epi8(_mm_unpacklo_epi64(r1, r2),
		_mm_set_epi8(12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3));
}

template <>
inline __m128i UnpackXMM<3>(const __m128i& a, const __m128i& b, const __m128i& c, const __m128i& d)
{
	// The shuffle converts to and from little-endian for SSE. A specialized
	// CHAM implementation can avoid the shuffle by framing the data for
	// encryption, decryption and benchmarks. The library cannot take the
	// speed-up because of the byte oriented API.
	const __m128i r1 = _mm_unpackhi_epi32(a, b);
	const __m128i r2 = _mm_unpackhi_epi32(c, d);
	return _mm_shuffle_epi8(_mm_unpackhi_epi64(r1, r2),
		_mm_set_epi8(12, 13, 14, 15, 8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3));
}

template <unsigned int IDX>
inline __m128i UnpackXMM(const __m128i& v)
{
	// Should not be instantiated
	CRYPTOPP_UNUSED(v); CRYPTOPP_ASSERT(0);
	return _mm_setzero_si128();
}

template <>
inline __m128i UnpackXMM<0>(const __m128i& v)
{
	return _mm_shuffle_epi8(v, _mm_set_epi8(0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3));
}

template <>
inline __m128i UnpackXMM<1>(const __m128i& v)
{
	return _mm_shuffle_epi8(v, _mm_set_epi8(4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7, 4, 5, 6, 7));
}

template <>
inline __m128i UnpackXMM<2>(const __m128i& v)
{
	return _mm_shuffle_epi8(v, _mm_set_epi8(8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11, 8, 9, 10, 11));
}

template <>
inline __m128i UnpackXMM<3>(const __m128i& v)
{
	return _mm_shuffle_epi8(v, _mm_set_epi8(12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15, 12, 13, 14, 15));
}

template <unsigned int IDX>
inline __m128i RepackXMM(const __m128i& a, const __m128i& b, const __m128i& c, const __m128i& d)
{
	return UnpackXMM<IDX>(a, b, c, d);
}

template <unsigned int IDX>
inline __m128i RepackXMM(const __m128i& v)
{
	return UnpackXMM<IDX>(v);
}

inline void CHAM128_Enc_Block(__m128i& block0,
	const word32* subkeys, unsigned int rounds)
{
	// Rearrange the data for vectorization. UnpackXMM includes a
	// little-endian swap for SSE. Thanks to Peter Cordes for help
	// with packing and unpacking.
	// [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 B1 C1 D1][A2 B2 C2 D2] ...
	__m128i a = UnpackXMM<0>(block0);
	__m128i b = UnpackXMM<1>(block0);
	__m128i c = UnpackXMM<2>(block0);
	__m128i d = UnpackXMM<3>(block0);

	__m128i counter = _mm_set_epi32(0, 0, 0, 0);
	__m128i increment = _mm_set_epi32(1, 1, 1, 1);

	const unsigned int MASK = (rounds == 80 ? 7 : 15);
	for (int i = 0; i < static_cast<int>(rounds); i += 4)
	{
		__m128i k, k1, k2, t1, t2;
		k = _mm_castpd_si128(_mm_load_sd(CONST_DOUBLE_CAST(&subkeys[(i + 0) & MASK])));

		// Shuffle out two subkeys
		k1 = _mm_shuffle_epi8(k, _mm_set_epi8(3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0));
		k2 = _mm_shuffle_epi8(k, _mm_set_epi8(7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4));

		t1 = _mm_xor_si128(a, counter);
		t2 = _mm_xor_si128(RotateLeft32<1>(b), k1);
		a = RotateLeft32<8>(_mm_add_epi32(t1, t2));

		counter = _mm_add_epi32(counter, increment);

		t1 = _mm_xor_si128(b, counter);
		t2 = _mm_xor_si128(RotateLeft32<8>(c), k2);
		b = RotateLeft32<1>(_mm_add_epi32(t1, t2));

		counter = _mm_add_epi32(counter, increment);
		k = _mm_castpd_si128(_mm_load_sd(CONST_DOUBLE_CAST(&subkeys[(i + 2) & MASK])));

		// Shuffle out two subkeys
		k1 = _mm_shuffle_epi8(k, _mm_set_epi8(3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0));
		k2 = _mm_shuffle_epi8(k, _mm_set_epi8(7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4));

		t1 = _mm_xor_si128(c, counter);
		t2 = _mm_xor_si128(RotateLeft32<1>(d), k1);
		c = RotateLeft32<8>(_mm_add_epi32(t1, t2));

		counter = _mm_add_epi32(counter, increment);

		t1 = _mm_xor_si128(d, counter);
		t2 = _mm_xor_si128(RotateLeft32<8>(a), k2);
		d = RotateLeft32<1>(_mm_add_epi32(t1, t2));

		counter = _mm_add_epi32(counter, increment);
	}

	// [A1 B1 C1 D1][A2 B2 C2 D2] ... => [A1 A2 A3 A4][B1 B2 B3 B4] ...
	block0 = RepackXMM<0>(a, b, c, d);
}

inline void CHAM128_Dec_Block(__m128i& block0,
	const word32* subkeys, unsigned int rounds)
{
	// Rearrange the data for vectorization. UnpackXMM includes a
	// little-endian swap for SSE. Thanks to Peter Cordes for help
	// with packing and unpacking.
	// [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 B1 C1 D1][A2 B2 C2 D2] ...
	__m128i a = UnpackXMM<0>(block0);
	__m128i b = UnpackXMM<1>(block0);
	__m128i c = UnpackXMM<2>(block0);
	__m128i d = UnpackXMM<3>(block0);

	__m128i counter = _mm_set_epi32(rounds - 1, rounds - 1, rounds - 1, rounds - 1);
	__m128i decrement = _mm_set_epi32(1, 1, 1, 1);

	const unsigned int MASK = (rounds == 80 ? 7 : 15);
	for (int i = static_cast<int>(rounds) - 1; i >= 0; i -= 4)
	{
		__m128i k, k1, k2, t1, t2;
		k = _mm_castpd_si128(_mm_load_sd(CONST_DOUBLE_CAST(&subkeys[(i - 1) & MASK])));

		// Shuffle out two subkeys
		k1 = _mm_shuffle_epi8(k, _mm_set_epi8(7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4));
		k2 = _mm_shuffle_epi8(k, _mm_set_epi8(3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0));

		// Odd round
		t1 = RotateRight32<1>(d);
		t2 = _mm_xor_si128(RotateLeft32<8>(a), k1);
		d = _mm_xor_si128(_mm_sub_epi32(t1, t2), counter);

		counter = _mm_sub_epi32(counter, decrement);

		// Even round
		t1 = RotateRight32<8>(c);
		t2 = _mm_xor_si128(RotateLeft32<1>(d), k2);
		c = _mm_xor_si128(_mm_sub_epi32(t1, t2), counter);

		counter = _mm_sub_epi32(counter, decrement);
		k = _mm_castpd_si128(_mm_load_sd(CONST_DOUBLE_CAST(&subkeys[(i - 3) & MASK])));

		// Shuffle out two subkeys
		k1 = _mm_shuffle_epi8(k, _mm_set_epi8(7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4));
		k2 = _mm_shuffle_epi8(k, _mm_set_epi8(3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0));

		// Odd round
		t1 = RotateRight32<1>(b);
		t2 = _mm_xor_si128(RotateLeft32<8>(c), k1);
		b = _mm_xor_si128(_mm_sub_epi32(t1, t2), counter);

		counter = _mm_sub_epi32(counter, decrement);

		// Even round
		t1 = RotateRight32<8>(a);
		t2 = _mm_xor_si128(RotateLeft32<1>(b), k2);
		a = _mm_xor_si128(_mm_sub_epi32(t1, t2), counter);

		counter = _mm_sub_epi32(counter, decrement);
	}

	// [A1 B1 C1 D1][A2 B2 C2 D2] ... => [A1 A2 A3 A4][B1 B2 B3 B4] ...
	block0 = RepackXMM<0>(a, b, c, d);
}

inline void CHAM128_Enc_4_Blocks(__m128i& block0, __m128i& block1,
	__m128i& block2, __m128i& block3, const word32* subkeys, unsigned int rounds)
{
	// Rearrange the data for vectorization. UnpackXMM includes a
	// little-endian swap for SSE. Thanks to Peter Cordes for help
	// with packing and unpacking.
	// [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 B1 C1 D1][A2 B2 C2 D2] ...
	__m128i a = UnpackXMM<0>(block0, block1, block2, block3);
	__m128i b = UnpackXMM<1>(block0, block1, block2, block3);
	__m128i c = UnpackXMM<2>(block0, block1, block2, block3);
	__m128i d = UnpackXMM<3>(block0, block1, block2, block3);

	__m128i counter = _mm_set_epi32(0, 0, 0, 0);
	__m128i increment = _mm_set_epi32(1, 1, 1, 1);

	const unsigned int MASK = (rounds == 80 ? 7 : 15);
	for (int i = 0; i < static_cast<int>(rounds); i += 4)
	{
		__m128i k, k1, k2, t1, t2;
		k = _mm_castpd_si128(_mm_load_sd(CONST_DOUBLE_CAST(&subkeys[(i + 0) & MASK])));

		// Shuffle out two subkeys
		k1 = _mm_shuffle_epi8(k, _mm_set_epi8(3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0));
		k2 = _mm_shuffle_epi8(k, _mm_set_epi8(7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4));

		t1 = _mm_xor_si128(a, counter);
		t2 = _mm_xor_si128(RotateLeft32<1>(b), k1);
		a = RotateLeft32<8>(_mm_add_epi32(t1, t2));

		counter = _mm_add_epi32(counter, increment);

		t1 = _mm_xor_si128(b, counter);
		t2 = _mm_xor_si128(RotateLeft32<8>(c), k2);
		b = RotateLeft32<1>(_mm_add_epi32(t1, t2));

		counter = _mm_add_epi32(counter, increment);
		k = _mm_castpd_si128(_mm_load_sd(CONST_DOUBLE_CAST(&subkeys[(i + 2) & MASK])));

		// Shuffle out two subkeys
		k1 = _mm_shuffle_epi8(k, _mm_set_epi8(3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0));
		k2 = _mm_shuffle_epi8(k, _mm_set_epi8(7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4));

		t1 = _mm_xor_si128(c, counter);
		t2 = _mm_xor_si128(RotateLeft32<1>(d), k1);
		c = RotateLeft32<8>(_mm_add_epi32(t1, t2));

		counter = _mm_add_epi32(counter, increment);

		t1 = _mm_xor_si128(d, counter);
		t2 = _mm_xor_si128(RotateLeft32<8>(a), k2);
		d = RotateLeft32<1>(_mm_add_epi32(t1, t2));

		counter = _mm_add_epi32(counter, increment);
	}

	// [A1 B1 C1 D1][A2 B2 C2 D2] ... => [A1 A2 A3 A4][B1 B2 B3 B4] ...
	block0 = RepackXMM<0>(a, b, c, d);
	block1 = RepackXMM<1>(a, b, c, d);
	block2 = RepackXMM<2>(a, b, c, d);
	block3 = RepackXMM<3>(a, b, c, d);
}

inline void CHAM128_Dec_4_Blocks(__m128i& block0, __m128i& block1,
	__m128i& block2, __m128i& block3, const word32* subkeys, unsigned int rounds)
{
	// Rearrange the data for vectorization. UnpackXMM includes a
	// little-endian swap for SSE. Thanks to Peter Cordes for help
	// with packing and unpacking.
	// [A1 A2 A3 A4][B1 B2 B3 B4] ... => [A1 B1 C1 D1][A2 B2 C2 D2] ...
	__m128i a = UnpackXMM<0>(block0, block1, block2, block3);
	__m128i b = UnpackXMM<1>(block0, block1, block2, block3);
	__m128i c = UnpackXMM<2>(block0, block1, block2, block3);
	__m128i d = UnpackXMM<3>(block0, block1, block2, block3);

	__m128i counter = _mm_set_epi32(rounds - 1, rounds - 1, rounds - 1, rounds - 1);
	__m128i decrement = _mm_set_epi32(1, 1, 1, 1);

	const unsigned int MASK = (rounds == 80 ? 7 : 15);
	for (int i = static_cast<int>(rounds) - 1; i >= 0; i -= 4)
	{
		__m128i k, k1, k2, t1, t2;
		k = _mm_castpd_si128(_mm_load_sd(CONST_DOUBLE_CAST(&subkeys[(i - 1) & MASK])));

		// Shuffle out two subkeys
		k1 = _mm_shuffle_epi8(k, _mm_set_epi8(7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4));
		k2 = _mm_shuffle_epi8(k, _mm_set_epi8(3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0));

		// Odd round
		t1 = RotateRight32<1>(d);
		t2 = _mm_xor_si128(RotateLeft32<8>(a), k1);
		d = _mm_xor_si128(_mm_sub_epi32(t1, t2), counter);

		counter = _mm_sub_epi32(counter, decrement);

		// Even round
		t1 = RotateRight32<8>(c);
		t2 = _mm_xor_si128(RotateLeft32<1>(d), k2);
		c = _mm_xor_si128(_mm_sub_epi32(t1, t2), counter);

		counter = _mm_sub_epi32(counter, decrement);
		k = _mm_castpd_si128(_mm_load_sd(CONST_DOUBLE_CAST(&subkeys[(i - 3) & MASK])));

		// Shuffle out two subkeys
		k1 = _mm_shuffle_epi8(k, _mm_set_epi8(7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4, 7, 6, 5, 4));
		k2 = _mm_shuffle_epi8(k, _mm_set_epi8(3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0, 3, 2, 1, 0));

		// Odd round
		t1 = RotateRight32<1>(b);
		t2 = _mm_xor_si128(RotateLeft32<8>(c), k1);
		b = _mm_xor_si128(_mm_sub_epi32(t1, t2), counter);

		counter = _mm_sub_epi32(counter, decrement);

		// Even round
		t1 = RotateRight32<8>(a);
		t2 = _mm_xor_si128(RotateLeft32<1>(b), k2);
		a = _mm_xor_si128(_mm_sub_epi32(t1, t2), counter);

		counter = _mm_sub_epi32(counter, decrement);
	}

	// [A1 B1 C1 D1][A2 B2 C2 D2] ... => [A1 A2 A3 A4][B1 B2 B3 B4] ...
	block0 = RepackXMM<0>(a, b, c, d);
	block1 = RepackXMM<1>(a, b, c, d);
	block2 = RepackXMM<2>(a, b, c, d);
	block3 = RepackXMM<3>(a, b, c, d);
}

//////////////////////////////////////////////////////////////////////////

NAMESPACE_END  // W32

#endif  // CRYPTOPP_SSSE3_AVAILABLE

ANONYMOUS_NAMESPACE_END

NAMESPACE_BEGIN(CryptoPP)

#if defined(CRYPTOPP_SSSE3_AVAILABLE)
size_t CHAM128_Enc_AdvancedProcessBlocks_SSSE3(const word32* subKeys, size_t rounds,
	const byte* inBlocks, const byte* xorBlocks, byte* outBlocks, size_t length, word32 flags)
{
	return AdvancedProcessBlocks128_4x1_SSE(W32::CHAM128_Enc_Block, W32::CHAM128_Enc_4_Blocks,
		subKeys, rounds, inBlocks, xorBlocks, outBlocks, length, flags);
}

size_t CHAM128_Dec_AdvancedProcessBlocks_SSSE3(const word32* subKeys, size_t rounds,
	const byte* inBlocks, const byte* xorBlocks, byte* outBlocks, size_t length, word32 flags)
{
	return AdvancedProcessBlocks128_4x1_SSE(W32::CHAM128_Dec_Block, W32::CHAM128_Dec_4_Blocks,
		subKeys, rounds, inBlocks, xorBlocks, outBlocks, length, flags);
}
#endif // CRYPTOPP_SSSE3_AVAILABLE

NAMESPACE_END
