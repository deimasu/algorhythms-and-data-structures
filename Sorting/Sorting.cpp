#include <iostream>
#include <vector>

#include "bubble_sort.h"

int main(int argc, char* argv[])
{
    std::vector<int> vec{55, 3, 80, 1, 13, 4, 78, 94, 10, 88, 4, 78, 33};

    bubble_sort(vec);
    
    for (auto el : vec)
    {
        std::cout << el << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
