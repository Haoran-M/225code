import turtle
import csv

# Read the CSV file
data = []
with open('test.csv', 'r') as file:
    reader = csv.DictReader(file)
    for row in reader:
        data.append(row)

# Filter the data for Bubble Sort
bubble_sort_data = [row for row in data if row['Sort Name'] == 'Bubble sort']

# Extract the relevant columns
x = [int(row['Vector_Size']) for row in bubble_sort_data]
cpu_time = [float(row['CPU Time (s)']) for row in bubble_sort_data]

# Set up the turtle window
win = turtle.Screen()
win.title("Bubble Sort CPU Time")
win.bgcolor("white")

# Set up the turtle
t = turtle.Turtle()
t.speed(0)  # Set the drawing speed (0 is the fastest)

# Define the maximum and minimum values for x and y
max_x = max(x)
min_x = min(x)
max_y = max(cpu_time)
min_y = min(cpu_time)

# Define the width and height of the graph area
width = 600
height = 400

# Define the scale factors for x and y
x_scale = width / (max_x - min_x)
y_scale = height / (max_y - min_y)

# Draw the x-axis
t.penup()
t.goto(-width / 2, -height / 2)
t.pendown()
t.forward(width)

# Draw the y-axis
t.penup()
t.goto(-width / 2, -height / 2)
t.pendown()
t.left(90)
t.forward(height)

# Draw the data points
t.penup()
t.goto((x[0] - min_x) * x_scale - width / 2, (cpu_time[0] - min_y) * y_scale - height / 2)
t.pendown()
t.color("red")
for i in range(1, len(x)):
    t.goto((x[i] - min_x) * x_scale - width / 2, (cpu_time[i] - min_y) * y_scale - height / 2)

# Hide the turtle
t.hideturtle()

# Exit on click
turtle.exitonclick()
