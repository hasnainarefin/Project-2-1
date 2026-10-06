#pragma once
#include "city_graph.cpp"


void bfsFrom(int src, int parent[], int hop[], int order[], int& count) {
    bool seen[MAX_NODES];
    int q[MAX_NODES], front = 0, rear = 0;
    count = 0;
    for (int i = 0; i < getNodeCount(); i++) { parent[i] = -1; hop[i] = -1; seen[i] = false; }
    if (!validNode(src)) return;
    seen[src] = true; hop[src] = 0; q[rear++] = src;
    while (front < rear) {
        int u = q[front++];                      
        order[count++] = u;
        int to[MAX_NODES], w[MAX_NODES], n = 0;
        copyNeighbors(u, to, w, n);
        for (int k = 0; k < n; k++)
            if (!seen[to[k]]) { seen[to[k]] = true; parent[to[k]] = u; hop[to[k]] = hop[u] + 1; q[rear++] = to[k]; }   
    }
}


void dfsFrom(int src, int order[], int& count) {
    bool visited[MAX_NODES];
    int st[MAX_NODES * 4], top = -1;
    count = 0;
    for (int i = 0; i < getNodeCount(); i++) visited[i] = false;
    if (!validNode(src)) return;
    st[++top] = src;                             
    while (top >= 0) {
        int u = st[top--];                       
        if (visited[u]) continue;
        visited[u] = true;
        order[count++] = u;
        int to[MAX_NODES], w[MAX_NODES], n = 0;
        copyNeighbors(u, to, w, n);
        for (int k = n - 1; k >= 0; k--) if (!visited[to[k]]) st[++top] = to[k];
    }
}


int countComponents() {
    bool done[MAX_NODES];
    int comps = 0;
    for (int i = 0; i < getNodeCount(); i++) done[i] = false;
    for (int s = 0; s < getNodeCount(); s++) {
        if (done[s]) continue;
        int order[MAX_NODES], oc = 0;
        dfsFrom(s, order, oc);
        for (int i = 0; i < oc; i++) done[order[i]] = true;
        comps++;
    }
    return comps;
}


struct ZoneNode { string name; int ids[16]; int idCount; ZoneNode* firstChild; ZoneNode* nextSibling; };
static ZoneNode* zoneRoot = NULL;
static int nodeToZone[MAX_NODES];
static const char* ZONE_NAMES[5] = {"North", "Central", "East", "West", "South"};

static ZoneNode* makeZone(const string& name) { return new ZoneNode{name, {0}, 0, NULL, NULL}; }


static void zoneAdd(ZoneNode* z, int zoneNo, int nodeId) { z->ids[z->idCount++] = nodeId; nodeToZone[nodeId] = zoneNo; }


static void zoneLink(ZoneNode* parent, ZoneNode* child) {
    if (parent->firstChild == NULL) { parent->firstChild = child; return; }
    ZoneNode* p = parent->firstChild;
    while (p->nextSibling != NULL) p = p->nextSibling;
    p->nextSibling = child;
}


static const int ZONE_MEMBERS[5][11] = {
    {1, 2, 3, 4, 5, 6, 32, 38, 39, -1},
    {0, 7, 8, 9, 10, 11, 12, 13, 33, 35, 36},
    {14, 15, 16, 17, 18, 19, 37, -1},
    {20, 21, 22, 23, 24, 25, -1},
    {26, 27, 28, 29, 30, 31, 34, -1}
};

void initZoneTree() {
    for (int i = 0; i < MAX_NODES; i++) nodeToZone[i] = -1;
    zoneRoot = makeZone("KhulnaCity");
    for (int z = 0; z < 5; z++) {
        ZoneNode* zone = makeZone(ZONE_NAMES[z]);
        zoneLink(zoneRoot, zone);
        for (int k = 0; k < 11 && ZONE_MEMBERS[z][k] != -1; k++) zoneAdd(zone, z, ZONE_MEMBERS[z][k]);
    }
}


static void zonePreorder(ZoneNode* z, int depth) {
    if (z == NULL) return;
    cout << string(depth * 4, ' ');
    color(YELLOW); cout << z->name; color(NONE);
    if (z->idCount > 0) {
        cout << "  [";
        for (int i = 0; i < z->idCount; i++) cout << nodeName(z->ids[i]) << (i + 1 < z->idCount ? ", " : "");
        cout << "]";
    }
    cout << "\n";
    zonePreorder(z->firstChild, depth + 1);
    zonePreorder(z->nextSibling, depth);
}
void printZoneTree() { heading("Khulna Zone Tree (preorder DFS)"); zonePreorder(zoneRoot, 0); }

string zoneOfNode(int id) { return (id >= 0 && id < MAX_NODES && nodeToZone[id] >= 0) ? ZONE_NAMES[nodeToZone[id]] : "Unknown"; }
