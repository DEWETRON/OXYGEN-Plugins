// The calculation and its settings. No ODK or Oxygen code here, so it can be unit tested.
// Rules: ../spec/gain.md
#pragma once

#include <cmath>
#include <limits>

namespace sample_gain
{
    // GAIN-3
    constexpr double DEFAULT_GAIN = 1.0;
    constexpr double DEFAULT_OFFSET = 0.0;

    // GAIN-3: the limits of gain and offset, which are all finite numbers.
    // Oxygen needs them: a number without a min/max range cannot be edited in its views.
    constexpr double SETTING_MIN = std::numeric_limits<double>::lowest();
    constexpr double SETTING_MAX = std::numeric_limits<double>::max();

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
