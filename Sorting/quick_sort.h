#pragma once
#include <iterator>

// recursively sorts data structure with help of pivot and left-right pointers
template <typename RandomIt, typename Comparator>
void quick_sort_impl(RandomIt begin, RandomIt end, Comparator comparator_l, Comparator comparator_r)
{
    if (end - begin <= 1)
    {
        return;
    }

    // select the pivot - middle element of the array
    RandomIt pivot = begin + (end - begin - 1) / 2;

    // define left and right pointers to the begin and end accordingly
    RandomIt left = begin, right = end - 1;

    // while pointers haven't crossed swap left and right elements so that:
    // elements < pivot - to the left, elements > pivot - to the right of pivot (ascending)
    // vice versa for descending order
    while (left < right)
    {
        // find left element to swap (ascending - >= pivot, descending - <= pivot)
        if (comparator_l(*left, *pivot))
        {
            ++left;
            continue;
        }
        // find right element to swap (ascending - <= pivot, descending - >= pivot)
        if (comparator_r(*right, *pivot))
        {
            --right;
            continue;
        }

        // swap left and right elements
        std::swap(*left, *right);
        // if one of the elements was pivot - update pivot position
        // shift pointers to avoid infinite loops
        if (pivot == right)
        {
            pivot = left;
            --right;
        }
        else if (pivot == left)
        {
            pivot = right;
            ++left;
        }
        else
        {
            --right;
            ++left;
        }
    }

    // recursively sort sub-array to the left of the pivot
    quick_sort_impl(begin, pivot, comparator_l, comparator_r);
    // recursively sort sub-array to the right of the pivot
    quick_sort_impl(pivot + 1, end, comparator_l, comparator_r);
}

template <typename RandomIt>
void quick_sort(RandomIt begin, RandomIt end, const bool asc = true)
{
    using T = typename std::iterator_traits<RandomIt>::value_type;

    // define comparators for left and right pointer move conditions
    auto comparator_l = asc
                            ? [](const T& l, const T& r) -> bool { return l < r; }
                            : [](const T& l, const T& r) -> bool { return l > r; };
    auto comparator_r = asc
                            ? [](const T& l, const T& r) -> bool { return l > r; }
                            : [](const T& l, const T& r) -> bool { return l < r; };

    quick_sort_impl(begin, end, comparator_l, comparator_r);
}
