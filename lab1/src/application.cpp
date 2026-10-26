#include "../include/application.h"
#include "../include/array.h"

#include <iostream>


namespace Application {
    namespace Constants {
        constexpr int APPLICATION_STOP = 0;
        constexpr int CREATE_ARRAY = 1;
        constexpr int PRINT_ARRAY = 2;
        constexpr int INSERT_ELEMENT = 3;
        constexpr int REMOVE_ELEMENT = 4;
        constexpr int RESIZE_ARRAY = 5;
        constexpr int FIND_ELEMENT = 6;
        constexpr int SORT_ARRAY = 7;
        constexpr int FILL_ARRAY = 8;
        constexpr int REMOVE_DUPLICATES = 9;
    }

    void printProgramMenu();
    void mapUserInput(int userInput, int*& array, std::size_t& size);
    void runProgramCycle(int*& array, std::size_t& size);

    template<typename T>
    auto readLine(const char* query) {
        std::cout << query;
        T input;
        while (!(std::cin >> input)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        return input;
    }

    inline bool isArrayInvalid(int*& array) {
        if (array == nullptr) {
            std::cout << "Array is empty!\n";
            return true;
        }
        return false;

    }

    void printProgramMenu() {
        std::cout << (
            R"(1. Create array)" "\n"
            R"(2. Print array)" "\n"
            R"(3. Insert element)" "\n"
            R"(4. Remove element)" "\n"
            R"(5. Resize array)" "\n"
            R"(6. Find element)" "\n"
            R"(7. Sort array)" "\n"
            R"(8. Fill array)" "\n"
            R"(9. Remove duplicates)" "\n"
            R"(0. Exit)" "\n"
        );
    }

    void runProgramCycle(int*& array, std::size_t& size) {
        int menuItem;
        while (
            printProgramMenu(),
            (menuItem = readLine<int>("Choose an option: ")) != Constants::APPLICATION_STOP
        ) {
            mapUserInput(menuItem, array, size);
        }
    }


    void run() {
        int* array = nullptr;
        std::size_t size = 0;
        runProgramCycle(array, size);
        Array::clear(array);
    }

    void mapUserInput(int userInput, int*& array, std::size_t& size) {
        switch (userInput) {
            case Constants::CREATE_ARRAY: {
                size = readLine<std::size_t>("Input array size: ");
                Array::clear(array);
                array = Array::create(size);
                std::cout << "Array created!\n";
                break;
            }
            case Constants::PRINT_ARRAY: {
                if (isArrayInvalid(array)) {
                    break;
                }
                Array::print(array, size);
                break;
            }
            case Constants::INSERT_ELEMENT: {
                if (isArrayInvalid(array)) {
                    break;
                }
                int element = readLine<int>("Insert element value: ");
                int pos = readLine<int>("Insert position: ");
                array = Array::insert(array, size, pos, element);
                std::cout << "The element was inserted\n";
                break;
            }
            case Constants::REMOVE_ELEMENT: {
                if (isArrayInvalid(array)) {
                    break;
                }
                int pos = readLine<int>("Insert position: ");
                array = Array::remove(array, size, pos);
                std::cout << "The element was deleted\n";
                break;
            }
            case Constants::RESIZE_ARRAY: {
                if (isArrayInvalid(array)) {
                    break;
                }
                int newSize = readLine<int>("Insert new size: ");
                array = Array::resize(array, size, newSize);
                size = newSize;
                std::cout << "Array was resized\n";
                break;
            }
            case Constants::FIND_ELEMENT: {
                if (isArrayInvalid(array)) {
                    break;
                }
                std::size_t resultPos;
                int element = readLine<int>("Insert the element you are looking for: ");
                bool isContains = Array::contains(array, size, element, resultPos);
                if (isContains) {
                    std::cout << "Element " << element << " was found at index " << resultPos << '\n';
                } else {
                    std::cout << "Element not found\n";
                }
                break;
            }
            case Constants::SORT_ARRAY: {
                if (isArrayInvalid(array)) {
                    break;
                }
                Array::sort(array, size);
                std::cout << "Array was sorted\n";
                break;
            }
            case Constants::FILL_ARRAY: {
                if (isArrayInvalid(array)) {
                    break;
                }
                int lowerBound = readLine<int>("Insert lower bound: ");
                int upperBound = readLine<int>("Insert upper bound: ");
                Array::fill(array, size, lowerBound, upperBound);
                std::cout << "Array was filled\n";
                break;
            }
            case Constants::REMOVE_DUPLICATES: {
                if (isArrayInvalid(array)) {
                    break;
                }
                size = Array::removeDuplicates(array, size);
                std::cout << "Duplicates were removed\n";
                break;
            }
            default: {
                std::cout << "Invalid choice!\n";
            }
        }
    }
}
