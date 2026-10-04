// FILE: hash_tables.cpp | JOB: look up patients (by ID) and login accounts (by username) in O(1)
// DSA: Hash table with separate chaining (array of buckets, each bucket = linked list)
#pragma once
#include "common.cpp"

const int BUCKETS = 101;   // prime size = fewer collisions

// Hash function: polynomial rolling hash of the key string -> bucket number
static int hashKey(const string& key) {
    unsigned long h = 0;
    for (size_t i = 0; i < key.size(); i++) h = h * 31 + (unsigned char)key[i];
    return (int)(h % BUCKETS);
}

// ================= PATIENTS (key = patient code) =================
struct PatientCell { PatientRecord data; PatientCell* next; };
static PatientCell* patientTable[BUCKETS];   // static arrays start as all NULL
static int nextPatNo = 1;

// insert at the head of the bucket's linked list
static void patientInsert(const PatientRecord& r) {
    int slot = hashKey(r.code);
    patientTable[slot] = new PatientCell{r, patientTable[slot]};
}

string registerPatient(const string& name, const string& phone, int atNode) {
    PatientRecord r = {makeCode("PAT", nextPatNo++), name, phone, atNode};
    patientInsert(r);
    return r.code;
}

// look up: hash the key, then walk only that one bucket
PatientRecord* lookupPatient(const string& code) {
    for (PatientCell* p = patientTable[hashKey(code)]; p != NULL; p = p->next)
        if (p->data.code == code) return &p->data;
    return NULL;
}

void listPatients() {
    heading("Registered Patients");
    for (int i = 0; i < BUCKETS; i++)
        for (PatientCell* p = patientTable[i]; p != NULL; p = p->next)
            cout << "  " << left << setw(8) << p->data.code << setw(16) << p->data.name << setw(14) << p->data.phone << "Node " << p->data.atNode << "\n";
}

void savePatients(const string& file) {
    ofstream f(file.c_str());
    for (int i = 0; i < BUCKETS; i++)
        for (PatientCell* p = patientTable[i]; p != NULL; p = p->next)
            f << p->data.code << "|" << p->data.name << "|" << p->data.phone << "|" << p->data.atNode << "\n";
}
void loadPatients(const string& file) {
    ifstream f(file.c_str());
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        PatientRecord r;
        r.code = nextField(ss); r.name = nextField(ss); r.phone = nextField(ss); r.atNode = atoi(nextField(ss).c_str());
        patientInsert(r);
        if (atoi(r.code.substr(3).c_str()) >= nextPatNo) nextPatNo = atoi(r.code.substr(3).c_str()) + 1;
    }
}

// ================= LOGIN ACCOUNTS (key = username) =================
struct AccountCell { UserAccount data; AccountCell* next; };
static AccountCell* accountTable[BUCKETS];
static UserAccount* loggedIn = NULL;

// simple djb2 password hash so plain passwords are never saved (course project, not real security)
static string hashPassword(const string& pw) {
    unsigned long h = 5381;
    for (size_t i = 0; i < pw.size(); i++) h = ((h << 5) + h) + (unsigned char)pw[i];
    ostringstream o; o << hex << h;
    return o.str();
}

static AccountCell* findAccountCell(const string& username) {
    for (AccountCell* p = accountTable[hashKey(username)]; p != NULL; p = p->next)
        if (p->data.username == username) return p;
    return NULL;
}

static void accountInsert(const UserAccount& a) {
    int slot = hashKey(a.username);
    accountTable[slot] = new AccountCell{a, accountTable[slot]};
}

bool registerAccount(const string& username, const string& password, int role, const string& patientCode) {
    if (findAccountCell(username) != NULL) return false;      // username already used
    UserAccount a = {username, hashPassword(password), role, patientCode};
    accountInsert(a);
    return true;
}

UserAccount* loginAccount(const string& username, const string& password) {
    AccountCell* c = findAccountCell(username);
    if (c == NULL || c->data.passHash != hashPassword(password)) return NULL;
    loggedIn = &c->data;
    return loggedIn;
}

void logoutAccount() { loggedIn = NULL; }
UserAccount* activeAccount() { return loggedIn; }

void seedAdminAccount() { if (findAccountCell("admin") == NULL) registerAccount("admin", "admin123", ROLE_ADMIN, ""); }

void setAccountPatientCode(const string& username, const string& code) {
    AccountCell* c = findAccountCell(username);
    if (c != NULL) c->data.patientCode = code;
}

bool changePassword(const string& username, const string& oldPw, const string& newPw) {
    AccountCell* c = findAccountCell(username);
    if (c == NULL || c->data.passHash != hashPassword(oldPw)) return false;
    c->data.passHash = hashPassword(newPw);
    return true;
}

void listAccounts() {
    heading("All Login Accounts (hash table walk)");
    for (int i = 0; i < BUCKETS; i++)
        for (AccountCell* p = accountTable[i]; p != NULL; p = p->next)
            cout << "  user=" << p->data.username << "  role=" << (p->data.role == ROLE_ADMIN ? "ADMIN" : "USER")
                 << "  patient=" << (p->data.patientCode.empty() ? "-" : p->data.patientCode) << "  bucket=" << i << "\n";
}

void saveAccounts(const string& file) {
    ofstream f(file.c_str());
    for (int i = 0; i < BUCKETS; i++)
        for (AccountCell* p = accountTable[i]; p != NULL; p = p->next)
            f << p->data.username << "|" << p->data.passHash << "|" << p->data.role << "|" << p->data.patientCode << "\n";
}
void loadAccounts(const string& file) {
    ifstream f(file.c_str());
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        UserAccount a;
        a.username = nextField(ss); a.passHash = nextField(ss); a.role = atoi(nextField(ss).c_str()); a.patientCode = nextField(ss);
        accountInsert(a);
    }
}
