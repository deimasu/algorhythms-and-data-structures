#pragma once

template <typename RandomIt>
void bubble_sort(RandomIt begin, RandomIt end, bool asc = true)
{
    using T = typename std::iterator_traits<RandomIt>::value_type;

    // swap condition for the pair of elements
    // if ascending -> left should be greater
    // if descending -> left should be lower
    auto should_swap = asc
                           ? [](const T& l, const T& r) -> bool { return l > r; }
                           : [](const T& l, const T& r) -> bool { return l < r; };

    // size of the offset for every iteration is increasing by one, since we push the extreme element to the back
    for (auto unsorted_end = end; unsorted_end != begin; --unsorted_end)
    {
        bool swapped = false;
        // iterate over the unsorted part of the vector, swapping pairs towards back depending on condition
        for (auto bubble = begin + 1; bubble < unsorted_end; ++bubble)
        {
            if (should_swap(*(bubble - 1), *bubble))
            {
                std::swap(*(bubble - 1), *bubble);
                swapped = true;
            }
        }
        // if there were no swaps then the vector is already sorted
        if (!swapped)
        {
            return;
        }
    }
}
