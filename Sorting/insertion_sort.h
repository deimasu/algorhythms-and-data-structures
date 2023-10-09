#pragma once

template <typename T>
void insertion_sort(std::vector<T>& vec, bool asc=true)
{
    // let the 0th element be "sorted" part. Iterating over unsorted part
    for (size_t unsorted_idx = 1; unsorted_idx < vec.size(); ++unsorted_idx)
    {
        // iterate back through the "sorted" part to find the correct place for the current element (vec[unsorted_idx])
        for (size_t insertion_idx = unsorted_idx - 1; insertion_idx != SIZE_MAX; --insertion_idx)
        {
            // swap condition for the pair of elements
            // if ascending -> left should be greater
            // if descending -> left should be lower
            const bool should_swap = (asc && vec[insertion_idx] > vec[insertion_idx + 1]) ||
                                     (!asc && vec[insertion_idx] < vec[insertion_idx + 1]);
            
            if (should_swap)
            {
                std::swap(vec[insertion_idx], vec[insertion_idx + 1]);
            }
            // if no need to swap current element then it's already in a correct position
            else
            {
                break;
            }
        }
    }
}