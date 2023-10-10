#pragma once

#include <functional>

template <typename T>
using comparator = std::function<bool(const T&, const T&)>;

template <typename T>
void sift_down(std::vector<T>& vec, const size_t idx_elem, const comparator<T>& comparator,
               const size_t heap_border_offset = 0)
{
    const size_t heap_size = vec.size() - heap_border_offset;

    // sifting is not possible for empty or 1-sized vector
    if (heap_size <= 1)
    {
        return;
    }

    const size_t idx_last_parent = (heap_size - 2) / 2;
    size_t idx_current = idx_elem;

    while (idx_current <= idx_last_parent)
    {
        size_t idx_left_child = idx_current * 2 + 1;
        size_t idx_right_child = idx_current * 2 + 2;

        // determine the child node for swapping (min for min heap, max for max heap)
        size_t idx_next = idx_current;

        if (idx_left_child < heap_size && comparator(vec[idx_next], vec[idx_left_child]))
        {
            idx_next = idx_left_child;
        }
        if (idx_right_child < heap_size && comparator(vec[idx_next], vec[idx_right_child]))
        {
            idx_next = idx_right_child;
        }

        // if found child to swap with (max heap -> cur < max child, min heap -> cur > min child)
        if (idx_next != idx_current)
        {
            // swap the nodes
            std::swap(vec[idx_current], vec[idx_next]);
            // focus on the next node
            idx_current = idx_next;
        }
        // else means current node has correct position -> stop sifting
        else
        {
            break;
        }
    }
}

template <typename T>
void make_heap(std::vector<T>& vec, const comparator<T>& comparator)
{
    // empty or 1-sized vector is already a valid min of max heap
    if (vec.size() <= 1)
    {
        return;
    }

    // start from the last parent node ((i - 1) / 2 where is is last element) and go to the root of the heap
    // sift down every of these nodes
    for (size_t idx_parent = (vec.size() - 2) / 2; idx_parent != SIZE_MAX; --idx_parent)
    {
        sift_down(vec, idx_parent, comparator);
    }
}

template <typename T>
void sort_heap(std::vector<T>& vec, const comparator<T>& comparator)
{
    for (size_t heap_border_offset = 1; heap_border_offset < vec.size(); ++heap_border_offset)
    {
        std::swap(vec[0], vec[vec.size() - heap_border_offset]);
        sift_down(vec, 0, comparator, heap_border_offset);
    }
}

template <typename T>
void heap_sort(std::vector<T>& vec, bool asc = true)
{
    // empty or 1-sized vector is already sorted
    if (vec.size() <= 1)
    {
        return;
    }

    // create comparator functions for ascending or descending order
    auto comparator = asc
                          ? std::function<bool(const T&, const T&)>(
                              [](const T& l, const T& r) -> bool { return l < r; })
                          : std::function<bool(const T&, const T&)>(
                              [](const T& l, const T& r) -> bool { return l > r; });

    // make a valid heap out of input vector
    make_heap(vec, comparator);
    // sort the vector with help 
    sort_heap(vec, comparator);
}
