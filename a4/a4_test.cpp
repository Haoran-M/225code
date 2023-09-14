#include "a4_base.h"
#include "a4_sort_implementations.h"
#include "test.h"
#include <cassert>
#include <string>
#include <fstream>

using namespace std;



int main() {
    const int min_n = 2000;
    const int max_n = 50000;
    const int step = 2000;

    ofstream output_file("a4.csv");  // Output file to store the sorting results

    output_file << "Sort Name, Vector Size, Number of Comparisons, CPU Time (s)" << endl;

    for (int n = min_n; n <= max_n; n += step) {
        vector<int> numbers = rand_vec(n, 1, n);

        SortStats stats;

        // Bubble sort
        stats = bubble_sort(numbers);
        output_file << stats.to_csv() << endl;
    }

    for (int n = min_n; n <= max_n; n += step) {
        vector<int> numbers = rand_vec(n, 1, n);

        SortStats stats;

        // Insertion sort
        stats = insertion_sort(numbers);
        output_file << stats.to_csv() << endl;
    }

    for (int n = min_n; n <= max_n; n += step) {
        vector<int> numbers = rand_vec(n, 1, n);

        SortStats stats;

        // Selection sort
        stats = selection_sort(numbers);
        output_file << stats.to_csv() << endl;
    }

    for (int n = min_n; n <= max_n; n += step) {
        vector<int> numbers = rand_vec(n, 1, n);

        SortStats stats;

        // Merge sort
        stats = merge_sort(numbers);
        output_file << stats.to_csv() << endl;
    }

    for (int n = min_n; n <= max_n; n += step) {
        vector<int> numbers = rand_vec(n, 1, n);

        SortStats stats;

        // Quick sort
        stats = quick_sort(numbers);
        output_file << stats.to_csv() << endl;
    }

    for (int n = min_n; n <= max_n; n += step) {
        vector<int> numbers = rand_vec(n, 1, n);

        SortStats stats;

        // Shell sort
        stats = shell_sort(numbers);
        output_file << stats.to_csv() << endl;
    }

    for (int n = min_n; n <= max_n; n += step) {
        vector<int> numbers = rand_vec(n, 1, n);

        SortStats stats;

        // Improved Quick sort
        stats = iquick_sort(numbers);
        output_file << stats.to_csv() << endl;
    }

    output_file.close();

    return 0;
}
