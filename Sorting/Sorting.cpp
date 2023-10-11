#include <iostream>
#include <vector>
#include <utility>

#include "insertion_sort.h"
#include "bubble_sort.h"
#include "selection_sort.h"
#include "heap_sort.h"

int main(int argc, char* argv[])
{
    std::vector<int> vec{55, 3, 80, 13, 4, 78, 94, 10, 88, 4, 78, 33, 1};

    // bubble_sort(vec.begin(), vec.end());
    // insertion_sort(vec.begin(), vec.end());
    // selection_sort(vec.begin(), vec.end());
    heap_sort(vec.begin(), vec.end());
    for (auto el : vec)
    {
        std::cout << el << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
