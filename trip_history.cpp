// FILE: trip_history.cpp | JOB: remember finished trips and the recent admin actions
// DSA: Singly Linked List (trips, newest at head), Stack (action log, last in first out), Merge sort for the report
#pragma once
#include "common.cpp"
#include "sort_search.cpp"

// ================= TRIPS: singly linked list =================
struct TripNode { string tripCode, patientCode, ambulanceCode, hospitalCode; int fromNode, toNode, meters; TripNode* next; };
static TripNode* tripHead = NULL;
static int nextTripNo = 1;

// Linked list: insert at the head, so the newest trip is always first
void logTrip(const string& patient, const string& amb, const string& hospital, int fromNode, int toNode, int meters) {
    tripHead = new TripNode{makeCode("TRP", nextTripNo++), patient, amb, hospital, fromNode, toNode, meters, tripHead};
}

// Linked list traversal: walk head -> NULL
void showAllTrips() {
    heading("Complete Trip History (linked list)");
    for (TripNode* t = tripHead; t != NULL; t = t->next)
        cout << t->tripCode << " | " << t->patientCode << " | " << t->ambulanceCode << " | " << t->hospitalCode << " | "
             << right << setw(2) << t->fromNode << " -> " << setw(2) << t->toNode << " | " << setw(6) << t->meters << " m" << left << "\n";
}

// Linked list traversal with a filter: only this patient's trips
void showTripsForPatient(const string& patientCode) {
    heading("Your Trip History");
    bool any = false;
    for (TripNode* t = tripHead; t != NULL; t = t->next)
        if (t->patientCode == patientCode) {
            any = true;
            cout << t->tripCode << " | " << t->ambulanceCode << " | " << t->hospitalCode << " | " << right << setw(6) << t->meters << " m" << left << "\n";
        }
    if (!any) info("No trips yet.");
}

// Report: copy the list into arrays, merge sort by distance, print shortest trip first
void showTripsSortedByDistance() {
    string codes[MAX_TRIPS];
    int dist[MAX_TRIPS], idx[MAX_TRIPS], n = 0;
    for (TripNode* t = tripHead; t != NULL && n < MAX_TRIPS; t = t->next) { codes[n] = t->tripCode; dist[n] = t->meters; idx[n] = n; n++; }
    heading("Trips sorted by distance (merge sort)");
    if (n == 0) { info("No trips yet."); return; }
    mergeSortByDistance(idx, dist, n);
    for (int i = 0; i < n; i++) cout << "  " << codes[idx[i]] << "  " << right << setw(6) << dist[i] << " m" << left << "\n";
}

// file is written newest-first, so loading reads the whole file into a buffer and inserts from the last line back to the first
void saveTrips(const string& file) {
    ofstream f(file.c_str());
    for (TripNode* t = tripHead; t != NULL; t = t->next)
        f << t->tripCode << "|" << t->patientCode << "|" << t->ambulanceCode << "|" << t->hospitalCode << "|" << t->fromNode << "|" << t->toNode << "|" << t->meters << "\n";
}
void loadTrips(const string& file) {
    ifstream f(file.c_str());
    static TripNode buf[MAX_TRIPS];
    int n = 0;
    string line;
    while (n < MAX_TRIPS && getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        TripNode t;
        t.tripCode = nextField(ss); t.patientCode = nextField(ss); t.ambulanceCode = nextField(ss); t.hospitalCode = nextField(ss);
        t.fromNode = atoi(nextField(ss).c_str()); t.toNode = atoi(nextField(ss).c_str()); t.meters = atoi(nextField(ss).c_str());
        buf[n++] = t;
        if (atoi(t.tripCode.substr(3).c_str()) >= nextTripNo) nextTripNo = atoi(t.tripCode.substr(3).c_str()) + 1;
    }
    for (int i = n - 1; i >= 0; i--) { TripNode* node = new TripNode(buf[i]); node->next = tripHead; tripHead = node; }
}

// ================= ACTION LOG: stack =================
static string actionArr[MAX_STACK];
static int actionTop = -1;

// Stack push: top index goes up
void pushAction(const string& text) { if (actionTop + 1 < MAX_STACK) actionArr[++actionTop] = text; }

// Stack pop: top index goes down (returns the latest action)
bool popAction(string& out) { if (actionTop < 0) return false; out = actionArr[actionTop--]; return true; }

// show from top (latest) down to bottom (oldest)
void showActionStack() {
    heading("Recent Actions (stack - latest first)");
    for (int i = actionTop; i >= 0; i--) cout << "- " << actionArr[i] << "\n";
}
