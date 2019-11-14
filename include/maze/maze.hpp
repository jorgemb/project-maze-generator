#ifndef PROJECT_MAZEGENERATOR_MAZE_HPP
#define PROJECT_MAZEGENERATOR_MAZE_HPP

#include <vector>
#include <set>
#include <ostream>
#include <utility>

/**
 * Class for creating a random 2D Orthogonal Perfect maze.
 */
class Maze {
public:
    /**
     * Default constructor. Requires maze dimensions.
     * @param rows Rows for the maze.
     * @param columns Columns for the maze.
     * @param seed Initial seed, use -1 for random seed.
     */
    Maze(int rows, int columns, long long int seed = -1);

    // Getters
    int get_rows() const;
    int get_cols() const;

    /**
     * Returns the total amount of rooms in the maze.
     * @return
     */
    int total_rooms() const;


    /**
     * @param wall
     * @return Returns true if the given wall is up.
     */
    bool wall_is_up(int wall) const;
private:
    /**
     * Returns adjacent rooms from a wall.
     * @param wall
     * @return
     */
    std::pair<int, int> get_adjacent(int wall) const;

    /**
     * Returns the wall that separates the rooms. Or -1 if they are not separated by a wall.
     * @param room_a
     * @param room_b
     * @return
     */
    int get_wall(int room_a, int room_b) const;

    // Dimensions
    int mRows, mCols;

    // Data
    std::vector<int> mWallsUp;
    std::vector<std::set<int>> mRoomSets;

    friend std::ostream& operator<<(std::ostream& os, const Maze& maze);

};

/**
 * Output operator overload.
 * @param os
 * @param maze
 * @return
 */
std::ostream& operator<<(std::ostream& os, const Maze& maze);

#endif //PROJECT_MAZEGENERATOR_MAZE_HPP
