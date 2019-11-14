
#include <gtest/gtest.h>
#include <input/input.hpp>

#include <sstream>
#include <string>
#include <memory>

using std::stringstream;
using std::string;
using std::make_shared;
using std::shared_ptr;
using std::istream;
using std::ostream;

/**
 * Class for testing input.
 */
class InputTest : public ::testing::Test {
protected:
    InputTest() {
        // Create streams
        input_stream = make_shared<stringstream>();
        output_stream = make_shared<stringstream>();

        // Initialize input class
        input::set_input(input_stream);
        input::set_output(output_stream);
    }

    shared_ptr<stringstream> input_stream;
    shared_ptr<stringstream> output_stream;
};

TEST_F(InputTest, BasicInput) {
    int value = 45;
    *input_stream << 45;
    int response = input::prompt<int>("Enter a number");

    ASSERT_EQ(value, response) << "Inserted a value.";
}

TEST_F(InputTest, VariousInput) {
    // Value to insert
    int int_value = 455;
    char char_value = 'c';
    double double_value = 4.5;
    string string_value = "This is a test";

    *input_stream << int_value << char_value << double_value << string_value;

    // Read values
    EXPECT_EQ(input::prompt<int>(""), int_value);
    EXPECT_EQ(input::prompt<char>(""), char_value);
    EXPECT_EQ(input::prompt<double>(""), double_value);
    EXPECT_EQ(input::prompt<string>(""), string_value);
}

TEST_F(InputTest, OutputTest) {
    string message = "This is a message:\n";
    string response = "This is a response";

    *input_stream << response;
    EXPECT_EQ(input::prompt<string>(message), response) << "Input message does not match.";
    EXPECT_EQ(output_stream->str(), message) << "Output message does not match.";
}

TEST_F(InputTest, InvalidTest) {
    int value = 50;
    *input_stream << "Invalid" << std::endl << "value" << std::endl << 100 << std::endl << value;

    EXPECT_EQ(input::prompt_range<int>("", 10, 70, true), value) << "Should get final value.";

    string output = output_stream->str();
    EXPECT_TRUE(output.find("Error") != output.npos)
                        << "'Error' should be inside the string. Output: "
                        << output;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
