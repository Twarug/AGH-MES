#pragma once
#include "Numbers.h"
#include <utility>

namespace mes {

    struct Matrix
    {
        u64 rows;
        u64 cols;
        f32* data;

        Matrix() : rows(0), cols(0), data(nullptr) {}

        Matrix(u64 rows, u64 cols) : rows(rows), cols(cols) {
            data = new f32[rows * cols]();
        }

        Matrix(const Matrix& other);
        Matrix& operator=(const Matrix& other);

        Matrix(Matrix&& other) noexcept;
        Matrix& operator=(Matrix&& other) noexcept;

        ~Matrix() { delete[] data; }

        f32& operator()(u64 i, u64 j) { return data[i * cols + j]; }
        f32 operator()(u64 i, u64 j) const { return data[i * cols + j]; }

        Matrix operator*(f32 scalar) const;
        Matrix& operator*=(f32 scalar);

        Matrix& operator+=(const Matrix& other);

        f32 determinant() const;

        Matrix inverse() const;

        Matrix subMatrix(u64 i, u64 j) const;
    };
}

#include <format>

template<typename CharT>
struct std::formatter<mes::Matrix, CharT>
{
    bool withNewline = false;

    template<typename ParseCtx>
    constexpr typename ParseCtx::iterator parse(ParseCtx& ctx)
    {
        auto it = ctx.begin();
        if (it != ctx.end()) {
            withNewline = true;
            ++it;
        }
        return it;
    }

    template<typename FormatCtx>
    typename FormatCtx::iterator format(const mes::Matrix& m, FormatCtx& ctx) const {
        auto out = ctx.out();
        out = format_to(out, "[");
        if (withNewline)
            out = format_to(out, "\n");
        for (mes::u64 i = 0; i < m.rows; ++i) {
            out = format_to(out, "[");
            for (mes::u64 j = 0; j < m.cols; ++j) {
                out = format_to(out, "{:6.2f}", m(i, j));
                if (j != m.cols - 1) {
                    out = format_to(out, " ");
                }
            }
            out = format_to(out, "]");
            if (withNewline)
                out = format_to(out, "\n");
        }
        return format_to(out, "]");
    }
};
