#pragma once
#include "dispatch.cpp"
#include "hash_tables.cpp"
#include "requests.cpp"

static int askLocationNode(const string& prompt) {
    showGridMap();
    listLocations();
    int node = readInt(prompt);
    if (!validNode(node)) { bad("That node id is not on the map."); return -1; }
    return node;
}

static void searchLocationFlow() {
    showScreen("Search Location");
    menuItem("1", "Linear search by name");
    menuItem("2", "Binary search by name");
    menuItem("3", "BST search by name");
    int c = readInt("Choice: ");
    string name = readLine("Location name (exact, e.g. KUET): ");
    string names[MAX_NODES]; int ids[MAX_NODES], count = 0;
    fillLocationNames(names, ids, count);
    int found = -1;
    if (c == 1) {
        found = linearSearchNames(names, count, name);
    } else if (c == 2) {
        mergeSortNames(names, count);
        if (binarySearchNames(names, count, name) != -1) found = bstFind(name);
    } else {
        found = bstFind(name);
    }
    if (found == -1) bad("Not found.");
    else good("Node " + toStr(found) + "  " + nodeName(found) + "  zone=" + zoneOfNode(found));
    waitEnter();
}

static void manageFleetMenu() {
    while (true) {
        showScreen("Admin -> Manage Fleet");
        menuItem("1", "View all ambulances");
        menuItem("2", "Add ambulance");
        menuItem("3", "Remove ambulance");
        menuItem("4", "Update status");
        menuItem("5", "Rank ambulances by distance to a location");
        menuItem("0", "Back");
        int c = readInt("Choice: ");
        if (c == 0) break;
        if (c == 1) { listFleet(); }
        else if (c == 2) {
            string d = readLine("Driver name: "), ph = readLine("Contact: ");
            int node = askLocationNode("Station node id: ");
            if (node >= 0) { string code = addAmbulance(d, ph, node); good("Added " + code); pushAction("Added ambulance " + code); }
        } else if (c == 3) {
            listFleet();
            string code = readLine("\nAmbulance ID to remove: ");
            if (removeAmbulance(code)) { good("Removed."); pushAction("Removed ambulance " + code); } else bad("Not found.");
        } else if (c == 4) {
            listFleet();
            string code = readLine("\nAmbulance ID: ");
            menuItem("1", "Available"); menuItem("2", "En Route"); menuItem("3", "Arrived"); menuItem("4", "Transporting"); menuItem("5", "Maintenance");
            int s = readInt("Status: ");
            if (s >= 1 && s <= 5 && findAmbulance(code) != -1) { setAmbulanceStatus(code, s - 1); good("Status updated."); } else bad("Invalid.");
        } else if (c == 5) {
            int node = askLocationNode("Location node id: ");
            if (node >= 0) rankAmbulancesTo(node);
        }
        waitEnter();
    }
}

static void manageHospitalsMenu() {
    while (true) {
        showScreen("Admin -> Manage Hospitals");
        menuItem("1", "View all hospitals");
        menuItem("2", "Add hospital");
        menuItem("3", "Remove hospital");
        menuItem("0", "Back");
        int c = readInt("Choice: ");
        if (c == 0) break;
        if (c == 1) { listHospitals(); }
        else if (c == 2) {
            string n = readLine("Name: ");
            int node = askLocationNode("Location node id: ");
            if (node >= 0) {
                int beds = readInt("Available beds: ");
                string spec = readLine("Specialty (General/Cardiac/Trauma/Pediatric): ");
                good("Added " + addHospital(n, node, beds, spec));
            }
        } else if (c == 3) {
            listHospitals();
            if (removeHospital(readLine("\nHospital ID to remove: "))) good("Removed."); else bad("Not found.");
        }
        waitEnter();
    }
}

static void manageAccountsMenu() {
    while (true) {
        showScreen("Admin -> Login Accounts");
        menuItem("1", "List all accounts");
        menuItem("2", "Create another admin login");
        menuItem("3", "Change my password");
        menuItem("4", "List registered patients");
        menuItem("0", "Back");
        int c = readInt("Choice: ");
        if (c == 0) break;
        if (c == 1) listAccounts();
        else if (c == 2) {
            string u = readLine("New admin username: "), p = readLine("Password: ");
            if (registerAccount(u, p, ROLE_ADMIN, "")) good("Admin created."); else bad("Username already exists.");
        } else if (c == 3) {
            string oldP = readLine("Current password: "), newP = readLine("New password: ");
            if (activeAccount() && changePassword(activeAccount()->username, oldP, newP)) good("Password updated."); else bad("Could not change password.");
        } else if (c == 4) listPatients();
        waitEnter();
    }
}

static void cityAnalyticsMenu() {
    while (true) {
        showScreen("Admin -> City Analytics");
        menuItem("1", "City map");
        menuItem("2", "Roads");
        menuItem("3", "Zone tree");
        menuItem("4", "Trips sorted by distance");
        menuItem("0", "Back");
        int c = readInt("Choice: ");
        if (c == 0) break;
        if (c == 1) showGridMap();
        else if (c == 2) { printRoadsMatrix(); cout << "\nConnected pieces in the city: " << countComponents() << "\n"; }
        else if (c == 3) printZoneTree();
        else if (c == 4) showTripsSortedByDistance();
        waitEnter();
    }
}

static void adminDashboard() {
    while (true) {
        showScreen("ADMIN DASHBOARD");
        menuItem("1", "Manage Ambulance Fleet");
        menuItem("2", "Manage Hospitals");
        menuItem("3", "Manage Login Accounts");
        menuItem("4", "View Requests");
        menuItem("5", "View All Trips");
        menuItem("6", "View Action History");
        menuItem("7", "City Analytics");
        menuItem("0", "Logout");
        int c = readInt("Choice: ");
        if (c == 0) break;
        if (c == 1) { manageFleetMenu(); continue; }
        if (c == 2) { manageHospitalsMenu(); continue; }
        if (c == 3) { manageAccountsMenu(); continue; }
        if (c == 7) { cityAnalyticsMenu(); continue; }
        if (c == 4) { showNormalQueue(); showEmergencyHeap(); }
        else if (c == 5) showAllTrips();
        else if (c == 6) showActionStack();
        waitEnter();
    }
}

static void requestAmbulanceFlow(UserAccount* acc) {
    showScreen("Request Ambulance - Step 1: Your Information");
    PatientRecord* p = lookupPatient(acc->patientCode);
    string name, phone;
    int node;
    if (p != NULL) {
        name = p->name; phone = p->phone; node = p->atNode;
        info("Welcome back, " + name + ". Saved location: " + nodeName(node));
        string change = readLine("Update phone or location? (y/n): ");
        if (change == "y" || change == "Y") {
            phone = p->phone = readLine("Contact number: ");
            int nn = askLocationNode("Current location node: ");
            if (nn >= 0) node = p->atNode = nn;
        }
    } else {
        name = readLine("Your full name: ");
        phone = readLine("Your contact number: ");
        node = askLocationNode("Your current location node: ");
        if (node < 0) { waitEnter(); return; }
        string newCode = registerPatient(name, phone, node);
        setAccountPatientCode(acc->username, newCode);
        acc->patientCode = newCode;
        good("Registered as patient " + newCode);
    }
    good("Pickup: " + nodeName(node));
    waitEnter();

    showScreen("Request Ambulance - Step 2: Destination Hospital");
    printReachableHospitals(node);
    listHospitals();
    string hospCode = readLine("\nHospital ID you want to go to: ");
    int hi = findHospital(hospCode);
    if (hi == -1) { bad("Hospital ID not found. Request cancelled."); waitEnter(); return; }
    good("Destination: " + hospitalAt(hi).title);
    waitEnter();

    showScreen("Request Ambulance - Step 3: Request Type");
    menuItem("1", "Normal (circular queue)");
    menuItem("2", "Emergency (min-heap by severity)");
    int type = readInt("Type: ");
    if (type == 2) {
        int sev = readInt("Severity (1 = most critical .. 5 = least): ");
        insertEmergency(acc->patientCode, node, sev);
        EmergencyRequest r;
        if (extractMostCritical(r)) runDispatch(r.atNode, r.patientCode, name, phone, true, r.severity, hospCode);
    } else {
        enqueueNormal(acc->patientCode, node);
        NormalRequest r;
        if (dequeueNormal(r)) runDispatch(r.atNode, r.patientCode, name, phone, false, 0, hospCode);
    }
    waitEnter();
}

static void showNearestHospitalsFlow(UserAccount* acc) {
    showScreen("Nearby Hospitals");
    PatientRecord* p = lookupPatient(acc->patientCode);
    int node = (p != NULL) ? p->atNode : askLocationNode("Your location node: ");
    if (node < 0) { waitEnter(); return; }
    int idx[MAX_HOSPITALS], d[MAX_HOSPITALS], rp[MAX_HOSPITALS][MAX_PATH_LEN], rl[MAX_HOSPITALS];
    int count = rankHospitalsByDistance(node, "", idx, d, rp, rl);
    for (int i = 0; i < count; i++) {
        HospitalRecord& h = hospitalAt(idx[i]);
        cout << "  " << left << setw(8) << h.code << setw(34) << h.title << right << setw(6) << d[i] << " m   Beds: " << setw(2) << h.beds << "   " << left << h.specialty << "\n";
    }
    waitEnter();
}

static void userDashboard(UserAccount* acc) {
    while (true) {
        showScreen("USER DASHBOARD - " + acc->username);
        menuItem("1", "Request Ambulance");
        menuItem("2", "My Trip History");
        menuItem("3", "Nearest Hospitals");
        menuItem("4", "View City Map");
        menuItem("5", "Hospitals reachable from me");
        menuItem("6", "Search a location");
        menuItem("0", "Logout");
        int c = readInt("Choice: ");
        if (c == 0) break;
        if (c == 1) { requestAmbulanceFlow(acc); continue; }
        if (c == 3) { showNearestHospitalsFlow(acc); continue; }
        if (c == 6) { searchLocationFlow(); continue; }
        if (c == 2) showTripsForPatient(acc->patientCode);
        else if (c == 4) showGridMap();
        else if (c == 5) {
            PatientRecord* p = lookupPatient(acc->patientCode);
            int node = (p != NULL) ? p->atNode : askLocationNode("Your node: ");
            if (node >= 0) printReachableHospitals(node);
        }
        waitEnter();
    }
}


static UserAccount* doLogin(int requiredRole, const string& title) {
    showScreen(title);
    string u = readLine("Username: "), p = readLine("Password: ");
    UserAccount* acc = loginAccount(u, p);
    if (acc == NULL) { bad("Invalid username or password."); waitEnter(); return NULL; }
    if (acc->role != requiredRole) {
        bad(requiredRole == ROLE_ADMIN ? "Not an admin account. Use User Login." : "This is an admin account. Use Admin Login.");
        logoutAccount(); waitEnter(); return NULL;
    }
    good("Welcome, " + acc->username + "!");
    waitEnter();
    return acc;
}

void runApplication() {
    while (true) {
        showScreen("KHULNA AMBULANCE ROUTING SYSTEM");
        menuItem("1", "User Login");
        menuItem("2", "User Signup");
        menuItem("3", "Admin Login");
        menuItem("0", "Exit");
        int c = readInt("Choice: ");
        if (c == 0) return;
        if (c == 1) { UserAccount* a = doLogin(ROLE_USER, "USER LOGIN"); if (a) { userDashboard(a); logoutAccount(); } }
        else if (c == 3) { UserAccount* a = doLogin(ROLE_ADMIN, "ADMIN LOGIN"); if (a) { adminDashboard(); logoutAccount(); } }
        else if (c == 2) {
            showScreen("User Signup");
            string u = readLine("Choose username: "), p = readLine("Choose password: ");
            if (registerAccount(u, p, ROLE_USER, "")) good("Account created. Use User Login next."); else bad("Username already taken.");
            waitEnter();
        }
    }
}
