# Introduction

This project has been made for the Udacity C++ Capstone project. The application uses an OpenGL Gui to display Crypto currency data obtained from an online API.

# Build Instructions

1. git clone https://github.com/JPNZ4/CryptoWatch.git
2. cd CryptoWatch
3. git submodule init
4. git submodule update
5. mkdir build
6. cd build
7. cmake ..
8. make 

# Running Instructions

From the build directory run:
    ./CryptoWatch

# Description

The project uses GLFW and GLAD to create a window and an OpenGL context. The application then uses a library Imgui & ImPlot to display the latest cryptocurrency data obtained from the Coincap API.
The program has a loop continuosly running and polling the coincap API to update the data displayed in the widgets. There are 4 widgets on the page displaying the data. There is a Table which displays an overview of all the crypto currencies tracked on coincap. The table columns can be sorted by clicking on the header. 
There is a line plot below the table which displays one coins value since the start of 2021. You can use the select menu to change which coin is displayed.
There are two bar graphs on the right side of the application which display the top and bottom five crypto currencies in regards to the value change in percentage over the last 24 hours.
The widgets will scale in width with the application but no in height.

# Class Structure

There are three classes in addition to the main file

## CustomGLFWWindow
Handles the creation of the window and the OpenGL context that is used for the application.
## Data
The data class handles requesting data from the coincap API and formatting it for use.
## ImGuiLayer
Displays the different widgets using the data from the Data class.

# Expected Output

The program should display 4 widgets (table, plot, two bars graphs). The table data should automatically refresh and be sued to sort columns. The user can change which coin is viewed on the plot.

# Rubric Points Addressed

