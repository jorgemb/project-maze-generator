
#include <gtest/gtest.h>
#include <input/input.hpp>

#include <sstream>

using std::stringstream;

/**
 * Class for testing input.
 */
class InputTest: ::testing::Test{
protected:
    InputTest(): input(input_stream, output_stream){

    }

    stringstream input_stream, output_stream;
    Input input;
};

int main(int argc, char** argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
