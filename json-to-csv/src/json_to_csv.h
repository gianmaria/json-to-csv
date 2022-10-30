#pragma once

#include "pch.h"

struct ISO_Duration
{
    uint32_t Y;
    uint32_t Month;
    uint32_t W;
    uint32_t D;

    uint32_t H;
    uint32_t Min;
    uint32_t S;

    uint32_t to_min() const noexcept;
    string to_str() const noexcept;
};

ISO_Duration parse_ISO_duration(const string& duration);

std::tuple<string, string, string> UTC_to_excel(const std::string& input);
