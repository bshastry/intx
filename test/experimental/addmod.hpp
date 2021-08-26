// intx: extended precision integer library.
// Copyright 2021 Pawel Bylica.
// Licensed under the Apache License, Version 2.0.
#pragma once

#include <intx/intx.hpp>

namespace intx::test
{
[[maybe_unused, gnu::noinline]] static uint256 addmod_public(
    const uint256& x, const uint256& y, const uint256& mod) noexcept
{
    return addmod(x, y, mod);
}

[[maybe_unused, gnu::noinline]] static uint256 addmod_simple(
    const uint256& x, const uint256& y, const uint256& mod) noexcept
{
    const auto s = add_with_carry(x, y);
    uint<256 + 64> n = s.value;
    n[4] = s.carry;
    return udivrem(n, mod).rem;
}

[[maybe_unused, gnu::noinline]] static uint256 addmod_v2(
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

[[maybe_unused, gnu::noinline]] static uint256 addmod_v4(
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

[[maybe_unused, gnu::noinline]] static uint256 addmod_v5(
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

[[maybe_unused, gnu::noinline]] static uint256 addmod_v6(
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

[[maybe_unused, gnu::noinline]] static uint256 addmod_v7(
    const uint256& x, const uint256& y, const uint256& mod) noexcept
{
    const auto xm = x >= mod ? x % mod : x;
    const auto ym = y >= mod ? y % mod : y;

    const auto s = add_with_carry(xm, ym);
    const auto t = sub_with_carry(s.value, mod);
    return s.carry || !t.carry ? t.value : s.value;
}

[[maybe_unused, gnu::noinline]] static uint256 addmod_daosvik(
    const uint256& x, const uint256& y, const uint256& m) noexcept
{
    // Fast path for m >= 2^192, with x and y at most slightly bigger than m.
    // This is always the case when x and y are already reduced modulo such m.

    if ((m[3] != 0) && (x[3] <= m[3]) && (y[3] <= m[3]))
    {
        auto s = sub_with_carry(x, m);
        if (s.carry)
            s.value = x;

        auto t = sub_with_carry(y, m);
        if (t.carry)
            t.value = y;

        s = add_with_carry(s.value, t.value);
        if (s.carry)
            s.value -= m;

        t = sub_with_carry(s.value, m);
        if (t.carry)
            return s.value;
        else
            return t.value;
    }

    const auto s = add_with_carry(x, y);
    uint<256 + 64> n = s.value;
    n[4] = s.carry;
    return udivrem(n, m).rem;
}

}  // namespace intx::test
