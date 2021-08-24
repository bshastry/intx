// intx: extended precision integer library.
// Copyright 2021 Pawel Bylica.
// Licensed under the Apache License, Version 2.0.
#pragma once

#include <intx/intx.hpp>

namespace intx::test
{
[[gnu::noinline]] static uint256 addmod_public(
    const uint256& x, const uint256& y, const uint256& mod) noexcept
{
    return addmod(x, y, mod);
}

[[gnu::noinline]] static uint256 addmod_simple(
    const uint256& x, const uint256& y, const uint256& mod) noexcept
{
    const auto s = add_with_carry(x, y);
    uint<256 + 64> n = s.value;
    n[4] = s.carry;
    return udivrem(n, mod).rem;
}

[[gnu::noinline]] static uint256 addmod_v2(
    const uint256& x, const uint256& y, const uint256& mod) noexcept
{
    const auto s = add_with_carry(x, y);
    if (!s.carry)
    {
        if (x < mod && y < mod)
        {
            if (s.value >= mod)
                return s.value - mod;
            else
                return s.value;
        }
        return s.value % mod;
    }

    uint<256 + 64> n = s.value;
    n[4] = s.carry;
    return udivrem(n, mod).rem;
}

[[gnu::noinline]] static uint256 addmod_v4(
    const uint256& x, const uint256& y, const uint256& mod) noexcept
{
    const auto s = add_with_carry(x, y);
    if (!s.carry)
    {
        if (x < mod && y < mod)
        {
            if (s.value >= mod)
                return s.value - mod;
            else
                return s.value;
        }
        return s.value % mod;
    }

    uint<256 + 64> n = s.value;
    n[4] = s.carry;

    if (x < mod && y < mod)
    {
        return s.value - mod;
    }

    return udivrem(n, mod).rem;
}

[[gnu::noinline]] static uint256 addmod_v5(
    const uint256& x, const uint256& y, const uint256& mod) noexcept
{
    const auto s = add_with_carry(x, y);
    if (x < mod && y < mod)
    {
        auto sum = s.value;
        if (s.carry || s.value >= mod)
            sum -= mod;
        return sum;
    }

    uint<256 + 64> n = s.value;
    n[4] = s.carry;
    return udivrem(n, mod).rem;
}

[[gnu::noinline]] static uint256 addmod_v6(
    const uint256& x, const uint256& y, const uint256& mod) noexcept
{
    const auto xm = x >= mod ? x % mod : x;
    const auto ym = y >= mod ? y % mod : y;

    const auto s = add_with_carry(xm, ym);
    auto sum = s.value;
    if (s.carry || s.value >= mod)
        sum -= mod;
    return sum;
}

[[gnu::noinline]] static uint256 addmod_v7(
    const uint256& x, const uint256& y, const uint256& mod) noexcept
{
    const auto xm = x >= mod ? x % mod : x;
    const auto ym = y >= mod ? y % mod : y;

    const auto s = add_with_carry(xm, ym);
    const auto t = sub_with_carry(s.value, mod);
    return s.carry || !t.carry ? t.value : s.value;
}

}  // namespace intx::test
