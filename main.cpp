#include <input/input.hpp>
#include <maze/maze.hpp>

#include <iostream>

#include <sstream>
#include <memory>

using input::prompt_range;

int main() {
    // Welcome message
    std::cout << "Hello, Welcome to the Maze Generator!" << std::endl;
    std::cout << "This program will generate a randomized 2D Orthogonal Perfect maze." << std::endl;

    // Dummy input for testing
//    std::shared_ptr<std::stringstream> dummy_input = std::make_shared<std::stringstream>();
//    input::set_input(dummy_input);
//    *dummy_input << 50 << std::endl << 50 << std::endl;

    // Get dimensions
    int rows = prompt_range<int>("Input amount of rows (2, 50)> ", 2, 50);
    int cols = prompt_range<int>("Input amount of columns (2, 50)> ", 2, 50);
    std::cout << std::endl;

    // Create maze
//    Maze maze(rows, cols, 1000); // Create with known seed for testing purposes
    Maze maze(rows, cols);

    std::cout << maze << std::endl;

    return 0;
}