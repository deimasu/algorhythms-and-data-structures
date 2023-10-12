#pragma once
#include <iterator>

template <typename RandomIt>
void selection_sort(RandomIt begin, RandomIt end, bool asc = true)
{
    using T = typename std::iterator_traits<RandomIt>::value_type;

    // check for extreme elements
    // if ascending -> find min value
    // if descending -> find max value
    auto extreme_found = asc
                             ? [](const T& l, const T& r) -> bool { return l < r; }
                             : [](const T& l, const T& r) -> bool { return l > r; };

    // let the array is "unsorted". iterate over all elements
    for (auto elem_unsorted = begin; elem_unsorted < end; ++elem_unsorted)
    {
        // define element and index for swap
        RandomIt elem_extreme = elem_unsorted;
        // iterate over the rest of the array to the right
        // searching for extreme to swap it with the first unsorted element (vec[idx_unsorted])
        for (auto elem_search = elem_unsorted + 1; elem_search < end; ++elem_search)
        {
            if (extreme_found(*elem_search, *elem_extreme))
            {
                // record the extreme
                elem_extreme = elem_search;
            }
        }
        std::swap(*elem_extreme, *elem_unsorted);
    }
}
