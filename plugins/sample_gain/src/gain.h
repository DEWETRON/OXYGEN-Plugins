// The calculation and its settings. No ODK or Oxygen code here, so it can be unit tested.
// Rules: ../spec/gain.md
#pragma once

#include <cmath>

namespace sample_gain
{
    // GAIN-3
    constexpr double DEFAULT_GAIN = 1.0;
    constexpr double DEFAULT_OFFSET = 0.0;

    // GAIN-3, GAIN-4: any finite number is a valid gain or offset
    inline bool isValidSetting(double value)
    {
        return std::isfinite(value);
    }

    // GAIN-4: an invalid new value keeps the previous one
    inline double acceptSetting(double candidate, double previous)
    {
        return isValidSetting(candidate) ? candidate : previous;
    }

    // GAIN-2, GAIN-5: NaN and infinity follow normal floating-point arithmetic
    inline double apply(double input, double gain, double offset)
    {
        return gain * input + offset;
    }
}
