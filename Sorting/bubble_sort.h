#pragma once

template <typename T>
void bubble_sort(std::vector<T>& vec, bool asc=true)
{
    // size of the offset for every iteration is increasing by one, since we push the extreme element to the back
    for (size_t offset = 0; offset < vec.size(); ++offset)
    {
        bool swapped = false;
        // iterate over the unsorted part of the vector, swapping pairs towards back depending on condition
        for (size_t bubble_idx = 1; bubble_idx < vec.size() - offset; ++bubble_idx)
        {
            // swap condition for the pair of elements
            // if ascending -> left should be greater
            // if descending -> left should be lower
            const bool should_swap = (asc && vec[bubble_idx - 1] > vec[bubble_idx]) ||
                                     (!asc && vec[bubble_idx - 1] < vec[bubble_idx]);
            
            if (should_swap)
            {
                std::swap(vec[bubble_idx - 1], vec[bubble_idx]);
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
