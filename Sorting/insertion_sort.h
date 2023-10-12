#pragma once
#include <iterator>

template <typename RandomIt>
void insertion_sort(RandomIt begin, RandomIt end, bool asc = true)
{
    using T = typename std::iterator_traits<RandomIt>::value_type;

    // swap condition for the pair of elements
    // if ascending -> left should be greater
    // if descending -> left should be lower
    auto should_swap = asc
                           ? [](const T& l, const T& r) -> bool { return l > r; }
                           : [](const T& l, const T& r) -> bool { return l < r; };

    // let the 0th element be "sorted" part. Iterating over unsorted part
    for (auto elem_unsorted = begin; elem_unsorted < end; ++elem_unsorted)
    {
        // iterate back through the "sorted" part to find the correct place for the current element (vec[unsorted_idx])
        for (auto elem_insertion = std::make_reverse_iterator(elem_unsorted);
             elem_insertion < std::make_reverse_iterator(begin); ++elem_insertion)
        {
            if (should_swap(*elem_insertion, *(elem_insertion - 1)))
            {
                std::swap(*elem_insertion, *(elem_insertion - 1));
            }
            // if no need to swap current element then it's already in a correct position
            else
            {
                break;
            }
        }
    }
}
