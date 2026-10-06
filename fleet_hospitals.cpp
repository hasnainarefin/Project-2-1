

#pragma once
#include "city_graph.cpp"
#include "sort_search.cpp"
#include "traversal.cpp"


static AmbulanceRecord fleet[MAX_AMBULANCES];
static int fleetCount = 0, nextAmbNo = 1;

int getFleetCount() { return fleetCount; }
AmbulanceRecord& fleetAt(int i) { return fleet[i]; }


string addAmbulance(const string& driver, const string& phone, int atNode) {
    if (fleetCount >= MAX_AMBULANCES) return "";
    AmbulanceRecord a = {makeCode("AMB", nextAmbNo++), driver, phone, atNode, ST_AVAILABLE, 0};
    fleet[fleetCount++] = a;
    return a.code;
}


int findAmbulance(const string& code) {
    for (int i = 0; i < fleetCount; i++) if (fleet[i].code == code) return i;
    return -1;
}


bool removeAmbulance(const string& code) {
    int idx = findAmbulance(code);
    if (idx == -1) return false;
    for (int i = idx; i < fleetCount - 1; i++) fleet[i] = fleet[i + 1];
    fleetCount--;
    return true;
}

void setAmbulanceStatus(const string& code, int status) { int i = findAmbulance(code); if (i != -1) fleet[i].status = status; }
void setAmbulanceNode(const string& code, int node)     { int i = findAmbulance(code); if (i != -1) fleet[i].atNode = node; }

string statusName(int s) {
    const char* names[] = {"Available", "En Route", "Arrived", "Transporting", "Maintenance"};
    return (s >= 0 && s <= 4) ? names[s] : "Unknown";
}

void listFleet() {
    heading("Ambulance Fleet");
    for (int i = 0; i < fleetCount; i++) {
        AmbulanceRecord& a = fleet[i];
        color(BLUE); cout << left << setw(8) << a.code; color(NONE);
        cout << setw(16) << a.driver << setw(14) << a.phone << "Node " << right << setw(2) << a.atNode << left << "   ";
        color(a.status == ST_AVAILABLE ? GREEN : YELLOW); cout << setw(14) << statusName(a.status); color(NONE);
        cout << "Trips: " << a.tripCount << "\n";
    }
}


void rankAmbulancesTo(int node) {
    int dist[MAX_NODES], prev[MAX_NODES];
    dijkstraFrom(node, dist, prev);
    int idx[MAX_AMBULANCES], d[MAX_AMBULANCES];
    for (int i = 0; i < fleetCount; i++) { idx[i] = i; d[i] = dist[fleet[i].atNode]; }
    insertionSortByDistance(idx, d, fleetCount);
    heading("Ambulances nearest to " + nodeName(node));
    for (int i = 0; i < fleetCount; i++)
        cout << "  " << left << setw(8) << fleet[idx[i]].code << setw(16) << fleet[idx[i]].driver << right << setw(6) << d[i] << " m   " << left << statusName(fleet[idx[i]].status) << "\n";
}

void saveFleet(const string& file) {
    ofstream f(file.c_str());
    for (int i = 0; i < fleetCount; i++)
        f << fleet[i].code << "|" << fleet[i].driver << "|" << fleet[i].phone << "|" << fleet[i].atNode << "|" << fleet[i].status << "|" << fleet[i].tripCount << "\n";
}
void loadFleet(const string& file) {
    ifstream f(file.c_str());
    string line;
    while (getline(f, line) && fleetCount < MAX_AMBULANCES) {
        if (line.empty()) continue;
        stringstream ss(line);
        AmbulanceRecord a;
        a.code = nextField(ss); a.driver = nextField(ss); a.phone = nextField(ss);
        a.atNode = atoi(nextField(ss).c_str()); a.status = atoi(nextField(ss).c_str()); a.tripCount = atoi(nextField(ss).c_str());
        fleet[fleetCount++] = a;
        if (atoi(a.code.substr(3).c_str()) >= nextAmbNo) nextAmbNo = atoi(a.code.substr(3).c_str()) + 1;
    }
}


static HospitalRecord hospitals[MAX_HOSPITALS];
static int hospitalCount = 0, nextHosNo = 1;

int getHospitalCount() { return hospitalCount; }
HospitalRecord& hospitalAt(int i) { return hospitals[i]; }


string addHospital(const string& title, int atNode, int beds, const string& specialty) {
    if (hospitalCount >= MAX_HOSPITALS) return "";
    HospitalRecord h = {makeCode("HOS", nextHosNo++), title, atNode, beds, specialty};
    hospitals[hospitalCount++] = h;
    markHospitalNode(atNode);
    return h.code;
}

int findHospital(const string& code) {
    for (int i = 0; i < hospitalCount; i++) if (hospitals[i].code == code) return i;
    return -1;
}

bool removeHospital(const string& code) {
    int idx = findHospital(code);
    if (idx == -1) return false;
    for (int i = idx; i < hospitalCount - 1; i++) hospitals[i] = hospitals[i + 1];
    hospitalCount--;
    return true;
}


bool bookBed(const string& code) {
    int i = findHospital(code);
    if (i == -1 || hospitals[i].beds <= 0) return false;
    hospitals[i].beds--;
    return true;
}

void listHospitals() {
    heading("Khulna Hospital Directory");
    for (int i = 0; i < hospitalCount; i++) {
        HospitalRecord& h = hospitals[i];
        color(GREEN); cout << left << setw(8) << h.code; color(NONE);
        cout << setw(34) << h.title << "Node " << right << setw(2) << h.atNode << left << "   Beds: " << right << setw(2) << h.beds << left << "   " << h.specialty << "\n";
    }
}


int rankHospitalsByDistance(int source, const string& specialty, int resIdx[], int resDist[], int resPath[][MAX_PATH_LEN], int resLen[]) {
    int dist[MAX_NODES], prev[MAX_NODES], count = 0;
    dijkstraFrom(source, dist, prev);
    for (int i = 0; i < hospitalCount; i++) {
        if (hospitals[i].beds <= 0) continue;
        if (!specialty.empty() && hospitals[i].specialty != specialty) continue;
        if (dist[hospitals[i].atNode] >= INF_DIST) continue;
        resIdx[count] = i; resDist[count] = dist[hospitals[i].atNode]; count++;
    }
    selectionSortByDistance(resIdx, resDist, count);
    for (int i = 0; i < count; i++) resLen[i] = buildPath(prev, hospitals[resIdx[i]].atNode, resPath[i]);   
    return count;
}


void printReachableHospitals(int src) {
    int parent[MAX_NODES], hop[MAX_NODES], order[MAX_NODES], oc = 0;
    bfsFrom(src, parent, hop, order, oc);
    heading("Hospitals reachable from " + nodeName(src));
    for (int i = 0; i < hospitalCount; i++)
        if (hop[hospitals[i].atNode] >= 0)
            cout << "  " << left << setw(8) << hospitals[i].code << setw(34) << hospitals[i].title << "hops=" << setw(3) << hop[hospitals[i].atNode] << "beds=" << hospitals[i].beds << "\n";
}

void saveHospitals(const string& file) {
    ofstream f(file.c_str());
    for (int i = 0; i < hospitalCount; i++)
        f << hospitals[i].code << "|" << hospitals[i].title << "|" << hospitals[i].atNode << "|" << hospitals[i].beds << "|" << hospitals[i].specialty << "\n";
}
void loadHospitals(const string& file) {
    ifstream f(file.c_str());
    string line;
    while (getline(f, line) && hospitalCount < MAX_HOSPITALS) {
        if (line.empty()) continue;
        stringstream ss(line);
        HospitalRecord h;
        h.code = nextField(ss); h.title = nextField(ss);
        h.atNode = atoi(nextField(ss).c_str()); h.beds = atoi(nextField(ss).c_str()); h.specialty = nextField(ss);
        hospitals[hospitalCount++] = h;
        markHospitalNode(h.atNode);
        if (atoi(h.code.substr(3).c_str()) >= nextHosNo) nextHosNo = atoi(h.code.substr(3).c_str()) + 1;
    }
}
