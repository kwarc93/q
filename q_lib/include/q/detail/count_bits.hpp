/*=============================================================================
   Copyright (c) 2014-2023 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_Q_COUNT_BITS_HPP_MARCH_12_2018)
#define CYCFI_Q_COUNT_BITS_HPP_MARCH_12_2018

#ifdef _MSC_VER
# include <intrin.h>
# include <nmmintrin.h>
#endif

#define CYCFI_Q_USE_CUSTOM_POPCOUNT

namespace cycfi::q::detail
{
#if defined(CYCFI_Q_USE_CUSTOM_POPCOUNT)
   struct popcount_lookup
   {
       constexpr popcount_lookup() : wordbits{}
       {
           unsigned i;
           for (i = 1; sizeof(wordbits) / sizeof(*wordbits) > i; ++i)
               wordbits[i] = wordbits [i >> 1] + (1 & i);
       }

       constexpr int operator()(std::uint32_t i) const
       {
           return wordbits[i & 0xFFFF] + wordbits[i >> 16];
       }

       std::uint8_t wordbits[65536];
   };

   constexpr inline popcount_lookup custom_popcount;
#endif

   inline std::uint32_t count_bits(std::uint32_t i)
   {
#if defined(CYCFI_Q_USE_CUSTOM_POPCOUNT)
      return custom_popcount(i);
#elif defined(_MSC_VER)
      return __popcnt(i);
#elif defined(__GNUC__)
      return __builtin_popcount(i);
#else
# error Unsupported compiler
#endif
   }

#if (!defined(_MSC_VER) || defined(_WIN64))
   inline std::uint64_t count_bits(std::uint64_t i)
   {
#if defined(_MSC_VER)
      return _mm_popcnt_u64(i);
#elif defined(__GNUC__)
      return __builtin_popcountll(i);
#else
# error Unsupported compiler
#endif
   }
#endif // (!defined(_MSC_VER) || defined(_WIN64))
}

#endif

