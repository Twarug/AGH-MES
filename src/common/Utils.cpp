#include "Utils.h"

namespace mes
{
    std::vector<f32> operator+(const std::vector<f32>& lhs, const std::vector<f32>& rhs)
    {
        if (lhs.size() != rhs.size())
            throw std::runtime_error("Vectors must have the same size");

        std::vector<f32> result(lhs.size());
        for (u64 i = 0; i < lhs.size(); ++i) {
            result[i] = lhs[i] + rhs[i];
        }
        return result;
    }

    std::vector<f32>& operator+=(std::vector<f32>& lhs, const std::vector<f32>& rhs)
    {
        if (lhs.size() != rhs.size())
            throw std::runtime_error("Vectors must have the same size");

        for (u64 i = 0; i < lhs.size(); ++i) {
            lhs[i] += rhs[i];
        }

        return lhs;
    }

    std::vector<f32> operator-(const std::vector<f32>& lhs, const std::vector<f32>& rhs)
    {
        if (lhs.size() != rhs.size())
            throw std::runtime_error("Vectors must have the same size");

        std::vector<f32> result(lhs.size());
        for (u64 i = 0; i < lhs.size(); ++i) {
            result[i] = lhs[i] - rhs[i];
        }

        return result;
    }

    std::vector<f32>& operator*=(std::vector<f32>& lhs, const std::vector<f32>& rhs)
    {
        if (lhs.size() != rhs.size())
            throw std::runtime_error("Vectors must have the same size");

        for (u64 i = 0; i < lhs.size(); ++i) {
            lhs[i] *= rhs[i];
        }

        return lhs;
    }
}