
#pragma once
#include "common.cpp"

struct LocationInfo
{
    const char *name;
    int row, col;
};
static const LocationInfo LOCATIONS[] = {
    {"Sonadanga", 13, 3},         // 0
    {"Fulbarigate", 19, 12},      // 1
    {"KUET", 17, 21},             // 2
    {"AbuNaserStadium", 17, 30},  // 3
    {"Daulatpur", 17, 48},        // 4
    {"Shiromoni", 19, 57},        // 5
    {"AbuNaserHospital", 15, 75}, // 6
    {"KCCOffice", 11, 12},        // 7
    {"Shibbari", 11, 30},         // 8
    {"KhulnaRailStn", 11, 39},    // 9
    {"NewMarket", 11, 57},        // 10
    {"Nirala", 11, 66},           // 11
    {"KhulnaMedical", 13, 21},    // 12
    {"KMPOffice", 9, 12},         // 13
    {"Rupsha", 13, 84},           // 14
    {"LaunchGhat", 9, 84},        // 15
    {"Fultala", 5, 84},           // 16
    {"GaziMedical", 11, 84},      // 17
    {"RupshaBridge", 3, 75},      // 18
    {"StrandRoad", 5, 75},        // 19
    {"Khalishpur", 11, 3},        // 20
    {"JuteMill", 7, 3},           // 21
    {"NewsprintMill", 3, 3},      // 22
    {"AdDinHospital", 5, 12},     // 23
    {"BhairabRiver", 3, 21},      // 24
    {"Gollamari", 5, 30},         // 25
    {"Boyra", 3, 39},             // 26
    {"KDAHousing", 3, 30},        // 27
    {"ZillaSchool", 3, 57},       // 28
    {"HadisPark", 7, 57},         // 29
    {"CityMedical", 1, 21},       // 30
    {"Mujgunni", 5, 39},          // 31
    {"Labanchora", 15, 12},       // 32
    {"Tutpara", 15, 39},          // 33
    {"RoyalMor", 7, 48},          // 34
    {"DakBangla", 13, 57},        // 35
    {"FireStation", 9, 39},       // 36
    {"Moylapota", 7, 66},         // 37
    {"ShishuHospital", 17, 57},   // 38
    {"Batiaghata", 17, 75},       // 39
};

static const int ROADS[][3] = {
    {0, 1, 900},
    {0, 7, 500},
    {0, 20, 450},
    {0, 13, 700},
    {1, 2, 600},
    {2, 3, 550},
    {3, 4, 500},
    {4, 5, 650},
    {5, 39, 700},
    {39, 6, 400},
    {6, 14, 550},
    {1, 32, 400},
    {32, 2, 700},
    {32, 20, 800},
    {32, 7, 900},
    {5, 38, 450},
    {38, 6, 800},
    {4, 38, 600},
    {38, 35, 500},
    {7, 8, 550},
    {8, 9, 600},
    {9, 10, 550},
    {10, 11, 650},
    {7, 12, 500},
    {8, 12, 450},
    {12, 2, 700},
    {7, 13, 400},
    {13, 21, 700},
    {13, 23, 650},
    {20, 21, 650},
    {21, 22, 500},
    {22, 24, 550},
    {21, 23, 500},
    {23, 24, 600},
    {23, 25, 700},
    {24, 30, 450},
    {22, 30, 700},
    {30, 27, 600},
    {27, 25, 500},
    {25, 31, 650},
    {31, 26, 400},
    {26, 28, 700},
    {28, 29, 500},
    {29, 34, 400},
    {31, 9, 800},
    {31, 34, 550},
    {26, 24, 900},
    {8, 25, 850},
    {9, 36, 350},
    {36, 8, 700},
    {36, 13, 1100},
    {36, 34, 600},
    {10, 34, 500},
    {10, 35, 700},
    {35, 33, 600},
    {33, 3, 700},
    {33, 9, 550},
    {33, 4, 650},
    {3, 8, 900},
    {11, 6, 800},
    {11, 15, 600},
    {11, 17, 550},
    {11, 37, 500},
    {14, 17, 400},
    {17, 15, 450},
    {15, 16, 700},
    {16, 18, 650},
    {18, 19, 550},
    {19, 11, 800},
    {19, 37, 500},
    {37, 16, 600},
    {14, 15, 750},
    {14, 39, 800},
    {16, 28, 1100},
    {28, 18, 900},
    {27, 31, 700},
    {34, 37, 650},
    {35, 29, 900},
    {20, 7, 500},
    {12, 32, 850},
    {24, 25, 550},
    {26, 29, 800},
};

static char nodeTagArr[MAX_NODES];
static int nodeCount = 0;
static int roadDist[MAX_NODES][MAX_NODES];
static bool hospitalFlag[MAX_NODES];

static char makeTag(int id) { return id < 26 ? (char)('A' + id) : (char)('a' + id - 26); }

static void addRoad(int u, int v, int d)
{
    if (roadDist[u][v] >= 0)
        return;
    roadDist[u][v] = roadDist[v][u] = d;
}

void initCityMap()
{
    nodeCount = sizeof(LOCATIONS) / sizeof(LOCATIONS[0]);
    for (int i = 0; i < MAX_NODES; i++)
    {
        hospitalFlag[i] = false;
        for (int j = 0; j < MAX_NODES; j++)
            roadDist[i][j] = -1;
    }
    for (int i = 0; i < nodeCount; i++)
        nodeTagArr[i] = makeTag(i);
    for (unsigned i = 0; i < sizeof(ROADS) / sizeof(ROADS[0]); i++)
        addRoad(ROADS[i][0], ROADS[i][1], ROADS[i][2]);
}

int getNodeCount() { return nodeCount; }
bool validNode(int id) { return id >= 0 && id < nodeCount; }
string nodeName(int id) { return LOCATIONS[id].name; }
char nodeTag(int id) { return nodeTagArr[id]; }
int nodeRow(int id) { return LOCATIONS[id].row; }
int nodeCol(int id) { return LOCATIONS[id].col; }
bool isHospitalNode(int id) { return hospitalFlag[id]; }
void markHospitalNode(int id)
{
    if (id >= 0 && id < MAX_NODES)
        hospitalFlag[id] = true;
}
bool hasRoad(int u, int v) { return roadDist[u][v] >= 0; }

void copyNeighbors(int u, int toArr[], int wArr[], int &n)
{
    n = 0;
    for (int v = 0; v < nodeCount; v++)
        if (roadDist[u][v] >= 0)
        {
            toArr[n] = v;
            wArr[n] = roadDist[u][v];
            n++;
        }
}

int getDegree(int u)
{
    int n = 0;
    for (int v = 0; v < nodeCount; v++)
        if (roadDist[u][v] >= 0)
            n++;
    return n;
}

void fillLocationNames(string names[], int ids[], int &count)
{
    count = nodeCount;
    for (int i = 0; i < nodeCount; i++)
    {
        names[i] = LOCATIONS[i].name;
        ids[i] = i;
    }
}

void dijkstraFrom(int source, int dist[], int prevNode[])
{
    bool done[MAX_NODES];
    for (int i = 0; i < nodeCount; i++)
    {
        dist[i] = INF_DIST;
        prevNode[i] = -1;
        done[i] = false;
    }
    if (!validNode(source))
        return;
    dist[source] = 0;
    for (int round = 0; round < nodeCount; round++)
    {
        int u = -1, best = INF_DIST + 1;
        for (int i = 0; i < nodeCount; i++)
            if (!done[i] && dist[i] < best)
            {
                best = dist[i];
                u = i;
            }
        if (u == -1)
            break;
        done[u] = true;
        int to[MAX_NODES], w[MAX_NODES], n = 0;
        copyNeighbors(u, to, w, n);
        for (int k = 0; k < n; k++)
            if (dist[u] + w[k] < dist[to[k]])
            {
                dist[to[k]] = dist[u] + w[k];
                prevNode[to[k]] = u;
            }
    }
}

int buildPath(int prevNode[], int target, int pathOut[])
{
    int tmp[MAX_PATH_LEN], count = 0;
    for (int cur = target; cur != -1 && count < MAX_PATH_LEN; cur = prevNode[cur])
        tmp[count++] = cur;
    for (int i = 0; i < count; i++)
        pathOut[i] = tmp[count - 1 - i];
    return count;
}

void printPathText(int path[], int count, int totalDist)
{
    info("Shortest route (Dijkstra):");
    for (int i = 0; i < count; i++)
    {
        color(CYAN);
        cout << nodeName(path[i]);
        color(NONE);
        if (i + 1 < count)
            cout << " -> ";
    }
    cout << "\n";
    warn("Total distance: " + toStr(totalDist) + " m");
}

void listLocations()
{
    heading("Khulna Locations");
    for (int i = 0; i < nodeCount; i++)
    {
        color(hospitalFlag[i] ? GREEN : CYAN);
        cout << "  " << nodeTag(i) << "  Node " << right << setw(2) << i << "  " << left << setw(18) << nodeName(i);
        color(NONE);
        cout << left << setw(12) << (hospitalFlag[i] ? "[HOSPITAL]" : "") << "roads=" << getDegree(i) << "\n";
    }
}

void printRoadsMatrix()
{
    heading("Road Network (adjacency matrix)");
    for (int i = 0; i < nodeCount; i++)
    {
        cout << "[" << right << setw(2) << i << "] " << left << setw(17) << nodeName(i) << "->";
        for (int j = 0; j < nodeCount; j++)
            if (roadDist[i][j] >= 0)
                cout << "  " << nodeName(j) << "(" << roadDist[i][j] << "m)";
        cout << "\n";
    }
}
