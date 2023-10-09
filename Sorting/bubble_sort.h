#pragma once

template <typename T>
void bubble_sort(std::vector<T>& vec, bool asc=true)
{
    for (size_t offset = 0; offset < vec.size(); offset++)
    {
        bool swapped = false;
        for (size_t bubble_idx = 1; bubble_idx < vec.size() - offset; bubble_idx++)
        {
            // "== asc" is to support asc and desc sorting (t == t or f == f -> need to swap)
            if (vec[bubble_idx - 1] > vec[bubble_idx] == asc)
            {
                // make swap using temp variable
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
