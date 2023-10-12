#pragma once
#include <iterator>

// places given element elem_to_sift to the correct place in the heap
template <typename RandomIt, typename Comp>
void sift_down(RandomIt begin, RandomIt end, RandomIt elem_to_sift,
               const Comp& comparator)
{
    const auto last_parent = begin + (end - begin - 2) / 2;

    // sifting is possible only for elements that have at least one child
    // hence stopping when current node is not a parent
    for (auto current = elem_to_sift; current <= last_parent;)
    {
        auto left_child = begin + (current - begin) * 2 + 1;
        auto right_child = begin + (current - begin) * 2 + 2;

        // determine the child node for swapping (min for min heap, max for max heap)
        auto idx_next = current;

        if (left_child < end && comparator(*idx_next, *left_child))
        {
            idx_next = left_child;
        }
        if (right_child < end && comparator(*idx_next, *right_child))
        {
            idx_next = right_child;
        }

        // if found child to swap with (max heap -> cur < max child, min heap -> cur > min child)
        if (idx_next != current)
        {
            // swap the nodes
            std::swap(*current, *idx_next);
            // focus on the next node
            current = idx_next;
        }
        // else means current node has correct position -> stop sifting
        else
        {
            break;
        }
    }
}

// creates a min or max heap based on the passed comparator
template <typename RandomIt, typename Comp>
void build_heap(RandomIt begin, RandomIt end,
                const Comp& comparator)
{
    // start from the last parent node ((i - 1) / 2 where is is last element) and go to the root of the heap
    // sift down every of these nodes
    for (auto parent = begin + (end - begin - 2) / 2; parent != begin; --parent)
    {
        sift_down(begin, end, parent, comparator);
    }
    sift_down(begin, end, begin, comparator);
}

// sorts a vector that is a valid min or max heap
// the vector should be prepared with make_heap function first
template <typename RandomIt, typename Comp>
void sort_with_heap(RandomIt begin, RandomIt end,
                    const Comp& comparator)
{
    // after each swap decrease heap size within the array
    for (auto heap_end = end - 1; heap_end > begin; --heap_end)
    {
        // swap the root of the heap (max or min element depending on order) with the latest element (sorted vector part)
        std::swap(*begin, *heap_end);
        // sift down the new root to the correct position in the heap
        sift_down(begin, heap_end, begin, comparator);
    }
}

template <typename RandomIt>
void heap_sort(RandomIt begin, RandomIt end, bool asc = true)
{
    using T = typename std::iterator_traits<RandomIt>::value_type;

    if (begin == end || begin + 1 == end)
    {
        return;
    }

    // create comparator functions for ascending or descending order
    auto comparator = asc
                          ? [](const T& l, const T& r) -> bool { return l < r; }
                          : [](const T& l, const T& r) -> bool { return l > r; };

    build_heap(begin, end, comparator);
    sort_with_heap(begin, end, comparator);
}
