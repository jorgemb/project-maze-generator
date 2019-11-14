//
// Created by jorge on 11/14/2019.
//

#include "maze/maze.hpp"
#include <random>
#include <chrono>
#include <iostream>
#include <string>
#include <algorithm>
#include <tuple>
#include <utility>

Maze::Maze(int rows, int columns, long long int seed) : mRows(rows), mCols(columns) {
    // Generate seed
    long long int initial_seed = seed;
    if (seed == -1)
        initial_seed = std::chrono::system_clock::now().time_since_epoch().count();

    // Create random generator
    auto generator = std::minstd_rand0(initial_seed);

    // Calculate maze parameters
    int rooms = rows * columns;
    int walls = (columns - 1) * rows /* vertical */ + (rows - 1) * columns;

    // Create initial walls and room sets
    mWallsUp.reserve(walls);
    for (int w = 0; w != walls; ++w) {
        mWallsUp.push_back(w);
    }

    mRoomSets.reserve(rooms);
    for (int c = 0; c != rooms; ++c) {
        mRoomSets.push_back(std::set<int>({c}));
    }

    // Shuffle walls
    std::shuffle(mWallsUp.begin(), mWallsUp.end(), generator);

    // Drop walls
    for (auto wall_iter = mWallsUp.begin(); wall_iter != mWallsUp.end();) {
        // Calculate adjacent cells
        int first, second;
        std::tie(first, second) = get_adjacent(*wall_iter);

        // Get the first set that contains first
        auto first_set = std::find_if(mRoomSets.begin(), mRoomSets.end(), [first](const std::set<int>& set){
            return set.find(first) != set.end();
        });
        auto second_set = std::find_if(mRoomSets.begin(), mRoomSets.end(), [second](const std::set<int>& set){
            return set.find(second) != set.end();
        });

        // Check if wall can be deleted. This means that the second element is not in the same set.
        if (first_set->find(second) == first_set->end()) {
            // Merge sets and delete the second
            first_set->insert(second_set->begin(), second_set->end());
            mRoomSets.erase(second_set);

            // Delete wall
            wall_iter = mWallsUp.erase(wall_iter);
        } else {
            ++wall_iter;
        }
    }
}

int Maze::get_rows() const {
    return mRows;
}

int Maze::get_cols() const {
    return mCols;
}

std::ostream &operator<<(std::ostream &os, const Maze &maze) {
    // Writes the Maze to the console
    int total_rows = maze.get_rows();
    int total_cols = maze.get_cols();
    char wall = '|';
    char floor = '_';
    char blank = ' ';

    // Top wall
    os << ' ';
    for (int col = 0; col != total_cols; ++col) {
        os << "_ ";
    }
    os << std::endl;

    // Each line
    for (int row = 0; row != total_rows; ++row) {
        // First wall
        os << wall;

        // Series of walls
        for (int col = 0; col != total_cols; ++col) {
            int current = row * total_cols + col;
            int right = current + 1;
            int bottom = current + total_cols;

            // Draw floor
            if(row == total_rows-1 || maze.wall_is_up(maze.get_wall(current, bottom))){
                    os << floor;
            } else {
                os << blank;
            }

            // Draw wall
            if(col == total_cols-1 || maze.wall_is_up(maze.get_wall(current, right))) {
                os << wall;
            } else {
                os << blank;
            }
        }

        // New line
        os << std::endl;
    }

    return os;
}

std::pair<int, int> Maze::get_adjacent(int wall) const{
    // Calculate line and wall number
    int walls_per_line = (2 * mCols - 1);
    int line = wall / walls_per_line;
    int wall_number = wall % walls_per_line;

    // Check if is a wall or is a floor
    if (wall_number < mCols - 1) {
        // It's a wall. Rooms are next to each other.
        int first = (line * mCols) + wall_number;
        int second = first + 1;
        return std::make_pair(first, second);
    } else {
        // It's a floor. Room is exactly below
        int first = (line * mCols) + (wall_number - (mCols - 1));
        int second = first + mCols;
        return std::make_pair(first, second);
    }
}

int Maze::get_wall(int room_a, int room_b) const{
    if (room_a < 0 || room_b < 0 || room_a > total_rooms() || room_b > total_rooms() || room_a == room_b)
        return -1;

    // Calculate the wall that separates them.
    int first = std::min(room_a, room_b);
    int second = std::max(room_a, room_b);
    int walls_per_line = (2 * mCols - 1);
    int first_row = first / mCols;
    int first_col = first % mCols;

    // By ordering them the possibilities are:
    // - Second is next to first
    // - Second is below first
    if (first_col != (mCols - 1)) { // Check if is the right-most room
        int wall_index = first - (first_row * mCols); // Wall index in the row
        int wall = walls_per_line * first_row + wall_index;
        auto pair = get_adjacent(wall);
        if (pair.second == second) return wall;
    }

    if (first_row != (mRows - 1)) { // Check if is the bottom-most room
        int wall_index = (first - (first_row * mCols)) + (mCols - 1);
        int wall = walls_per_line * first_row + wall_index;
        auto pair = get_adjacent(wall);
        if(pair.second == second) return wall;
    }

    return -1;
}

int Maze::total_rooms() const{
    return mCols * mRows;
}

bool Maze::wall_is_up(int wall) const{
    return std::find(mWallsUp.begin(), mWallsUp.end(), wall) != mWallsUp.end();
}


