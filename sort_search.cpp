
#pragma once
#include "city_graph.cpp"

void selectionSortByDistance(int idx[], int dist[], int count) {
    for (int i = 0; i < count - 1; i++) {
        int minPos = i;
        for (int j = i + 1; j < count; j++) if (dist[j] < dist[minPos]) minPos = j;
        if (minPos != i) {
            int d = dist[i]; dist[i] = dist[minPos]; dist[minPos] = d;
            int x = idx[i];  idx[i]  = idx[minPos];  idx[minPos]  = x;
        }
    }
}

// Insertion sort: take each item and slide it left into its place
void insertionSortByDistance(int idx[], int dist[], int count) {
    for (int i = 1; i < count; i++) {
        int keyD = dist[i], keyI = idx[i], j = i - 1;
        while (j >= 0 && dist[j] > keyD) { dist[j + 1] = dist[j]; idx[j + 1] = idx[j]; j--; }
        dist[j + 1] = keyD; idx[j + 1] = keyI;
    }
}

// Merge sort (divide and conquer) on idx/dist: merge two sorted halves
static void mergeHalves(int idx[], int dist[], int lo, int mid, int hi, int tIdx[], int tDist[]) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) {
        if (dist[i] <= dist[j]) { tDist[k] = dist[i]; tIdx[k] = idx[i]; i++; }
        else                    { tDist[k] = dist[j]; tIdx[k] = idx[j]; j++; }
        k++;
    }
    while (i <= mid) { tDist[k] = dist[i]; tIdx[k] = idx[i]; i++; k++; }
    while (j <= hi)  { tDist[k] = dist[j]; tIdx[k] = idx[j]; j++; k++; }
    for (int t = lo; t <= hi; t++) { dist[t] = tDist[t]; idx[t] = tIdx[t]; }
}
static void mergeSortRec(int idx[], int dist[], int lo, int hi, int tIdx[], int tDist[]) {
    if (lo >= hi) return;
    int mid = (lo + hi) / 2;
    mergeSortRec(idx, dist, lo, mid, tIdx, tDist);
    mergeSortRec(idx, dist, mid + 1, hi, tIdx, tDist);
    mergeHalves(idx, dist, lo, mid, hi, tIdx, tDist);
}
void mergeSortByDistance(int idx[], int dist[], int count) {
    int tIdx[MAX_TRIPS], tDist[MAX_TRIPS];
    mergeSortRec(idx, dist, 0, count - 1, tIdx, tDist);
}

// Merge sort on names (needed before binary search)
static void mergeNames(string a[], int lo, int mid, int hi, string tmp[]) {
    int i = lo, j = mid + 1, k = lo;
    while (i <= mid && j <= hi) tmp[k++] = (a[i] <= a[j]) ? a[i++] : a[j++];
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi)  tmp[k++] = a[j++];
    for (int t = lo; t <= hi; t++) a[t] = tmp[t];
}
static void mergeSortNamesRec(string a[], int lo, int hi, string tmp[]) {
    if (lo >= hi) return;
    int mid = (lo + hi) / 2;
    mergeSortNamesRec(a, lo, mid, tmp);
    mergeSortNamesRec(a, mid + 1, hi, tmp);
    mergeNames(a, lo, mid, hi, tmp);
}
void mergeSortNames(string a[], int count) {
    string tmp[MAX_SORT];
    mergeSortNamesRec(a, 0, count - 1, tmp);
}

// ---------- searching on a name array ----------

// Linear search: check every item one by one, O(n)
int linearSearchNames(const string a[], int count, const string& target) {
    for (int i = 0; i < count; i++) if (a[i] == target) return i;
    return -1;
}

// Binary search: array must be sorted; halve the range each step, O(log n)
int binarySearchNames(const string a[], int count, const string& target) {
    int lo = 0, hi = count - 1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (a[mid] == target) return mid;
        if (a[mid] < target) lo = mid + 1; else hi = mid - 1;
    }
    return -1;
}

// ---------- Binary Search Tree: location name -> node id ----------
struct BstNode { string name; int nodeId; BstNode* left; BstNode* right; };
static BstNode* bstRoot = NULL;

// BST insert: smaller names go left, bigger go right
static BstNode* bstInsertRec(BstNode* n, const string& name, int id) {
    if (n == NULL) { return new BstNode{name, id, NULL, NULL}; }
    if (name < n->name) n->left = bstInsertRec(n->left, name, id);
    else if (name > n->name) n->right = bstInsertRec(n->right, name, id);
    return n;
}

// BST search: go left or right until found, O(height)
static int bstFindRec(BstNode* n, const string& name) {
    if (n == NULL) return -1;
    if (name == n->name) return n->nodeId;
    return bstFindRec(name < n->name ? n->left : n->right, name);
}
int bstFind(const string& name) { return bstFindRec(bstRoot, name); }

// build the tree from all map locations
void bstBuild() {
    string names[MAX_NODES]; int ids[MAX_NODES], count = 0;
    fillLocationNames(names, ids, count);
    for (int i = 0; i < count; i++) bstRoot = bstInsertRec(bstRoot, names[i], ids[i]);
}
