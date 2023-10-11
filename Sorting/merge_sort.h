#pragma once
#include <vector>

template <typename RandomIt, typename Comparator>
void merge_arrays(RandomIt begin_l, RandomIt end_l, RandomIt begin_r, RandomIt end_r,
                  std::vector<typename std::iterator_traits<RandomIt>::value_type>& temp_vec,
                  Comparator comparator)
{
    auto elem_l = begin_l, elem_r = begin_r;
    size_t size_temp_vec = 0;

    // loop through both arrays simultaneously
    while (elem_l < end_l && elem_r < end_r)
    {
        // get max or min from current arrays pointers and put it to the array
        if (comparator(*elem_l, *elem_r))
        {
            temp_vec[size_temp_vec] = *elem_l;
            ++elem_l;
        }
        else
        {
            temp_vec[size_temp_vec] = *elem_r;
            ++elem_r;
        }
        ++size_temp_vec;
    }

    // put the rest without comparison
    while (elem_l < end_l)
    {
        temp_vec[size_temp_vec] = *elem_l;
        ++elem_l;
        ++size_temp_vec;
    }
    while (elem_r < end_r)
    {
        temp_vec[size_temp_vec] = *elem_r;
        ++elem_r;
        ++size_temp_vec;
    }


    // put all elements from the array to range begin_l:end_r
    auto elem_both = begin_l;
    for (size_t idx_temp_vec = 0; idx_temp_vec < size_temp_vec; ++idx_temp_vec)
    {
        *elem_both = temp_vec[idx_temp_vec];
        ++elem_both;
    }
}

template <typename RandomIt, typename Comparator>
void merge_sort_impl(RandomIt begin, RandomIt end,
                     std::vector<typename std::iterator_traits<RandomIt>::value_type>& temp_vec, Comparator comparator)
{
    RandomIt middle_elem = begin + (end - begin) / 2;
    if (end - begin > 1)
    {
        merge_sort_impl(begin, middle_elem, temp_vec, comparator);
        merge_sort_impl(middle_elem, end, temp_vec, comparator);
    }
    merge_arrays(begin, middle_elem, middle_elem, end, temp_vec, comparator);
}

template <typename RandomIt>
void merge_sort(RandomIt begin, RandomIt end, bool asc = true)
{
    using T = typename std::iterator_traits<RandomIt>::value_type;
    std::vector<T> temp_vec(end - begin);

    auto comparator = asc
                          ? [](const T& l, const T& r) -> bool { return l < r; }
                          : [](const T& l, const T& r) -> bool { return r < l; };

    merge_sort_impl(begin, end, temp_vec, comparator);
}
