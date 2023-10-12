#include <iostream>
#include <vector>

#include "insertion_sort.h"
#include "bubble_sort.h"
#include "selection_sort.h"
#include "heap_sort.h"
#include "merge_sort.h"
#include "quick_sort.h"

template <typename RandomIt>
bool is_sorted(RandomIt begin, RandomIt end, bool asc = true)
{
    if (begin == end)
    {
        return true;
    }

    for (auto elem = begin + 1; elem < end; ++elem)
    {
        if (asc && *elem < *(elem - 1) || !asc && *elem > *(elem - 1))
        {
            return false;
        }
    }

    return true;
}

template <typename T, typename SortFunc>
void check_sorting(std::vector<std::vector<T>> test_data, SortFunc& sort_func, bool asc)
{
    std::cout << (asc ? "ASC" : "DESC") << std::endl;
    
    bool errors = false;
    for (auto test_array : test_data)
    {
        std::vector<T> cpy = test_array;
        sort_func(test_array.begin(), test_array.end(), asc);

        const bool result = is_sorted(test_array.begin(), test_array.end(), asc);
        if (!result)
        {
            std::cout << "Failed to sort array:" << std::endl;
            for (auto el : cpy)
            {
                std::cout << el << ' ';
            }
            std::cout << std::endl;
            std::cout << "Actual result:" << std::endl;

            for (auto el : test_array)
            {
                std::cout << el << ' ';
            }
            std::cout << std::endl << std::endl;

            errors = true;
        }
    }
    if (!errors)
    {
        std::cout << "All test cases passed" << std::endl;
    }
}

int main(int argc, char* argv[])
{
    const std::vector<std::vector<int>> test_data{
        {55, 3, 80, 13, 4, 78, 94, 10, 88, 4, 78, 33, 1},
        {3, -1, 4, -1, 5, -9, 2, -6, 5},
        {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5},
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
        {5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
        {5},
        {5, 5},
        {5, 6},
        {6, 5},
        {1, 2, 3, 4, 5, 5, 5, 5, 6, 7, 8, 9, 10},
        {10, 9, 8, 7, 6, 5, 5, 5, 5, 4, 3, 2, 1},
        {1, 10, 2, 9, 3, 8, 4, 7, 5, 6},
        {10, 1, 9, 2, 8, 3, 7, 4, 6, 5},
        {1, 2, 3, 4, 5, 10, 9, 8, 7, 6},
        {10, 9, 8, 7, 6, 1, 2, 3, 4, 5},
        {1, 3, 2, 5, 4, 7, 6, 9, 8, 10},
        {10, 8, 9, 6, 7, 4, 5, 2, 3, 1},
        {1, 3, 2, 5, 4, 5, 6, 9, 8, 10},
        {10, 8, 9, 6, 7, 5, 4, 2, 3, 1},
        {1, 100, 2, 99, 3, 98, 4, 97, 5, 96},
        {100, 1, 99, 2, 98, 3, 97, 4, 96, 5},
        {1, 100, 2, 99, 3, 98, 3, 97, 4, 96}
    };

    std::cout << std::boolalpha;

    std::cout << "Bubble sort:" << std::endl;
    check_sorting(test_data, bubble_sort<std::vector<int>::iterator>, true);
    check_sorting(test_data, bubble_sort<std::vector<int>::iterator>, false);
    std::cout << "-----------------------------------" << std::endl << std::endl;
    
    std::cout << "Insertion sort:" << std::endl;
    check_sorting(test_data, insertion_sort<std::vector<int>::iterator>, true);
    check_sorting(test_data, insertion_sort<std::vector<int>::iterator>, false);
    std::cout << "-----------------------------------" << std::endl << std::endl;

    std::cout << "Selection sort:" << std::endl;
    check_sorting(test_data, selection_sort<std::vector<int>::iterator>, true);
    check_sorting(test_data, selection_sort<std::vector<int>::iterator>, false);
    std::cout << "-----------------------------------" << std::endl << std::endl;

    std::cout << "Heap sort:" << std::endl;
    check_sorting(test_data, heap_sort<std::vector<int>::iterator>, true);
    check_sorting(test_data, heap_sort<std::vector<int>::iterator>, false);
    std::cout << "-----------------------------------" << std::endl << std::endl;

    std::cout << "Merge sort:" << std::endl;
    check_sorting(test_data, merge_sort<std::vector<int>::iterator>, true);
    check_sorting(test_data, merge_sort<std::vector<int>::iterator>, false);
    std::cout << "-----------------------------------" << std::endl << std::endl;

    std::cout << "Quick sort:" << std::endl;
    check_sorting(test_data, quick_sort<std::vector<int>::iterator>, true);
    check_sorting(test_data, quick_sort<std::vector<int>::iterator>, false);
    std::cout << "-----------------------------------" << std::endl << std::endl;
    

    return 0;
}
