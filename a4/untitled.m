% Read the CSV file
data = readtable('test.csv');

% Filter the data for Bubble Sort, Insertion Sort, and Selection Sort
bubble_sort_data = data(strcmp(data.SortName, 'Bubble sort'), :);
insertion_sort_data = data(strcmp(data.SortName, 'Insertion sort'), :);
selection_sort_data = data(strcmp(data.SortName, 'Selection sort'), :);

% Extract the relevant columns
x = bubble_sort_data.VectorSize;
bubble_sort_time = bubble_sort_data.CPUTime_s;
insertion_sort_time = insertion_sort_data.CPUTime_s;
selection_sort_time = selection_sort_data.CPUTime_s;

% Create the plot
plot(x, bubble_sort_time, 'o-', 'DisplayName', 'Bubble Sort');
hold on;
plot(x, insertion_sort_time, 'o-', 'DisplayName', 'Insertion Sort');
plot(x, selection_sort_time, 'o-', 'DisplayName', 'Selection Sort');
hold off;

% Set the plot title and labels
title('CPU Time Comparison');
xlabel('Vector Size');
ylabel('CPU Time (s)');

% Add a legend
legend('Location', 'northwest');

% Display the plot
grid on;
