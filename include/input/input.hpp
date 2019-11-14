//
// Created by jorge on 13/11/2019.
//

#ifndef PROJECT_MAZEGENERATOR_INPUT_HPP
#define PROJECT_MAZEGENERATOR_INPUT_HPP

#include <iostream>
#include <string>
#include <memory>
#include <limits>

namespace input {
    static std::weak_ptr<std::istream> mInputStream;
    static std::weak_ptr<std::ostream> mOutputStream;

    static void set_input(std::weak_ptr<std::istream> input_stream) {
        mInputStream = input_stream;
    }

    static void set_output(std::weak_ptr<std::ostream> output_stream) {
        mOutputStream = output_stream;
    }

    /**
     * Tries to lock input stream.
     * @return
     */
    std::shared_ptr<std::istream> get_input() {
        std::shared_ptr<std::istream> input_ptr = mInputStream.lock();
        if (!input_ptr) {
            input_ptr = std::shared_ptr<std::istream>(&std::cin, [](void *) {}); // stdin with null deleter
        }

        return input_ptr;
    }

    /**
     * Tries to lock the output stream.
     * @return
     */
    std::shared_ptr<std::ostream> get_output() {
        std::shared_ptr<std::ostream> output_ptr = mOutputStream.lock();
        if (!output_ptr) {
            output_ptr = std::shared_ptr<std::ostream>(&std::cout, [](void *) {}); // stdout with null deleter
        }

        return output_ptr;
    }

    /**
     * Requests a value from a user. If there is an error the function will ask again.
     * @tparam T Type of value
     * @param message Message to show the user.
     * @return Value
     */
    template<class T>
    T prompt(const std::string &message, bool ask_until_ok = true) {
        T value;

        // Show message and ask input
        auto output = get_output();
        auto input = get_input();

        *output << message;
        *input >> value;

        while (ask_until_ok && input->fail()) {
            // If there is an error, try again.
            *output << "Error: Invalid entry. Try again." << std::endl;
            *output << message;

            input->clear();
            input->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            input->sync();
            *input >> value;
        }

        return std::move(value);
    }

    /**
     * Template specialization for asking strings. Reads the complete line.
     * @param message
     * @param ask_until_ok
     * @return
     */
    template<>
    std::string prompt(const std::string &message, bool ask_until_ok) {
        std::string value;

        // Show message and ask input.
        auto output = get_output();
        auto input = get_input();

        *output << message;
        std::getline(*input, value);

        return value;
    }

    /**
     * Prompts a range to the user.
     * @tparam T
     * @param message
     * @param lower_limit
     * @param upper_limit
     * @param ask_untik_ok
     * @return
     */
    template<class T>
    T prompt_range(const std::string &message, T lower_limit, T upper_limit, bool ask_until_ok = true) {
        T value;

        // Show message and ask input
        auto output = get_output();
        auto input = get_input();

        *output << message;
        *input >> value;

        while (ask_until_ok && ( input->fail() || value < lower_limit || value > upper_limit) ) {
            // If there is an error, try again.
            *output << "Error: Value must be between " << lower_limit << " and " << upper_limit << ". Try again."
                    << std::endl;
            *output << message;

            input->clear();
            input->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            input->sync();
            *input >> value;
        }

        return std::move(value);
    }


};


#endif //PROJECT_MAZEGENERATOR_INPUT_HPP
