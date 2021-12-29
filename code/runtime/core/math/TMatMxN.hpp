/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* MIT License                                                                    */
/*                                                                                */
/* Copyright (c) 2018 - 2021 Egor Orachyov                                        */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/**********************************************************************************/

#ifndef BERSERK_TMATMXN_H
#define BERSERK_TMATMXN_H

#include <core/Config.hpp>
#include <core/Typedefs.hpp>
#include <core/math/MathUtils.hpp>
#include <core/math/TVecN.hpp>

#include <array>
#include <cassert>

BRK_NS_BEGIN

/**
 * @addtogroup core
 * @{
 */

/**
 * @class TMatMxN
 * @brief Generic matrix of M x N size for space of type T.
 * @note Matrix data stored in row-major order
 *
 * @tparam T Type of scalar matrix elements
 * @tparam M Number of matrix rows
 * @tparam N Number of matrix columns
 */
template<typename T, uint32 M, uint32 N>
class TMatMxN {
public:
    TMatMxN() noexcept {
        Zero();
    }

    explicit TMatMxN(const std::array<TVecN<T, N>, M> &rows) noexcept {
        uint32 row = 0;

        for (auto &r : rows) {
            for (uint32 j = 0; j < N; j++) {
                values[row * N + j] = r[j];
            }
        }
    }

    TMatMxN(const T *vs, size_t count) {
        assert(count == GetSize());
        Memory::Copy(values, vs, sizeof(T) * count);
    }

    TMatMxN(T m_00, T m_01,
            T m_10, T m_11) noexcept : TMatMxN() {
        static_assert(M >= 2 && N >= 2, "Cannot apply to this type");

        values[0 * N + 0] = m_00;
        values[0 * N + 1] = m_01;
        values[1 * N + 0] = m_10;
        values[1 * N + 1] = m_11;
    }

    TMatMxN(T m_00, T m_01, T m_02,
            T m_10, T m_11, T m_12,
            T m_20, T m_21, T m_22) noexcept : TMatMxN() {
        static_assert(M >= 3 && N >= 3, "Cannot apply to this type");

        values[0 * N + 0] = m_00;
        values[0 * N + 1] = m_01;
        values[0 * N + 2] = m_02;

        values[1 * N + 0] = m_10;
        values[1 * N + 1] = m_11;
        values[1 * N + 2] = m_12;

        values[2 * N + 0] = m_20;
        values[2 * N + 1] = m_21;
        values[2 * N + 2] = m_22;
    }

    TMatMxN(T m_00, T m_01, T m_02, T m_03,
            T m_10, T m_11, T m_12, T m_13,
            T m_20, T m_21, T m_22, T m_23,
            T m_30, T m_31, T m_32, T m_33) noexcept : TMatMxN() {
        static_assert(M >= 3 && N >= 3, "Cannot apply to this type");

        values[0 * N + 0] = m_00;
        values[0 * N + 1] = m_01;
        values[0 * N + 2] = m_02;
        values[0 * N + 3] = m_03;

        values[1 * N + 0] = m_10;
        values[1 * N + 1] = m_11;
        values[1 * N + 2] = m_12;
        values[1 * N + 3] = m_13;

        values[2 * N + 0] = m_20;
        values[2 * N + 1] = m_21;
        values[2 * N + 2] = m_22;
        values[2 * N + 3] = m_23;

        values[3 * N + 0] = m_30;
        values[3 * N + 1] = m_31;
        values[3 * N + 2] = m_32;
        values[3 * N + 3] = m_33;
    }

    TMatMxN(const TMatMxN &other) noexcept {
        for (uint32 i = 0; i < M * N; i++) {
            values[i] = other.values[i];
        }
    }

    TMatMxN(TMatMxN &&other) noexcept {
        for (uint32 i = 0; i < M * N; i++) {
            values[i] = other.values[i];
        }
    }

    template<uint32 M1, uint32 N1>
    TMatMxN(const TMatMxN<T, M1, N1> &other) : TMatMxN() {
        static_assert(M1 <= M, "Matrix too large");
        static_assert(N1 <= N, "Matrix too large");

        for (uint32 i = 0; i < M1; i++) {
            for (uint32 j = 0; j < N1; j++) {
                values[i * N + j] = other.values[i * N1 + j];
            }
        }

        for (uint32 i = M1; i < M; i++) {
            values[i * N + i] = 1;
        }
    }

    TMatMxN &operator+=(const TMatMxN &other) {
        for (uint32 i = 0; i < M * N; i++) {
            values[i] += other.values[i];
        }
        return *this;
    }

    TMatMxN &operator-=(const TMatMxN &other) {
        for (uint32 i = 0; i < M * N; i++) {
            values[i] -= other.values[i];
        }
        return *this;
    }

    TMatMxN operator+(const TMatMxN &other) const {
        TMatMxN result = *this;
        result += other;
        return result;
    }

    TMatMxN operator-(const TMatMxN &other) const {
        TMatMxN result = *this;
        result -= other;
        return result;
    }

    template<uint32 K>
    TMatMxN<T, M, K> operator*(const TMatMxN<T, N, K> &other) const {
        TMatMxN<T, M, K> result;

        for (uint32 i = 0; i < M; i++) {
            for (uint32 j = 0; j < K; j++) {
                T v = 0;

                for (uint32 f = 0; f < N; f++) {
                    v += values[i * N + f] * other.values[f * K + j];
                }

                result.values[i * K + j] = v;
            }
        }

        return result;
    }

    TMatMxN &operator*=(T other) {
        for (uint32 i = 0; i < M * N; i++) {
            values[i] *= other;
        }
        return *this;
    }

    TMatMxN &operator/=(T other) {
        for (uint32 i = 0; i < M * N; i++) {
            values[i] /= other;
        }
        return *this;
    }

    TMatMxN &operator+=(T other) {
        for (uint32 i = 0; i < M * N; i++) {
            values[i] += other;
        }
        return *this;
    }

    TMatMxN &operator-=(T other) {
        for (uint32 i = 0; i < M * N; i++) {
            values[i] -= other;
        }
        return *this;
    }

    TMatMxN operator*(T value) const {
        TMatMxN result = *this;
        result *= value;
        return result;
    }

    TMatMxN operator/(T value) const {
        TMatMxN result = *this;
        result /= value;
        return result;
    }

    TMatMxN operator+(T value) const {
        TMatMxN result = *this;
        result += value;
        return result;
    }

    TMatMxN operator-(T value) const {
        TMatMxN result = *this;
        result -= value;
        return result;
    }

    TMatMxN &operator=(const TMatMxN &other) {
        for (uint32 i = 0; i < M * N; i++) {
            values[i] = other.values[i];
        }
        return *this;
    }

    template<uint32 M1, uint32 N1>
    TMatMxN<T, M1, N1> SubMatrix(uint32 i, uint32 j) const {
        if (i + M1 > M) return {};
        if (j + N1 > N) return {};

        TMatMxN<T, M1, N1> result;

        for (uint32 si = 0; si < M1; si++) {
            const T *row = &values[(si + i) * N + j];
            for (uint32 sj = 0; sj < N1; sj++) {
                result.values[si * N1 + sj] = *row;
                row += 1;
            }
        }

        return result;
    }

    template<uint32 M1, uint32 N1>
    void SubMatrix(TMatMxN<T, M1, N1> &result, uint32 i, uint32 j) const {
        if (i + M1 > M) return;
        if (j + N1 > N) return;

        for (uint32 si = 0; si < M1; si++) {
            const T *row = &values[(si + i) * N + j];
            for (uint32 sj = 0; sj < N1; sj++) {
                result.values[si * N1 + sj] = *row;
                row += 1;
            }
        }
    }

    TMatMxN<T, M, N - 1> ExcludeColumn(uint32 columnIndex) const {
        TMatMxN<T, M, N - 1> result;

        for (uint32 i = 0; i < M; i++) {
            for (uint32 j = 0; j < columnIndex; j++) {
                result.values[i * result.GetDimN() + j] = values[i * N + j];
            }

            for (uint32 j = columnIndex; j < result.GetDimN(); j++) {
                result.values[i * result.GetDimN() + j] = values[i * N + j + 1];
            }
        }

        return result;
    }

    TMatMxN<T, N, M> Transpose() const {
        TMatMxN<T, N, M> result;

        for (uint32 i = 0; i < M; i++) {
            for (uint32 j = 0; j < N; j++) {
                result.values[j * M + i] = values[i * N + j];
            }
        }

        return result;
    }

    TVecN<T, M> GetColumn(uint32 index) const {
        assert(index < N);

        TVecN<T, M> result;
        for (uint32 i = 0; i < M; i++) {
            result.values[i] = values[i * N + index];
        }

        return result;
    }

    TVecN<T, N> GetRow(uint32 index) const {
        assert(index < M);

        TVecN<T, N> result;
        for (uint32 i = 0; i < N; i++) {
            result.values[i] = values[index * N + i];
        }

        return result;
    }

    TVecN<T, M> operator*(const TVecN<T, N> &v) const {
        TVecN<T, M> result;

        for (uint32 i = 0; i < M; i++) {
            for (uint32 j = 0; j < N; j++) {
                result.values[i] += values[i * N + j] * v[j];
            }
        }

        return result;
    }

private:
    template<typename D, uint32 DM, uint32 DN>
    struct TDetMxN;

    template<typename D>
    struct TDetMxN<D, 1, 1> {
        T operator()(const TMatMxN<T, 1, 1> &m) const {
            return m.values[0];
        }
    };

    template<typename D>
    struct TDetMxN<D, 2, 2> {
        T operator()(const TMatMxN<T, 2, 2> &m) const {
            return m.values[0] * m.values[3] - m.values[1] * m.values[2];
        }
    };

    template<typename D, uint32 DN>
    struct TDetMxN<D, DN, DN> {
        T operator()(const TMatMxN<T, DN, DN> &m) const {
            T result = 0;
            TMatMxN<T, DN - 1, DN> sub;
            m.Submatrix(sub, 1, 0);

            for (uint32 i = 0; i < DN; i++) {
                result += (i % 2 ? -1 : 1) * m.values[i] * sub.ExcludeColumn(i).Det();
            }

            return result;
        }
    };

    template<typename D, uint32 DM, uint32 DN>
    struct TDetMxN {
        T operator()(const TMatMxN<T, DM, DN> &m) const {
            return 0;
        }
    };

public:
    T Det() const {
        TDetMxN<void, M, N> det;
        return det(*this);
    }

    void Zero() {
        for (auto &v : values) {
            v = 0;
        }
    }

    T *operator[](uint32 i) { return &values[i * N]; }
    const T *operator[](uint32 i) const { return &values[i * N]; }

    T *GetData() { return values; }
    const T *GetData() const { return values; }

    void CopyTransposed(T *buffer) const {
        for (uint32 i = 0; i < M; i++) {
            for (uint32 j = 0; j < N; j++) {
                buffer[j * M + i] = values[i * N + j];
            }
        }
    }

    constexpr static uint32 GetStride() { return N * sizeof(T); }
    constexpr static uint32 GetSize() { return N * M; }

    constexpr static uint32 GetDimM() { return M; }
    constexpr static uint32 GetDimN() { return N; }

public:
    T values[M * N];
};

using Mat2x2f = TMatMxN<float, 2, 2>;
using Mat3x3f = TMatMxN<float, 3, 3>;
using Mat4x4f = TMatMxN<float, 4, 4>;

/**
 * @}
 */

BRK_NS_END

#endif//BERSERK_TMATMXN_H