#define BOOST_TEST_MODULE sample_gain
#include <boost/test/included/unit_test.hpp>

#include "../src/gain.h"

#include <limits>

using namespace sample_gain;

namespace
{
    const double NOT_A_NUMBER = std::numeric_limits<double>::quiet_NaN();
    const double INF = std::numeric_limits<double>::infinity();
}

// GAIN-2
BOOST_AUTO_TEST_CASE(applies_gain_then_offset)
{
    BOOST_CHECK_EQUAL(apply(3.0, 2.0, 1.0), 7.0);
    BOOST_CHECK_EQUAL(apply(3.0, -2.0, 1.0), -5.0);
    BOOST_CHECK_EQUAL(apply(3.0, 0.0, 4.0), 4.0);
}

// GAIN-3
BOOST_AUTO_TEST_CASE(defaults_leave_the_input_unchanged)
{
    BOOST_CHECK_EQUAL(DEFAULT_GAIN, 1.0);
    BOOST_CHECK_EQUAL(DEFAULT_OFFSET, 0.0);
    BOOST_CHECK_EQUAL(apply(-12.5, DEFAULT_GAIN, DEFAULT_OFFSET), -12.5);
}

// GAIN-3
BOOST_AUTO_TEST_CASE(setting_limits_include_all_finite_numbers)
{
    BOOST_CHECK(isValidSetting(SETTING_MIN));
    BOOST_CHECK(isValidSetting(SETTING_MAX));
    BOOST_CHECK_LT(SETTING_MIN, -1e300);
    BOOST_CHECK_GT(SETTING_MAX, 1e300);
}

// GAIN-3, GAIN-4
BOOST_AUTO_TEST_CASE(only_finite_settings_are_valid)
{
    BOOST_CHECK(isValidSetting(0.0));
    BOOST_CHECK(isValidSetting(-1e300));
    BOOST_CHECK(!isValidSetting(NOT_A_NUMBER));
    BOOST_CHECK(!isValidSetting(INF));
    BOOST_CHECK(!isValidSetting(-INF));
}

// GAIN-4
BOOST_AUTO_TEST_CASE(invalid_setting_keeps_the_previous_value)
{
    BOOST_CHECK_EQUAL(acceptSetting(5.0, 2.0), 5.0);
    BOOST_CHECK_EQUAL(acceptSetting(NOT_A_NUMBER, 2.0), 2.0);
    BOOST_CHECK_EQUAL(acceptSetting(INF, 2.0), 2.0);
}

// GAIN-5
BOOST_AUTO_TEST_CASE(non_finite_input_follows_floating_point_arithmetic)
{
    BOOST_CHECK(std::isnan(apply(NOT_A_NUMBER, 2.0, 1.0)));
    BOOST_CHECK_EQUAL(apply(INF, 2.0, 1.0), INF);
    BOOST_CHECK_EQUAL(apply(INF, -2.0, 1.0), -INF);
    BOOST_CHECK(std::isnan(apply(INF, 0.0, 1.0)));
}
