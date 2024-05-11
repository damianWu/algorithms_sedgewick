#include <gtest/gtest.h>
#include "temp_read_2/temp_read.hpp"
#include "homework/homework.hpp"
//#include "utils/exception.hpp"

// Tuple used only for educational purposes
class MaxConsecutiveOnesNaiveParameterizedTestFixture
        : public ::testing::TestWithParam<std::tuple<unsigned int>> {
};


TEST(TemperatureReadingTest, ShouldReturnCorrectStringMonth) {
    // Month range [0-11]
    // Given
    int month{11};
    std::string expected{"dec"};

    // When
    std::string result{int_to_month(month)};

    // Then
    ASSERT_EQ(expected, result);
}


TEST(TemperatureReadingHomeworkTestEx2, ShouldReturnCorrectMeanValue) {
    // Given
    std::vector<Reading> r{Reading{0, 0, 10},
                           Reading{0, 0, 20},
                           Reading{0, 0, 30},
                           Reading{0, 0, 40}};

    std::vector<Reading> r2{Reading{0, 0, 10},
                            Reading{0, 0, 10},
                            Reading{0, 0, 10},
                            Reading{0, 0, 10}};

    double expected{25.0};
    double expected2{10.0};

    // When
    double actual = mean(r);
    double actual2 = mean(r2);

    // Then
    ASSERT_EQ(expected, actual);
    ASSERT_EQ(expected2, actual2);
}

TEST(TemperatureReadingHomeworkTestEx2, ShouldReturnCorrectMedianValueOrdered) {
    // Given
    std::vector<Reading> r{Reading{0, 0, 10},
                           Reading{0, 0, 20},
                           Reading{0, 0, 30}};

    double expected{20.0};

    // When
    double actual = median(r);

    // Then
    ASSERT_EQ(expected, actual);
}

TEST(TemperatureReadingHomeworkTestEx2,
     ShouldReturnCorrectMedianValueUnordered) {
    // Given
    std::vector<Reading> r{Reading{0, 0, 10},
                           Reading{0, 0, 120},
                           Reading{0, 0, 20},
                           Reading{0, 0, 420},
                           Reading{0, 0, 60},
                           Reading{0, 0, 50},
                           Reading{0, 0, 30}};

    double expected{50.0};

    // When
    double actual = median(r);

    // Then
    ASSERT_EQ(expected, actual);
}

TEST(TemperatureReadingHomeworkTestEx3,
     ShouldReturnCorrectTemperatureConvertedValueCtoF) {
    // Given
    double input_c_temperature{25.0};
    double actual{77.0};

    // When
    double result = convert_celsius_to_fahrenheit(given);

    // Then
    ASSERT_EQ(actual, result);
}
