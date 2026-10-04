// FILE: requests.cpp | JOB: hold waiting ambulance requests (normal = first come first served, emergency = most critical first)
// DSA: Circular Queue (array, front/rear wrap-around), Min-Heap / Priority Queue (array, sift-up / sift-down)
#pragma once
#include "common.cpp"

struct NormalRequest    { string code, patientCode; int atNode; };
struct EmergencyRequest { string code, patientCode; int atNode, severity; };   // severity 1 = most critical

// ================= NORMAL REQUESTS: circular queue =================
static NormalRequest queueArr[MAX_NORMAL_Q];
static int qFront = 0, qRear = -1, qCount = 0, nextReqNo = 1;

// enqueue at the rear, wrapping around with %
void enqueueNormal(const string& patientCode, int atNode) {
    if (qCount >= MAX_NORMAL_Q) { warn("Normal request queue is full."); return; }
    qRear = (qRear + 1) % MAX_NORMAL_Q;
    queueArr[qRear] = {makeCode("REQ", nextReqNo++), patientCode, atNode};
    qCount++;
    info("Normal request queued: " + queueArr[qRear].code);
}

// dequeue from the front (oldest request leaves first)
bool dequeueNormal(NormalRequest& out) {
    if (qCount == 0) return false;
    out = queueArr[qFront];
    qFront = (qFront + 1) % MAX_NORMAL_Q;
    qCount--;
    return true;
}

void showNormalQueue() {
    heading("Normal Request Queue (FIFO)");
    for (int i = 0, k = qFront; i < qCount; i++, k = (k + 1) % MAX_NORMAL_Q)
        cout << queueArr[k].code << " | " << queueArr[k].patientCode << " | Node " << right << setw(2) << queueArr[k].atNode << left << "\n";
}

// ================= EMERGENCY REQUESTS: min-heap =================
static EmergencyRequest heapArr[MAX_HEAP];
static int heapSize = 0, nextEmgNo = 1;

static void heapSwap(int i, int j) { EmergencyRequest t = heapArr[i]; heapArr[i] = heapArr[j]; heapArr[j] = t; }

// sift-up: new item moves up while it is more critical than its parent
static void siftUp(int i) {
    while (i > 0 && heapArr[(i - 1) / 2].severity > heapArr[i].severity) { heapSwap((i - 1) / 2, i); i = (i - 1) / 2; }
}

// sift-down: root moves down while a child is more critical
static void siftDown(int i) {
    while (true) {
        int l = 2 * i + 1, r = 2 * i + 2, smallest = i;
        if (l < heapSize && heapArr[l].severity < heapArr[smallest].severity) smallest = l;
        if (r < heapSize && heapArr[r].severity < heapArr[smallest].severity) smallest = r;
        if (smallest == i) break;
        heapSwap(i, smallest);
        i = smallest;
    }
}

// insert at the end, then sift-up, O(log n)
void insertEmergency(const string& patientCode, int atNode, int severity) {
    if (heapSize >= MAX_HEAP) { warn("Emergency queue is full."); return; }
    string code = makeCode("EMG", nextEmgNo++);
    heapArr[heapSize] = {code, patientCode, atNode, severity};
    siftUp(heapSize++);
    warn("Emergency queued: " + code + " (severity " + toStr(severity) + ")");
}

// extract the root (smallest severity number), fill the hole with the last item, sift-down
bool extractMostCritical(EmergencyRequest& out) {
    if (heapSize == 0) return false;
    out = heapArr[0];
    heapArr[0] = heapArr[--heapSize];
    siftDown(0);
    return true;
}

void showEmergencyHeap() {
    heading("Emergency Priority Queue (min-heap by severity)");
    for (int i = 0; i < heapSize; i++)
        cout << heapArr[i].code << " | " << heapArr[i].patientCode << " | Severity " << heapArr[i].severity << " | Node " << right << setw(2) << heapArr[i].atNode << left << "\n";
}
