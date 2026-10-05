# Khulna Ambulance Dispatch System

A C++ project that simulates an ambulance dispatch and hospital coordination system for Khulna city. The program combines a city road graph, ambulance fleet management, patient and account handling, hospital allocation, request queues, and route planning in a single console-based application.

This project was developed as a Data Structures and Algorithms coursework project and demonstrates several classic DSA concepts in a realistic scenario.

## Overview

The system models Khulna as a weighted road network with multiple locations and hospitals. Users can:

- register patients and request an ambulance
- choose pickup and destination locations
- request normal or emergency transport
- view route information with shortest-path calculations
- track ambulance statuses and trip history
- view action logs and city analytics

Administrators can:

- manage the ambulance fleet
- add/remove hospitals
- review request queues
- inspect emergency priorities
- browse city route data and zone information
- manage account access

## Features

- City map visualization using a grid-based terminal layout
- Weighted graph of Khulna roads with shortest-path routing using Dijkstra
- Hospital selection based on patient needs and bed availability
- Emergency request handling using a min-heap priority queue
- Normal request handling using a circular queue
- Ambulance ranking by distance using sorting algorithms
- Patient and admin account storage using hash tables
- Location search using linear search, binary search, and BST
- Graph traversal through BFS and DFS
- Zone tree structure for city region organization
- Trip history and action history records
- Persistent save/load using `.dat` files

## Key Data Structures Used

- Graph / adjacency matrix for the city road network
- Dijkstra shortest path algorithm for route planning
- Hash tables for patient lookup and login accounts
- Circular queue for normal ambulance requests
- Min-heap for emergency request prioritization
- Arrays for ambulance and hospital records
- Merge sort, selection sort, and insertion sort for ranking
- Binary search tree for location-name lookup
- BFS and DFS for connectivity and traversal
- Tree structure for city zones and sub-areas

## Project Structure

- `main.cpp` — starts the program, loads data, seeds startup data, and runs the app
- `app_menu.cpp` — main menu flow for admin and user operations
- `city_graph.cpp` — city map, roads, graph, and zone definitions
- `map_view.cpp` — terminal map rendering and ambulance animation
- `dispatch.cpp` — ambulance assignment and trip execution logic
- `fleet_hospitals.cpp` — ambulance and hospital storage, ranking, and booking
- `hash_tables.cpp` — patient and account hash-table implementations
- `requests.cpp` — normal and emergency request queues
- `sort_search.cpp` — sorting and searching algorithms, BST implementation
- `traversal.cpp` — BFS, DFS, connected components, and zone tree traversal
- `trip_history.cpp` — trip log and action history management
- `common.cpp` — shared constants, input helpers, colors, and utility functions

## Default Login

The project seeds a default admin account:

- Username: `admin`
- Password: `admin123`

## Build and Run

### Linux / macOS

```bash
g++ -std=c++11 -O2 main.cpp -o ambulance_system
./ambulance_system
```

### Windows (MinGW / g++)

```bash
g++ -std=c++11 -O2 main.cpp -o ambulance_system.exe
ambulance_system.exe
```

## Runtime Notes

- The application is console-based and uses ANSI color output.
- It saves data automatically when the program exits.
- Save files are created in the project folder:
  - `khulna_fleet.dat`
  - `khulna_hospitals.dat`
  - `khulna_patients.dat`
  - `khulna_accounts.dat`
  - `khulna_trips.dat`

## Typical Use Case

1. Log in as admin or user.
2. Create or select a patient record.
3. Request ambulance service from a current location.
4. Provide emergency severity or hospital preference.
5. Review generated route and transport result.
6. Manage fleet and hospital status from the admin dashboard.

## Course Context

This project is intended to demonstrate practical implementations of DSA topics in a realistic management system. It is suitable for educational use, demonstration, and understanding how fundamental algorithmic ideas can be applied to real-world problem solving.

## License

This project is provided for academic and learning purposes.
