#pragma once

#include "Numbers.h"

#include <format>
#include <vector>
#include <concepts>

template<std::floating_point T, typename CharT>
struct std::formatter<std::vector<T>, CharT>
{
    template<typename ParseCtx>
    constexpr typename ParseCtx::iterator parse(ParseCtx& ctx)
    {
        auto it = ctx.begin();
        return it;
    }

    template<typename FormatCtx>
    typename FormatCtx::iterator format(const std::vector<T>& m, FormatCtx& ctx) const {
        auto out = ctx.out();
        out = format_to(out, "[");
        for (mes::u64 i = 0; i < m.size(); ++i) {
            out = format_to(out, "{:6.2f}", m[i]);
            if (i != m.size() - 1) {
                out = format_to(out, " ");
            }
        }
        return format_to(out, "]");
    }
};


namespace mes
{
    std::vector<f32> operator+(const std::vector<f32>& lhs, const std::vector<f32>& rhs);
    std::vector<f32>& operator+=(std::vector<f32>& lhs, const std::vector<f32>& rhs);

    std::vector<f32> operator-(const std::vector<f32>& lhs, const std::vector<f32>& rhs);

    std::vector<f32>& operator*=(std::vector<f32>& lhs, const std::vector<f32>& rhs);
}
