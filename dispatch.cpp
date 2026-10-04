// FILE: dispatch.cpp | JOB: run one full ambulance trip (nearest ambulance -> patient -> hospital -> log)
// DSA: uses Dijkstra + Selection sort to choose, then updates arrays, linked list and stack
#pragma once
#include "map_view.cpp"
#include "fleet_hospitals.cpp"
#include "trip_history.cpp"

const double AVG_SPEED_MPS = 11.11;   // about 40 km/h, only used for animation timing

// Dijkstra + Selection sort: nearest AVAILABLE ambulance to a node (returns fleet index or -1)
static int findNearestAmbulance(int node) {
    int dist[MAX_NODES], prev[MAX_NODES];
    dijkstraFrom(node, dist, prev);
    int idx[MAX_AMBULANCES], d[MAX_AMBULANCES], count = 0;
    for (int i = 0; i < getFleetCount(); i++) {
        AmbulanceRecord& a = fleetAt(i);
        if (a.status != ST_AVAILABLE || dist[a.atNode] >= INF_DIST) continue;
        idx[count] = i; d[count] = dist[a.atNode]; count++;
    }
    if (count == 0) return -1;
    selectionSortByDistance(idx, d, count);
    return idx[0];
}

// choose the hospital: patient's choice if it has a bed, else nearest suitable one (returns hospital index or -1)
static int chooseHospital(int patientNode, const string& wantedCode, bool emergency, int severity, int& dist, int path[], int& pathLen) {
    if (!wantedCode.empty()) {
        int h = findHospital(wantedCode);
        if (h != -1 && hospitalAt(h).beds > 0) {
            int dd[MAX_NODES], pv[MAX_NODES];
            dijkstraFrom(patientNode, dd, pv);
            int target = hospitalAt(h).atNode;
            if (dd[target] < INF_DIST) { dist = dd[target]; pathLen = buildPath(pv, target, path); return h; }
        }
        warn("Chosen hospital is full or unreachable -- using the nearest alternative.");
    }
    // emergencies prefer a matching specialty first
    string specialty = "";
    if (emergency && severity <= 2) specialty = "Cardiac";
    else if (emergency && severity == 3) specialty = "Trauma";

    int idx[MAX_HOSPITALS], d[MAX_HOSPITALS], rp[MAX_HOSPITALS][MAX_PATH_LEN], rl[MAX_HOSPITALS];
    int count = rankHospitalsByDistance(patientNode, specialty, idx, d, rp, rl);
    if (count == 0) count = rankHospitalsByDistance(patientNode, "", idx, d, rp, rl);   // fall back to any specialty
    if (count == 0) return -1;
    dist = d[0]; pathLen = rl[0];
    for (int i = 0; i < pathLen; i++) path[i] = rp[0][i];
    return idx[0];
}

void runDispatch(int patientNode, const string& patientCode, const string& name, const string& phone,
                 bool emergency, int severity, const string& wantedHospital) {
    // step 1: nearest free ambulance
    int ai = findNearestAmbulance(patientNode);
    if (ai == -1) { bad("No ambulance is available right now."); return; }
    AmbulanceRecord& amb = fleetAt(ai);
    good("Assigned Ambulance: " + amb.code + " (" + amb.driver + ")");
    say(BLUE, "Driver : " + amb.driver + "  Contact: " + amb.phone);
    say(MAGENTA, "Patient: " + name + " (" + patientCode + ", " + phone + ")  at " + nodeName(patientNode));

    // step 2: route ambulance -> patient, then animate
    int dist1[MAX_NODES], prev1[MAX_NODES], path1[MAX_PATH_LEN];
    dijkstraFrom(amb.atNode, dist1, prev1);
    int len1 = buildPath(prev1, patientNode, path1);
    printPathText(path1, len1, dist1[patientNode]);
    waitEnter();
    animateDispatch(path1, len1, (int)(dist1[patientNode] / AVG_SPEED_MPS), "Ambulance heading to patient");
    setAmbulanceStatus(amb.code, ST_ENROUTE);
    pushAction("Assigned " + amb.code + " to patient " + patientCode);
    setAmbulanceStatus(amb.code, ST_ARRIVED);
    setAmbulanceNode(amb.code, patientNode);
    good("Ambulance " + amb.code + " has arrived at the patient.");

    // step 3: choose the hospital
    int dist2 = 0, len2 = 0, path2[MAX_PATH_LEN];
    int hi = chooseHospital(patientNode, wantedHospital, emergency, severity, dist2, path2, len2);
    if (hi == -1) {
        bad("No hospital beds available anywhere.");
        setAmbulanceStatus(amb.code, ST_AVAILABLE);
        return;
    }
    HospitalRecord& hos = hospitalAt(hi);

    // step 4: route patient -> hospital, then animate
    setAmbulanceStatus(amb.code, ST_TRANSPORT);
    printPathText(path2, len2, dist2);
    waitEnter();
    animateDispatch(path2, len2, (int)(dist2 / AVG_SPEED_MPS), "Transporting patient to " + hos.title);

    // step 5: hospital takes the patient, trip is recorded
    heading("HOSPITAL NOTIFIED");
    say(GREEN, "Hospital : " + hos.title + " (" + hos.code + ")");
    say(MAGENTA, "Incoming : " + name + " (" + patientCode + ")");
    bookBed(hos.code);                              // array update: beds--
    setAmbulanceStatus(amb.code, ST_AVAILABLE);
    setAmbulanceNode(amb.code, hos.atNode);
    amb.tripCount++;
    logTrip(patientCode, amb.code, hos.code, patientNode, hos.atNode, dist2);   // linked list insert
    pushAction("Completed trip for patient " + patientCode);                    // stack push
    good("Trip completed. " + amb.code + " is now available at " + hos.title + ".");
}
