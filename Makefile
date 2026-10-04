# Makefile for Windows (MinGW / mingw32-make)
# Only main.cpp is compiled; the other .cpp files are #included by it.

CXX      = g++
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra
TARGET   = ambulance_system.exe
SRC      = main.cpp

# Data files written at runtime
DATAFILES = khulna_fleet.dat khulna_hospitals.dat khulna_patients.dat \
            khulna_accounts.dat khulna_trips.dat

# Header-like source files that main.cpp depends on (for rebuild detection)
DEPS = app_menu.cpp city_graph.cpp common.cpp dispatch.cpp \
       fleet_hospitals.cpp hash_tables.cpp map_view.cpp requests.cpp \
       sort_search.cpp traversal.cpp trip_history.cpp

.PHONY: all run clean distclean rebuild

all: $(TARGET)

$(TARGET): $(SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	$(TARGET)

clean:
	-@del /Q $(TARGET) 2>NUL

distclean: clean
	-@del /Q $(DATAFILES) 2>NUL

rebuild: clean all