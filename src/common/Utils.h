#pragma once

#include "Numbers.h"

#include <format>
#include <vector>
#include <concepts>

template<std::floating_point T, typename CharT>
struct std::formatter<std::vector<T>, CharT>
{
    template<typename ParseCtx>
    constexpr ParseCtx::iterator parse(ParseCtx& ctx)
    {
        auto it = ctx.begin();
        return it;
    }

    template<typename FormatCtx>
    FormatCtx::iterator format(const std::vector<T>& m, FormatCtx& ctx) const {
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