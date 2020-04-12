/**********************************************************************************/
/* This file is part of Berserk Engine project                                    */
/* https://github.com/EgorOrachyov/Berserk                                        */
/**********************************************************************************/
/* Licensed under MIT License                                                     */
/* Copyright (c) 2019 - 2020 Egor Orachyov                                        */
/**********************************************************************************/

#ifndef BERSERK_TMATMXN_H
#define BERSERK_TMATMXN_H

#include <Math/Math.h>
#include <Math/TVecN.h>
#include <initializer_list>

namespace Berserk {

    /**
     * Generic matrix of M x N size for space of type T.
     * Matrix data stored in row-major order
     * @tparam T Type of scalar matrix elements
     * @tparam M Number of matrix rows
     * @tparam N Number of matrix columns
     */
    template <typename T, uint32 M, uint32 N>
    class TMatMxN {
    public:

        TMatMxN() noexcept {
            zero();
        }

        TMatMxN(const std::initializer_list<T> &list) noexcept : TMatMxN<T,M,N>() {
            uint32 i = 0;
            for (const auto& v: list) {

                if (i >= M * N) return;

                values[i] = v;
                i += 1;
            }
        }

        TMatMxN(const TMatMxN& other) noexcept {
            for (uint32 i = 0; i < M * N; i++) {
                values[i] = other.values[i];
            }
        }

        TMatMxN(TMatMxN&& other) noexcept {
            for (uint32 i = 0; i < M * N; i++) {
                values[i] = other.values[i];
            }
        }

        TMatMxN& operator+=(const TMatMxN& other) {
            for (uint32 i = 0; i < M * N; i++) {
                values[i] += other.values[i];
            }
            return *this;
        }
        TMatMxN& operator-=(const TMatMxN& other) {
            for (uint32 i = 0; i < M * N; i++) {
                values[i] -= other.values[i];
            }
            return *this;
        }

        TMatMxN operator+(const TMatMxN& other) const {
            TMatMxN result = *this;
            result += other;
            return result;
        }
        TMatMxN operator-(const TMatMxN& other) const {
            TMatMxN result = *this;
            result -= other;
            return result;
        }
        template <uint32 K>
        TMatMxN<T,M,K> operator*(const TMatMxN<T,N,K>& other) const {
            TMatMxN<T,M,K> result;

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

        TMatMxN& operator*=(T other) {
            for (uint32 i = 0; i < M * N; i++) {
                values[i] *= other;
            }
            return *this;
        }
        TMatMxN& operator/=(T other) {
            for (uint32 i = 0; i < M * N; i++) {
                values[i] /= other;
            }
            return *this;
        }
        TMatMxN& operator+=(T other) {
            for (uint32 i = 0; i < M * N; i++) {
                values[i] += other;
            }
            return *this;
        }
        TMatMxN& operator-=(T other) {
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

        TMatMxN &operator=(const TMatMxN& other) {
            for (uint32 i = 0; i < M * N; i++) {
                values[i] = other.values[i];
            }
            return *this;
        }

        template <uint32 M1, uint32 N1>
        TMatMxN<T,M1,N1> submatrix(uint32 i, uint32 j) const {
            if (i + M1 > M) return {};
            if (j + N1 > N) return {};

            TMatMxN<T,M1,N1> result;

            for (uint32 si = 0; si < M1; si++) {
                const T* row = &values[(si + i) * N + j];
                for (uint32 sj = 0; sj < N1; sj++) {
                    result.values[si * N1 + sj] = *row;
                    row += 1;
                }
            }

            return result;
        }

        template <uint32 M1, uint32 N1>
        void submatrix(TMatMxN<T,M1,N1> &result, uint32 i, uint32 j) const {
            if (i + M1 > M) return;
            if (j + N1 > N) return;

            for (uint32 si = 0; si < M1; si++) {
                const T* row = &values[(si + i) * N + j];
                for (uint32 sj = 0; sj < N1; sj++) {
                    result.values[si * N1 + sj] = *row;
                    row += 1;
                }
            }
        }

        TMatMxN<T,M,N-1> excludeColumn(uint32 columnIndex) const {
            TMatMxN<T,M,N-1> result;

            for (uint32 i = 0; i < M; i++) {
                for (uint32 j = 0; j < columnIndex; j++) {
                    result.values[i * result.dimN() + j] = values[i * N + j];
                }

                for (uint32 j = columnIndex; j < result.dimN(); j++) {
                    result.values[i * result.dimN() + j] = values[i * N + j + 1];
                }
            }

            return result;
        }

        TMatMxN<T,N,M> transpose() const {
            TMatMxN<T,N,M> result;

            for (uint32 i = 0; i < M; i++) {
                for (uint32 j = 0; j < N; j++) {
                    result.values[j * M + i] = values[i * N + j];
                }
            }

            return result;
        }

        TVecN<T,M> getColumn(uint32 index) {
            if (index >= N)
                return {};

            TVecN<T,M> result;
            for (uint32 i = 0; i < M; i++) {
                result.values[i] = values[i * N + index];
            }

            return result;
        }

        TVecN<T,N> getRow(uint32 index) {
            if (index >= M)
                return {};

            TVecN<T,N> result;
            for (uint32 i = 0; i < N; i++) {
                result.values[i] = values[index * N + i];
            }

            return result;
        }

        TVecN<T,M> operator*(const TVecN<T,N> &v) const {
            TVecN<T,M> result;

            for (uint32 i = 0; i < M; i++) {
                for (uint32 j = 0; j < N; j++) {
                    result.values[i] += values[i * N + j] * v[j];
                }
            }

            return result;
        }

    private:
        template <uint32 DM, uint32 DN>
        struct TDetMxN;

        template <>
        struct TDetMxN<1,1> {
            T operator()(const TMatMxN<T,1,1> &m) const {
                return m.values[0];
            }
        };

        template <>
        struct TDetMxN<2,2> {
            T operator()(const TMatMxN<T,2,2> &m) const {
                return m.values[0] * m.values[3] - m.values[1] * m.values[2];
            }
        };

        template <uint32 DN>
        struct TDetMxN<DN,DN> {
            T operator()(const TMatMxN<T,DN,DN> &m) const {
                T result = 0;
                TMatMxN<T,DN-1,DN> sub;
                m.submatrix(sub, 1,0);

                for (uint32 i = 0; i < DN; i++) {
                    result += (i % 2? -1: 1) * m.values[i] * sub.excludeColumn(i).det();
                }

                return result;
            }
        };

        template <uint32 DM, uint32 DN>
        struct TDetMxN {
            T operator()(const TMatMxN<T,DM,DN> &m) const {
                return 0;
            }
        };

    public:
        T det() const {
            TDetMxN<M,N> det;
            return det(*this);
        }

        void zero() {
            for (auto& v: values) {
                v = 0;
            }
        }

        T* operator[](uint32 i) { return &values[i * N]; }
        const T* operator[](uint32 i) const { return &values[i * N]; }

        T* data() { return values; }
        const T* data() const { return values; }

        void copyTransposed(T* buffer) const {
            for (uint32 i = 0; i < M; i++) {
                for (uint32 j = 0; j < N; j++) {
                    buffer[j * M + i] = values[i * N + j];
                }
            }
        }

        constexpr static uint32 stride() { return N * sizeof(T); }
        constexpr static uint32 size()   { return N * M; }
        constexpr static uint32 dimM()   { return M; }
        constexpr static uint32 dimN()   { return N; }
    public:
        T values[M * N];
    };

}



#endif //BERSERK_TMATMXN_H
