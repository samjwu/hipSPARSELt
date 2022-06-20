/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) 2022 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *******************************************************************************/

/* ========================================Gtest Unit Check
 * ==================================================== */

/*! \brief gtest unit compare two matrices float/double/complex */

#pragma once

#include "hipsparselt.h"
#include "hipsparselt_math.hpp"
#include "hipsparselt_test.hpp"
#include "hipsparselt_vector.hpp"

#ifndef GOOGLE_TEST
#define UNIT_CHECK(M, N, lda, strideA, hCPU, hGPU, batch_count, UNIT_ASSERT_EQ)
#define UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, UNIT_ASSERT_EQ)
#else
#define UNIT_CHECK(M, N, lda, strideA, hCPU, hGPU, batch_count, UNIT_ASSERT_EQ)                  \
    do                                                                                           \
    {                                                                                            \
        for(size_t k = 0; k < batch_count; k++)                                                  \
            for(size_t j = 0; j < N; j++)                                                        \
                for(size_t i = 0; i < M; i++)                                                    \
                    if(hipsparselt_isnan(hCPU[i + j * size_t(lda) + k * strideA]))               \
                    {                                                                            \
                        ASSERT_TRUE(hipsparselt_isnan(hGPU[i + j * size_t(lda) + k * strideA])); \
                    }                                                                            \
                    else                                                                         \
                    {                                                                            \
                        UNIT_ASSERT_EQ(hCPU[i + j * size_t(lda) + k * strideA],                  \
                                       hGPU[i + j * size_t(lda) + k * strideA]);                 \
                    }                                                                            \
    } while(0)

#define UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, UNIT_ASSERT_EQ)              \
    do                                                                                \
    {                                                                                 \
        for(size_t k = 0; k < batch_count; k++)                                       \
            for(size_t j = 0; j < N; j++)                                             \
                for(size_t i = 0; i < M; i++)                                         \
                    if(hipsparselt_isnan(hCPU[k][i + j * size_t(lda)]))               \
                    {                                                                 \
                        ASSERT_TRUE(hipsparselt_isnan(hGPU[k][i + j * size_t(lda)])); \
                    }                                                                 \
                    else                                                              \
                    {                                                                 \
                        UNIT_ASSERT_EQ(hCPU[k][i + j * size_t(lda)],                  \
                                       hGPU[k][i + j * size_t(lda)]);                 \
                    }                                                                 \
    } while(0)

//#define ASSERT_HALF_EQ(a, b) ASSERT_FLOAT_EQ(float(a), float(b))
//#define ASSERT_BF16_EQ(a, b) ASSERT_FLOAT_EQ(float(a), float(b))

#define ASSERT_HALF_EQ(a, b)                                   \
    do                                                         \
    {                                                          \
        hipsparseLtHalf absA    = (a > 0) ? a : -a;            \
        hipsparseLtHalf absB    = (b > 0) ? b : -b;            \
        hipsparseLtHalf absDiff = (a - b > 0) ? a - b : b - a; \
        ASSERT_TRUE(absDiff / (absA + absB + 1) < 0.01);       \
    } while(0)

#define ASSERT_BF16_EQ(a, b)                                                                      \
    do                                                                                            \
    {                                                                                             \
        const hipsparseLtBfloat16 bf16A    = static_cast<hipsparseLtBfloat16>(a);                 \
        const hipsparseLtBfloat16 bf16B    = static_cast<hipsparseLtBfloat16>(b);                 \
        const hipsparseLtBfloat16 bf16Zero = static_cast<hipsparseLtBfloat16>(0.0f);              \
        const hipsparseLtBfloat16 bf16One  = static_cast<hipsparseLtBfloat16>(1.0f);              \
        hipsparseLtBfloat16       absA     = (bf16A > bf16Zero) ? bf16A : -bf16A;                 \
        hipsparseLtBfloat16       absB     = (bf16B > bf16Zero) ? bf16B : -bf16B;                 \
        hipsparseLtBfloat16 absDiff = (bf16A - bf16B > bf16Zero) ? bf16A - bf16B : bf16B - bf16A; \
        ASSERT_TRUE(absDiff / (absA + absB + bf16One) < static_cast<hipsparseLtBfloat16>(0.1f));  \
    } while(0)

// Compare float to hipsparseLtBfloat16
// Allow the hipsparseLtBfloat16 to match the rounded or truncated value of float
// Only call ASSERT_FLOAT_EQ with the rounded value if the truncated value does not match
#include <gtest/internal/gtest-internal.h>
#define ASSERT_FLOAT_BF16_EQ(a, b)                                                           \
    do                                                                                       \
    {                                                                                        \
        using testing::internal::FloatingPoint;                                              \
        if(!FloatingPoint<float>(b).AlmostEquals(                                            \
               FloatingPoint<float>(hipsparseLtBfloat16(a, hipsparseLtBfloat16::truncate)))) \
            ASSERT_FLOAT_EQ(b, hipsparseLtBfloat16(a));                                      \
    } while(0)

#define ASSERT_FLOAT_COMPLEX_EQ(a, b)                  \
    do                                                 \
    {                                                  \
        auto ta = (a), tb = (b);                       \
        ASSERT_FLOAT_EQ(std::real(ta), std::real(tb)); \
        ASSERT_FLOAT_EQ(std::imag(ta), std::imag(tb)); \
    } while(0)

#define ASSERT_DOUBLE_COMPLEX_EQ(a, b)                  \
    do                                                  \
    {                                                   \
        auto ta = (a), tb = (b);                        \
        ASSERT_DOUBLE_EQ(std::real(ta), std::real(tb)); \
        ASSERT_DOUBLE_EQ(std::imag(ta), std::imag(tb)); \
    } while(0)

#endif // GOOGLE_TEST

// TODO: Replace std::remove_cv_t with std::type_identity_t in C++20
// It is only used to make T_hpa non-deduced
template <typename T, typename T_hpa = T>
void unit_check_general(
    int64_t M, int64_t N, int64_t lda, const std::remove_cv_t<T_hpa>* hCPU, const T* hGPU);

template <>
inline void unit_check_general(int64_t                    M,
                               int64_t                    N,
                               int64_t                    lda,
                               const hipsparseLtBfloat16* hCPU,
                               const hipsparseLtBfloat16* hGPU)
{
    UNIT_CHECK(M, N, lda, 0, hCPU, hGPU, 1, ASSERT_BF16_EQ);
}

template <>
inline void unit_check_general<hipsparseLtBfloat16, float>(
    int64_t M, int64_t N, int64_t lda, const float* hCPU, const hipsparseLtBfloat16* hGPU)
{
    UNIT_CHECK(M, N, lda, 0, hCPU, hGPU, 1, ASSERT_FLOAT_BF16_EQ);
}

template <>
inline void unit_check_general(
    int64_t M, int64_t N, int64_t lda, const hipsparseLtHalf* hCPU, const hipsparseLtHalf* hGPU)
{
    UNIT_CHECK(M, N, lda, 0, hCPU, hGPU, 1, ASSERT_HALF_EQ);
}

template <>
inline void
    unit_check_general(int64_t M, int64_t N, int64_t lda, const float* hCPU, const float* hGPU)
{
    UNIT_CHECK(M, N, lda, 0, hCPU, hGPU, 1, ASSERT_FLOAT_EQ);
}

template <>
inline void
    unit_check_general(int64_t M, int64_t N, int64_t lda, const double* hCPU, const double* hGPU)
{
    UNIT_CHECK(M, N, lda, 0, hCPU, hGPU, 1, ASSERT_DOUBLE_EQ);
}

template <>
inline void
    unit_check_general(int64_t M, int64_t N, int64_t lda, const int64_t* hCPU, const int64_t* hGPU)
{
    UNIT_CHECK(M, N, lda, 0, hCPU, hGPU, 1, ASSERT_EQ);
}

template <>
inline void
    unit_check_general(int64_t M, int64_t N, int64_t lda, const int8_t* hCPU, const int8_t* hGPU)
{
    UNIT_CHECK(M, N, lda, 0, hCPU, hGPU, 1, ASSERT_EQ);
}

template <typename T, typename T_hpa = T>
void unit_check_general(int64_t                        M,
                        int64_t                        N,
                        int64_t                        lda,
                        int64_t                        strideA,
                        const std::remove_cv_t<T_hpa>* hCPU,
                        const T*                       hGPU,
                        int64_t                        batch_count);

template <>
inline void unit_check_general(int64_t                    M,
                               int64_t                    N,
                               int64_t                    lda,
                               int64_t                    strideA,
                               const hipsparseLtBfloat16* hCPU,
                               const hipsparseLtBfloat16* hGPU,
                               int64_t                    batch_count)
{
    UNIT_CHECK(M, N, lda, strideA, hCPU, hGPU, batch_count, ASSERT_BF16_EQ);
}

template <>
inline void unit_check_general<hipsparseLtBfloat16, float>(int64_t                    M,
                                                           int64_t                    N,
                                                           int64_t                    lda,
                                                           int64_t                    strideA,
                                                           const float*               hCPU,
                                                           const hipsparseLtBfloat16* hGPU,
                                                           int64_t                    batch_count)
{
    UNIT_CHECK(M, N, lda, strideA, hCPU, hGPU, batch_count, ASSERT_FLOAT_BF16_EQ);
}

template <>
inline void unit_check_general(int64_t                M,
                               int64_t                N,
                               int64_t                lda,
                               int64_t                strideA,
                               const hipsparseLtHalf* hCPU,
                               const hipsparseLtHalf* hGPU,
                               int64_t                batch_count)
{
    UNIT_CHECK(M, N, lda, strideA, hCPU, hGPU, batch_count, ASSERT_HALF_EQ);
}

template <>
inline void unit_check_general(int64_t      M,
                               int64_t      N,
                               int64_t      lda,
                               int64_t      strideA,
                               const float* hCPU,
                               const float* hGPU,
                               int64_t      batch_count)
{
    UNIT_CHECK(M, N, lda, strideA, hCPU, hGPU, batch_count, ASSERT_FLOAT_EQ);
}

template <>
inline void unit_check_general(int64_t       M,
                               int64_t       N,
                               int64_t       lda,
                               int64_t       strideA,
                               const double* hCPU,
                               const double* hGPU,
                               int64_t       batch_count)
{
    UNIT_CHECK(M, N, lda, strideA, hCPU, hGPU, batch_count, ASSERT_DOUBLE_EQ);
}

template <>
inline void unit_check_general(int64_t        M,
                               int64_t        N,
                               int64_t        lda,
                               int64_t        strideA,
                               const int64_t* hCPU,
                               const int64_t* hGPU,
                               int64_t        batch_count)
{
    UNIT_CHECK(M, N, lda, strideA, hCPU, hGPU, batch_count, ASSERT_EQ);
}

template <>
inline void unit_check_general(int64_t       M,
                               int64_t       N,
                               int64_t       lda,
                               int64_t       strideA,
                               const int8_t* hCPU,
                               const int8_t* hGPU,
                               int64_t       batch_count)
{
    UNIT_CHECK(M, N, lda, strideA, hCPU, hGPU, batch_count, ASSERT_EQ);
}

template <typename T, typename T_hpa = T>
void unit_check_general(int64_t                                    M,
                        int64_t                                    N,
                        int64_t                                    lda,
                        const host_vector<std::remove_cv_t<T_hpa>> hCPU[],
                        const host_vector<T>                       hGPU[],
                        int64_t                                    batch_count);

template <>
inline void unit_check_general(int64_t                                M,
                               int64_t                                N,
                               int64_t                                lda,
                               const host_vector<hipsparseLtBfloat16> hCPU[],
                               const host_vector<hipsparseLtBfloat16> hGPU[],
                               int64_t                                batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_BF16_EQ);
}

template <>
inline void
    unit_check_general<hipsparseLtBfloat16, float>(int64_t                                M,
                                                   int64_t                                N,
                                                   int64_t                                lda,
                                                   const host_vector<float>               hCPU[],
                                                   const host_vector<hipsparseLtBfloat16> hGPU[],
                                                   int64_t batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_FLOAT_BF16_EQ);
}

template <>
inline void unit_check_general(int64_t                            M,
                               int64_t                            N,
                               int64_t                            lda,
                               const host_vector<hipsparseLtHalf> hCPU[],
                               const host_vector<hipsparseLtHalf> hGPU[],
                               int64_t                            batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_HALF_EQ);
}

template <>
inline void unit_check_general(int64_t                M,
                               int64_t                N,
                               int64_t                lda,
                               const host_vector<int> hCPU[],
                               const host_vector<int> hGPU[],
                               int64_t                batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_EQ);
}

template <>
inline void unit_check_general(int64_t                   M,
                               int64_t                   N,
                               int64_t                   lda,
                               const host_vector<int8_t> hCPU[],
                               const host_vector<int8_t> hGPU[],
                               int64_t                   batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_EQ);
}

template <>
inline void unit_check_general(int64_t                  M,
                               int64_t                  N,
                               int64_t                  lda,
                               const host_vector<float> hCPU[],
                               const host_vector<float> hGPU[],
                               int64_t                  batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_FLOAT_EQ);
}

template <>
inline void unit_check_general(int64_t                   M,
                               int64_t                   N,
                               int64_t                   lda,
                               const host_vector<double> hCPU[],
                               const host_vector<double> hGPU[],
                               int64_t                   batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_DOUBLE_EQ);
}

template <typename T, typename T_hpa = T>
void unit_check_general(int64_t                              M,
                        int64_t                              N,
                        int64_t                              lda,
                        const std::remove_cv_t<T_hpa>* const hCPU[],
                        const T* const                       hGPU[],
                        int64_t                              batch_count);

template <>
inline void unit_check_general(int64_t                          M,
                               int64_t                          N,
                               int64_t                          lda,
                               const hipsparseLtBfloat16* const hCPU[],
                               const hipsparseLtBfloat16* const hGPU[],
                               int64_t                          batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_BF16_EQ);
}

template <>
inline void unit_check_general<hipsparseLtBfloat16, float>(int64_t                          M,
                                                           int64_t                          N,
                                                           int64_t                          lda,
                                                           const float* const               hCPU[],
                                                           const hipsparseLtBfloat16* const hGPU[],
                                                           int64_t batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_FLOAT_BF16_EQ);
}

template <>
inline void unit_check_general(int64_t                      M,
                               int64_t                      N,
                               int64_t                      lda,
                               const hipsparseLtHalf* const hCPU[],
                               const hipsparseLtHalf* const hGPU[],
                               int64_t                      batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_HALF_EQ);
}

template <>
inline void unit_check_general(int64_t          M,
                               int64_t          N,
                               int64_t          lda,
                               const int* const hCPU[],
                               const int* const hGPU[],
                               int64_t          batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_EQ);
}

template <>
inline void unit_check_general(int64_t             M,
                               int64_t             N,
                               int64_t             lda,
                               const int8_t* const hCPU[],
                               const int8_t* const hGPU[],
                               int64_t             batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_EQ);
}

template <>
inline void unit_check_general(int64_t            M,
                               int64_t            N,
                               int64_t            lda,
                               const float* const hCPU[],
                               const float* const hGPU[],
                               int64_t            batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_FLOAT_EQ);
}

template <>
inline void unit_check_general(int64_t             M,
                               int64_t             N,
                               int64_t             lda,
                               const double* const hCPU[],
                               const double* const hGPU[],
                               int64_t             batch_count)
{
    UNIT_CHECK_B(M, N, lda, hCPU, hGPU, batch_count, ASSERT_DOUBLE_EQ);
}

template <typename T>
inline void trsm_err_res_check(T max_error, int64_t M, T forward_tolerance, T eps)
{
#ifdef GOOGLE_TEST
    ASSERT_LE(max_error, forward_tolerance * eps * M);
#endif
}

template <typename T>
constexpr double get_epsilon()
{
    return std::numeric_limits<T>::epsilon();
}

template <typename T>
inline int64_t unit_check_diff(
    int64_t M, int64_t N, int64_t lda, int64_t stride, T* hCPU, T* hGPU, int64_t batch_count)
{
    int64_t error = 0;
    do
    {
        for(size_t k = 0; k < batch_count; k++)
            for(size_t j = 0; j < N; j++)
                for(size_t i = 0; i < M; i++)
                    if(hipsparselt_isnan(hCPU[i + j * size_t(lda) + k * stride]))
                    {
                        error += hipsparselt_isnan(hGPU[i + j * size_t(lda) + k * stride]) ? 0 : 1;
                    }
                    else
                    {
                        error += (hCPU[i + j * size_t(lda) + k * stride]
                                  == hGPU[i + j * size_t(lda) + k * stride])
                                     ? 0
                                     : 1;
                    }
    } while(0);
    return error;
}
