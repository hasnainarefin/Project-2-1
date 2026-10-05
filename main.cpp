#include "app_menu.cpp"

// save files (plain text, one record per line)
static const char* F_FLEET = "khulna_fleet.dat", *F_HOSP = "khulna_hospitals.dat", *F_PAT = "khulna_patients.dat",
                  *F_ACC = "khulna_accounts.dat", *F_TRIP = "khulna_trips.dat";

static void saveAll() { saveFleet(F_FLEET); saveHospitals(F_HOSP); savePatients(F_PAT); saveAccounts(F_ACC); saveTrips(F_TRIP); }
static void loadAll() { loadFleet(F_FLEET); loadHospitals(F_HOSP); loadPatients(F_PAT); loadAccounts(F_ACC); loadTrips(F_TRIP); }

// starting data (only added if the save files did not already contain it)
static void seedStartupData() {
    if (getFleetCount() < 10) {
        // driver, phone, start node
        addAmbulance("Zulfikar Alom",   "01711000001", 0);    // Sonadanga
        addAmbulance("Ettisuf Rup", "01711000002", 13);   // KMPOffice
        addAmbulance("Afia Broti",  "01711000003", 36);   // FireStation
        addAmbulance("Joyita Saha",  "01711000004", 9);    // KhulnaRailStn
        addAmbulance("Mahima Ghosh",  "01711000005", 18);   // RupshaBridge
        addAmbulance("Shadman sami",    "01711000006", 21);   // JuteMill
        addAmbulance("Soummo Deb", "01711000007", 26);   // Boyra
        addAmbulance("Fahim Sheikh",    "01711000008", 11);   // Nirala
        addAmbulance("Musfiq Ali",    "01711000009", 7);    // KCCOffice
        addAmbulance("Farhan Ahmed",    "01711000010", 34);   // RoyalMor
    }
    if (getHospitalCount() == 0) {
        // name, node, beds, specialty
        addHospital("Khulna Medical College Hospital", 12, 8, "General");
        addHospital("Abu Naser Specialized Hospital",   6, 4, "Cardiac");
        addHospital("Gazi Medical College Hospital",   17, 5, "General");
        addHospital("Ad-din Hospital",                 23, 6, "General");
        addHospital("Khulna City Medical College",     30, 5, "Trauma");
        addHospital("Khulna Shishu Hospital",          38, 4, "Pediatric");
    }
}

int main() {
    enableColors();
    initCityMap();      // graph
    bstBuild();         // BST of location names
    initZoneTree();     // district tree
    loadAll();
    seedStartupData();
    seedAdminAccount();
    runApplication();
    saveAll();
    return 0;
}
