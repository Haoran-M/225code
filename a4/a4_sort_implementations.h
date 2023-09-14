// a4_sort_implementations.h

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : <Haoran Miao>
// St.# : <301542189>
// Email: <hma98@sfu.ca>
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

#pragma once

#include "a4_base.h"
#include <algorithm> // for random_shuffle function in generate random vector


using namespace std;

//
// Put the implementations of all the functions listed in a4_base.h here.
template <typename T>
bool is_sorted(vector<T>& v) {
    if (v.empty()) {
        return true;
    }

    for (size_t i = 0; i < v.size() - 1; ++i) {
        if (v[i] > v[i + 1]) {
            return false;
        }
    }

    return true;
}



template <typename T>
SortStats bubble_sort(vector<T>& v) {
    unsigned long num_cmp = 0; // set compair counter
    clock_t start = clock();

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v.size() - 1; j++) {
            num_cmp++; // compair counter+1
            if (v[j] > v[j + 1]) {
                T temp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = temp;
            }
        }
    }

    clock_t end = clock();
    double elapsed_cpu_time_sec = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    return SortStats{"Bubble sort",
                     v.size(),
                     num_cmp,
                     elapsed_cpu_time_sec};
}


template <typename T>
SortStats insertion_sort(vector<T>& v) {
    unsigned long num_cmp = 0;
    clock_t start = clock();

    for (size_t i = 1; i < v.size(); ++i) {
        T key = v[i];
        int j = i - 1;
        while (j >= 0 && v[j] > key) {
            v[j + 1] = v[j];
            --j;
            ++num_cmp;
        }
        v[j + 1] = key;
    }

    clock_t end = clock();
    double elapsed_cpu_time_sec = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    return SortStats{"Insertion sort",
                     v.size(),
                     num_cmp,
                     elapsed_cpu_time_sec};
}

template <typename T>
SortStats selection_sort(vector<T>& v) {
    unsigned long num_cmp = 0;
    clock_t start = clock();

    for (size_t i = 0; i < v.size() - 1; ++i) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < v.size(); ++j) {
            ++num_cmp;
            if (v[j] < v[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(v[i], v[min_idx]);
        }
    }

    clock_t end = clock();
    double elapsed_cpu_time_sec = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    return SortStats{"Selection sort",
                     v.size(),
                     num_cmp,
                     elapsed_cpu_time_sec};
}

template <typename T>
void merge(vector<T>& v, vector<T>& temp, size_t left, size_t mid, size_t right, unsigned long& num_cmp) {
    size_t i = left;
    size_t j = mid + 1;
    size_t k = left;

    while (i <= mid && j <= right) {
        ++num_cmp;
        if (v[i] <= v[j]) {
            temp[k] = v[i];
            ++i;
        } else {
            temp[k] = v[j];
            ++j;
        }
        ++k;
    }

    while (i <= mid) {
        temp[k] = v[i];
        ++i;
        ++k;
    }

    while (j <= right) {
        temp[k] = v[j];
        ++j;
        ++k;
    }

    for (size_t p = left; p <= right; ++p) {
        v[p] = temp[p];
    }
}

template <typename T>
void merge_sort_helper(vector<T>& v, vector<T>& temp, size_t left, size_t right, unsigned long& num_cmp) {
    if (left < right) {
        size_t mid = left + (right - left) / 2;
        merge_sort_helper(v, temp, left, mid, num_cmp);
        merge_sort_helper(v, temp, mid + 1, right, num_cmp);
        merge(v, temp, left, mid, right, num_cmp);
    }
}

template <typename T>
SortStats merge_sort(vector<T>& v) {
    unsigned long num_cmp = 0;
    clock_t start = clock();

    vector<T> temp(v.size());
    merge_sort_helper(v, temp, 0, v.size() - 1, num_cmp);

    clock_t end = clock();
    double elapsed_cpu_time_sec = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    return SortStats{"Merge sort",
                     v.size(),
                     num_cmp,
                     elapsed_cpu_time_sec};
}


template <typename T>
size_t partition(vector<T>& v, size_t low, size_t high, unsigned long& num_cmp) {
    T pivot = v[high];
    size_t i = low - 1;

    for (size_t j = low; j < high; ++j) {
        ++num_cmp;
        if (v[j] <= pivot) {
            ++i;
            swap(v[i], v[j]);
        }
    }

    swap(v[i + 1], v[high]);
    return i + 1;
}

template <typename T>
void quick_sort_helper(vector<T>& v, int low, int high, unsigned long& num_cmp) {
    if (low < high) {
        size_t pivot_idx = partition(v, low, high, num_cmp);
        quick_sort_helper(v, low, pivot_idx - 1, num_cmp);
        quick_sort_helper(v, pivot_idx + 1, high, num_cmp);
    }
}

template <typename T>
SortStats quick_sort(vector<T>& v) {
    unsigned long num_cmp = 0;
    clock_t start = clock();

    quick_sort_helper(v, 0, v.size() - 1, num_cmp);

    clock_t end = clock();
    double elapsed_cpu_time_sec = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    return SortStats{"Quick sort",
                     v.size(),
                     num_cmp,
                     elapsed_cpu_time_sec};
}

template <typename T>
SortStats shell_sort(vector<T>& v) {
    unsigned long num_cmp = 0;
    clock_t start = clock();

    size_t n = v.size();
    for (size_t gap = n / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < n; ++i) {
            T temp = v[i];
            size_t j = i;

            while (j >= gap && v[j - gap] > temp) {
                v[j] = v[j - gap];
                j -= gap;
                ++num_cmp;
            }

            v[j] = temp;
        }
    }

    clock_t end = clock();
    double elapsed_cpu_time_sec = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    return SortStats{"Shell sort",
                     v.size(),
                     num_cmp,
                     elapsed_cpu_time_sec};
}

template <typename T>
void iquick_sort_helper(vector<T>& v, int low, int high, unsigned long& num_cmp, size_t threshold) {
    if (low < high) {
        if (high - low + 1 <= static_cast<int>(threshold)) {
            // If sub-vector is smaller than or equal to threshold, return
            return;
        }

        size_t pivot_idx = partition(v, low, high, num_cmp);
        iquick_sort_helper(v, low, pivot_idx - 1, num_cmp, threshold);
        iquick_sort_helper(v, pivot_idx + 1, high, num_cmp, threshold);
    }
}

template <typename T>
SortStats iquick_sort(vector<T>& v) {
    unsigned long num_cmp = 0;
    clock_t start = clock();
    size_t threshold = 5;

    iquick_sort_helper(v, 0, v.size() - 1, num_cmp, threshold);

    clock_t end = clock();
    double elapsed_cpu_time_sec = static_cast<double>(end - start) / CLOCKS_PER_SEC;

    return SortStats{"iQuick sort",
                     v.size(),
                     num_cmp,
                     elapsed_cpu_time_sec};
}
vector<int> rand_vec(int n, int min, int max) {
    vector<int> result;
    result.reserve(n);

    srand(static_cast<unsigned int>(time(nullptr)));

    for (int i = min; i <= max; ++i) {
        result.push_back(i);
    }

    random_shuffle(result.begin(), result.end());

    result.resize(n); // Resize the vector to the desired size

    return result;
}



