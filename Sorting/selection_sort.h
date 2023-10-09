#pragma once

template <typename T>
void selection_sort(std::vector<T>& vec, bool asc=true)
{
    // define extreme element here to save allocation time
    T elem_extreme;
    // let the array is "unsorted". iterate over all elements
    for (size_t idx_unsorted = 0; idx_unsorted < vec.size(); ++idx_unsorted)
    {
        // define element and index for swap
        elem_extreme = vec[idx_unsorted];
        size_t idx_extreme = idx_unsorted;
        // iterate over the rest of the array to the right
        // searching for extreme to swap it with the first unsorted element (vec[idx_unsorted])
        for (size_t idx_search = idx_unsorted + 1; idx_search < vec.size(); ++idx_search)
        {
            // check for extreme elements
            // if ascending -> find min value
            // if descending -> find max value
            const bool extreme_found = (asc && vec[idx_search] < elem_extreme) ||
                                       (!asc && vec[idx_search] > elem_extreme);
            
            if (extreme_found)
            {
                // record the extreme
                elem_extreme = vec[idx_search];
                idx_extreme = idx_search;
            }
        }
        std::swap(vec[idx_extreme], vec[idx_unsorted]);
    }
}
