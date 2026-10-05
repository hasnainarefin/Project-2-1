// FILE: common.cpp | JOB: shared sizes, record structs and console helpers (no DSA here)
#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

// fixed array sizes: the whole project uses plain arrays, no vector/queue/map
const int MAX_NODES = 48, MAX_AMBULANCES = 80, MAX_HOSPITALS = 24;
const int MAX_NORMAL_Q = 100, MAX_HEAP = 100, MAX_STACK = 100, MAX_TRIPS = 500;
const int MAX_PATH_LEN = MAX_NODES, MAX_SORT = 200;
const int INF_DIST = 1000000000;

// ambulance status codes and login roles
const int ST_AVAILABLE = 0, ST_ENROUTE = 1, ST_ARRIVED = 2, ST_TRANSPORT = 3, ST_MAINTENANCE = 4;
const int ROLE_ADMIN = 0, ROLE_USER = 1;

// record structs stored in the arrays / hash tables
struct AmbulanceRecord
{
    string code, driver, phone;
    int atNode, status, tripCount;
};
struct HospitalRecord
{
    string code, title;
    int atNode, beds;
    string specialty;
};
struct PatientRecord
{
    string code, name, phone;
    int atNode;
};
struct UserAccount
{
    string username, passHash;
    int role;
    string patientCode;
};

// ANSI colour codes
const int RED = 91, GREEN = 92, YELLOW = 93, BLUE = 94, MAGENTA = 95, CYAN = 96, WHITE = 37, GRAY = 90, NONE = 0;

// turn on ANSI colours (needed only on Windows)
void enableColors()
{
#ifdef _WIN32
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    if (h != INVALID_HANDLE_VALUE && GetConsoleMode(h, &mode))
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#endif
}

void color(int c) { cout << "\033[" << c << "m"; }

void clearScreen()
{
#ifdef _WIN32
    if (system("cls"))
    {
    }
#else
    if (system("clear"))
    {
    }
#endif
}

// busy-wait delay (used by the animation)
void delayMs(int ms)
{
    clock_t start = clock();
    while ((double)(clock() - start) < ms / 7000.0 * CLOCKS_PER_SEC)
    {
    }
}

// print one coloured line
void say(int c, const string &text)
{
    color(c);
    cout << text << "\n";
    color(NONE);
}
void info(const string &t) { say(WHITE, t); }
void good(const string &t) { say(GREEN, t); }
void warn(const string &t) { say(YELLOW, t); }
void bad(const string &t) { say(RED, t); }

// clean, unboxed heading: a coloured title with a matching underline rule
// (no surrounding box -- keeps every screen lighter and less cluttered)
void heading(const string &text)
{
    int w = (int)text.size();
    if (w < 40)
        w = 40;
    cout << "\n";
    color(CYAN);
    cout << "  " << text << "\n";
    color(GRAY);
    cout << "  " << string(w, '-') << "\n";
    color(NONE);
}

void showScreen(const string &title)
{
    clearScreen();
    heading(title);
}

void menuItem(const string &key, const string &text)
{
    color(YELLOW);
    cout << "  [" << key << "] ";
    color(NONE);
    cout << text << "\n";
}

// safe input helpers
int readInt(const string &prompt)
{
    int v;
    while (true)
    {
        cout << prompt;
        if (cin >> v)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return v;
        }
        if (cin.eof())
            exit(0); // input finished (e.g. piped file) -> stop instead of looping
        bad("Please enter a whole number.");
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string &prompt)
{
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}
void waitEnter()
{
    cout << "\nPress Enter to continue...";
    cin.get();
}

string toStr(int v)
{
    ostringstream o;
    o << v;
    return o.str();
}
//making ID
string makeCode(const string &prefix, int n)
{
    ostringstream o;
    o << prefix << setw(3) << setfill('0') << n;
    return o.str();
}

string nextField(stringstream &ss)
{
    string t;
    getline(ss, t, '|');
    return t;
}
