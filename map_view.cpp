

#pragma once
#include "city_graph.cpp"

const int GRID_ROWS = 22, GRID_COLS = 92, TRAJ_CAP = 8000;

static int absInt(int x) { return x < 0 ? -x : x; }
static int maxInt(int a, int b) { return a > b ? a : b; }


static bool isRoadChar(char ch) { return ch == '-' || ch == '|' || ch == '/' || ch == '\\' || ch == '+'; }



static void plotRoad(char grid[][GRID_COLS], int clr[][GRID_COLS], int r1, int c1, int r2, int c2, int roadColor, bool isRoute) {
    int steps = maxInt(absInt(r2 - r1), absInt(c2 - c1));
    int prevR = r1, prevC = c1;
    for (int s = 1; s < steps; s++) {
        int r = r1 + (r2 - r1) * s / steps, c = c1 + (c2 - c1) * s / steps;
        char ch;
        if (r != prevR && c != prevC) ch = ((r - prevR) * (c - prevC) > 0) ? '\\' : '/';
        else if (r != prevR)          ch = '|';
        else                          ch = '-';
        
        if (!isRoute && isRoadChar(grid[r][c]) && grid[r][c] != ch) ch = '+';
        grid[r][c] = ch; clr[r][c] = roadColor;
        prevR = r; prevC = c;
    }
}


static bool onRoute(int id, int route[], int routeLen) {
    for (int i = 0; i < routeLen; i++) if (route[i] == id) return true;
    return false;
}


static void renderGrid(int route[], int routeLen, int ambRow, int ambCol, bool showAmb) {
    char grid[GRID_ROWS][GRID_COLS];
    int  clr[GRID_ROWS][GRID_COLS];
    for (int r = 0; r < GRID_ROWS; r++) for (int c = 0; c < GRID_COLS; c++) { grid[r][c] = ' '; clr[r][c] = NONE; }

    
    for (int u = 0; u < getNodeCount(); u++)
        for (int v = u + 1; v < getNodeCount(); v++)
            if (hasRoad(u, v)) plotRoad(grid, clr, nodeRow(u), nodeCol(u), nodeRow(v), nodeCol(v), WHITE, false);

    
    for (int i = 0; i + 1 < routeLen; i++)
        plotRoad(grid, clr, nodeRow(route[i]), nodeCol(route[i]), nodeRow(route[i + 1]), nodeCol(route[i + 1]), YELLOW, true);

    
    for (int i = 0; i < getNodeCount(); i++) {
        int r = nodeRow(i), c = nodeCol(i);
        bool hos = isHospitalNode(i);
        grid[r][c - 1] = hos ? '[' : '(';
        grid[r][c]     = nodeTag(i);
        grid[r][c + 1] = hos ? ']' : ')';
        int col = hos ? GREEN : (onRoute(i, route, routeLen) ? YELLOW : CYAN);
        clr[r][c - 1] = clr[r][c] = clr[r][c + 1] = col;
    }

    if (showAmb) { grid[ambRow][ambCol] = '@'; clr[ambRow][ambCol] = RED; }

    for (int r = 0; r < GRID_ROWS; r++) {
        for (int c = 0; c < GRID_COLS; c++) { color(clr[r][c]); cout << grid[r][c]; }
        color(NONE);
        cout << "\n";
    }
}


static void printLegend() {
    info("  (A) location    [G] hospital    @ ambulance    - | / \\ road    + junction");
    info("  yellow = Dijkstra route");
    
}

void showGridMap() {
    heading("KHULNA CITY MAP (40 locations)");
    renderGrid(NULL, 0, -1, -1, false);
    printLegend();
}


void animateDispatch(int path[], int pathLen, int etaSeconds, const string& label) {
    if (pathLen < 1) return;
    static int trajR[TRAJ_CAP], trajC[TRAJ_CAP];
    int n = 0;
    trajR[n] = nodeRow(path[0]); trajC[n] = nodeCol(path[0]); n++;

    for (int i = 0; i + 1 < pathLen; i++) {
        int r1 = nodeRow(path[i]), c1 = nodeCol(path[i]), r2 = nodeRow(path[i + 1]), c2 = nodeCol(path[i + 1]);
        int steps = maxInt(absInt(r2 - r1), absInt(c2 - c1));
        if (steps == 0) steps = 1;
        for (int s = 1; s <= steps && n < TRAJ_CAP; s++) {
            trajR[n] = r1 + (r2 - r1) * s / steps;
            trajC[n] = c1 + (c2 - c1) * s / steps;
            n++;
        }
    }

    
    int delay = etaSeconds > 0 ? etaSeconds * 1000 / n : 80;
    if (delay < 35) delay = 35;
    if (delay > 180) delay = 180;

    for (int step = 0; step < n; step++) {
        clearScreen();
        heading("LIVE TRACKING: " + label);
        renderGrid(path, pathLen, trajR[step], trajC[step], true);
        printLegend();
        warn("Progress: " + toStr(step + 1) + " / " + toStr(n) + " steps");
        delayMs(delay);
    }
    good("Ambulance has reached the destination.");
}
