#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "calculator/calculator.hpp"

using namespace ::testing;

TEST(CalculatorTest, parse_numbers)
{
    auto input = std::string{"12.555"};
    auto r = parse_numbers(input.begin(), input.end());
    ASSERT_TRUE(r);
}

TEST(CalculatorTest, parse_complex)
{
    auto input = std::string{"(12.555, 22.1)"};
    std::complex<double> c;
    auto r = parse_complex(input.begin(), input.end(), c);
    ASSERT_TRUE(r);
    ASSERT_EQ(c.real(), 12.555);
    ASSERT_EQ(c.imag(), 22.1);
}

TEST(CalculatorTest, parse_doubles)
{
    auto input = std::string{"12.5, 125.2, 1.1, 52.12"};
    std::vector<double> v;
    auto r = parse_doubles(input.begin(), input.end(), v);
    ASSERT_TRUE(r);
    ASSERT_THAT(v, ElementsAre(12.5, 125.2, 1.1, 52.12));
}
