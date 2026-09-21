#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <windows.h> 

using namespace std;

// --- COLOR AND STYLE CONFIGURATION ---
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BOLD_RED    "\033[1;31m"
#define BOLD_GREEN  "\033[1;32m"
#define BOLD_YELLOW "\033[1;33m"
#define BOLD_BLUE   "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN   "\033[1;36m"
#define BOLD_WHITE  "\033[1;37m"

// --- ENUMS ---
enum Airline { 
    AIR_INDIA = 1, EMIRATES, LUFTHANSA, SINGAPORE, UNITED, 
    INDIGO, VISTARA, QATAR, QANTAS, AIR_CANADA, 
    CATHAY, KLM, ETIHAD, THAI, BRITISH_AIRWAYS, AIR_FRANCE,
    SPICEJET, AKASA, AIR_EXPRESS, ANA, SCOOT, VIRGIN_ATLANTIC, VARIOUS
};

enum PassType { FREQUENT_FLYER = 1, GUEST };
enum SeatType { ECONOMY = 1, BUSINESS, FIRST };
enum SortPreference { TIME_SAVING = 1, ECONOMIC };

// ==========================================
// 1. ENCAPSULATION: Airport Class
// ==========================================
class Airport {
private:
    int id;
    string code;
    string city;
    bool isDom;
    bool isIntl;

public:
    Airport(int i, string c, string ci, bool d, bool in) 
        : id(i), code(c), city(ci), isDom(d), isIntl(in) {}

    int getId() const { return id; }
    string getCode() const { return code; }
    string getCity() const { return city; }
    bool isDomestic() const { return isDom; }
    bool isInternational() const { return isIntl; }
};

struct Itinerary {
    vector<int> path;
    vector<int> airlines; 
    int num_layovers;
    int total_time;
    double total_fare;
};

// ==========================================
// 2. INHERITANCE & POLYMORPHISM: Passenger Classes
// ==========================================
class Passenger {
protected:
    double balance;
    SeatType seat;

public:
    Passenger(double b, SeatType s) : balance(b), seat(s) {}
    virtual ~Passenger() = default;

    double getBalance() const { return balance; }
    SeatType getSeatType() const { return seat; }
    
    virtual double getFinalFare(double baseTotalFare) const = 0;
};

class FrequentFlyer : public Passenger {
public:
    FrequentFlyer(double b, SeatType s) : Passenger(b, s) {}
    
    double getFinalFare(double baseTotalFare) const override {
        double multiplier = (seat == FIRST) ? 4.20 : (seat == BUSINESS ? 2.45 : 1.0);
        return (baseTotalFare * multiplier) * 0.90; 
    }
};

class Guest : public Passenger {
public:
    Guest(double b, SeatType s) : Passenger(b, s) {}
    
    double getFinalFare(double baseTotalFare) const override {
        double multiplier = (seat == FIRST) ? 4.20 : (seat == BUSINESS ? 2.45 : 1.0);
        return (baseTotalFare * multiplier); 
    }
};

// ==========================================
// 3. BASE CLASS: Network Graph Logic
// ==========================================
class NetworkGraph {
protected:
    vector<Airport> airports;
    int time_matrix[100][100];
    int airline_matrix[100][100];
    double fare_matrix[100][100];

public:
    NetworkGraph() {
        for(int i=0; i<100; i++) {
            for(int j=0; j<100; j++) {
                time_matrix[i][j] = 0;
                airline_matrix[i][j] = 0;
                fare_matrix[i][j] = 0.0;
            }
        }
    }
    
    virtual ~NetworkGraph() = default;

    void addAirport(string code, string city, bool isD, bool isI) {
        int id = airports.size();
        airports.push_back(Airport(id, code, city, isD, isI));
    }

    void addFlight(int u, int v, int time_mins, int airlineId, double fare) {
        if (fare_matrix[u][v] == 0.0 || fare < fare_matrix[u][v]) {
            fare_matrix[u][v] = fare;
            fare_matrix[v][u] = fare;
            time_matrix[u][v] = time_mins; 
            time_matrix[v][u] = time_mins; 
            airline_matrix[u][v] = airlineId; 
            airline_matrix[v][u] = airlineId;
        }
    }
    
    int getAirportIdByCode(const string& code) {
        for (const auto& a : airports) {
            if (a.getCode() == code) return a.getId();
        }
        return -1;
    }
};

// ==========================================
// 4. DERIVED CLASS: Vaayu System Implementation
// ==========================================
class VaayuSystem : public NetworkGraph {
private:
    vector<Itinerary> all_itineraries;
    vector<bool> visited;
    vector<int> tempPath;

    int parseDuration(const string& durStr) {
        int h = 0, m = 0;
        size_t hPos = durStr.find('h');
        size_t mPos = durStr.find('m');
        if (hPos != string::npos) h = stoi(durStr.substr(0, hPos));
        if (mPos != string::npos && hPos != string::npos) {
            m = stoi(durStr.substr(hPos + 1, mPos - hPos - 1));
        }
        return (h * 60) + m;
    }

    string getAirlineName(int id) {
        switch(id) {
            case AIR_INDIA:       return "Air India";
            case EMIRATES:        return "Emirates";
            case LUFTHANSA:       return "Lufthansa";
            case SINGAPORE:       return "Singapore Airlines";
            case INDIGO:          return "IndiGo";
            case VISTARA:         return "Vistara";
            case SPICEJET:        return "SpiceJet";
            case AKASA:           return "Akasa Air";
            case AIR_EXPRESS:     return "Air Express";
            case ETIHAD:          return "Etihad Airways";
            case CATHAY:          return "Cathay Pacific";
            case ANA:             return "ANA";
            case SCOOT:           return "Scoot";
            case KLM:             return "KLM";
            case BRITISH_AIRWAYS: return "British Airways";
            case QATAR:           return "Qatar Airways";
            case THAI:            return "Thai Airways";
            case AIR_FRANCE:      return "Air France";
            case QANTAS:          return "Qantas";
            case AIR_CANADA:      return "Air Canada";
            case VIRGIN_ATLANTIC: return "Virgin Atlantic";
            default:              return "Partner Airline";
        }
    }

    int mapAirlineNameToId(string name) {
        if (name == "Air India") return AIR_INDIA;
        if (name == "IndiGo") return INDIGO;
        if (name == "Vistara") return VISTARA;
        if (name == "SpiceJet") return SPICEJET;
        if (name == "Akasa Air") return AKASA;
        if (name == "Air Express") return AIR_EXPRESS;
        if (name == "Etihad Airways") return ETIHAD;
        if (name == "Singapore Airlines") return SINGAPORE;
        if (name == "Cathay Pacific") return CATHAY;
        if (name == "ANA") return ANA;
        if (name == "Scoot") return SCOOT;
        if (name == "Emirates") return EMIRATES;
        if (name == "KLM") return KLM;
        if (name == "British Airways") return BRITISH_AIRWAYS;
        if (name == "Qatar Airways") return QATAR;
        if (name == "Thai Airways") return THAI;
        if (name == "Lufthansa") return LUFTHANSA;
        if (name == "Air France") return AIR_FRANCE;
        if (name == "Qantas") return QANTAS;
        if (name == "Air Canada") return AIR_CANADA;
        if (name == "Virgin Atlantic") return VIRGIN_ATLANTIC;
        return VARIOUS;
    }

    void dfs(int curr, int dest, int limit) {
        if (tempPath.size() > limit) return; 
        visited[curr] = true;
        tempPath.push_back(curr);

        if (curr == dest) {
            Itinerary iten;
            iten.path = tempPath;
            iten.num_layovers = max(0, (int)tempPath.size() - 2);
            iten.total_time = 0;
            double base_fare = 0;

            for (size_t i = 0; i < tempPath.size() - 1; i++) {
                int u = tempPath[i], v = tempPath[i+1];
                iten.total_time += time_matrix[u][v];
                base_fare += fare_matrix[u][v]; 
                iten.airlines.push_back(airline_matrix[u][v]); 
                if (i > 0) iten.total_time += 90; 
            }
            
            iten.total_fare = 1500.0 + base_fare + (iten.num_layovers * 1200.0);
            all_itineraries.push_back(iten);
        } else {
            for (size_t i = 0; i < airports.size(); i++) {
                if (time_matrix[curr][i] > 0 && !visited[i]) {
                    dfs(i, dest, limit);
                }
            }
        }
        tempPath.pop_back();
        visited[curr] = false;
    }

public:
    VaayuSystem() {
        visited.assign(100, false);
    }

    void initializeAirports() {
        addAirport("DEL", "New Delhi", true, true);
        addAirport("BOM", "Mumbai", true, true);
        addAirport("BLR", "Bangalore", true, true);
        addAirport("HYD", "Hyderabad", true, true);
        addAirport("MAA", "Chennai", true, true);
        addAirport("CCU", "Kolkata", true, false);
        addAirport("COK", "Kochi", true, false);
        addAirport("AMD", "Ahmedabad", true, false);
        addAirport("GOI", "Goa", true, false);
        addAirport("PNQ", "Pune", true, false);
        addAirport("SFO", "San Francisco", false, true);
        addAirport("JFK", "New York", false, true);
        addAirport("SIN", "Singapore", false, true);
        addAirport("BKK", "Bangkok", false, true);
        addAirport("HKG", "Hong Kong", false, true);
        addAirport("DXB", "Dubai", false, true);
        addAirport("PEK", "Beijing", false, true);
        addAirport("AMS", "Amsterdam", false, true);
        addAirport("LHR", "London", false, true);
        addAirport("DOH", "Doha", false, true);
        addAirport("FRA", "Frankfurt", false, true);
        addAirport("CDG", "Paris", false, true);
        addAirport("NRT", "Tokyo", false, true);
        addAirport("SYD", "Sydney", false, true);
        addAirport("YYZ", "Toronto", false, true);
        addAirport("AUH", "Abu Dhabi", false, true);
    }

    void loadFlightsFromCSV(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "\n" << BOLD_RED << " [!] CRITICAL ERROR: Could not open " << filename << RESET << "\n";
            return;
        }

        string line;
        int flightCount = 0;
        
        while (getline(file, line)) {
            if (line.empty() || line.find("Origin_IATA") != string::npos) continue;

            stringstream ss(line);
            string uCode, vCode, airline, routing, distStr, durStr, fareStr, curr;
            
            getline(ss, uCode, ',');
            getline(ss, vCode, ',');
            getline(ss, airline, ',');
            getline(ss, routing, ',');
            getline(ss, distStr, ',');
            getline(ss, durStr, ',');
            getline(ss, fareStr, ',');
            getline(ss, curr, '\r'); 

            int uId = getAirportIdByCode(uCode);
            int vId = getAirportIdByCode(vCode);
            
            if (uId == -1 || vId == -1) continue;

            int timeMins = parseDuration(durStr);
            int airlineId = mapAirlineNameToId(airline);
            double fare = stod(fareStr);

            if (curr.find("USD") != string::npos) fare *= 83.50;
            if (curr.find("SGD") != string::npos) fare *= 61.80;
            if (curr.find("HKD") != string::npos) fare *= 10.65;

            addFlight(uId, vId, timeMins, airlineId, fare);
            flightCount++;
        }
        file.close();
        cout << BOLD_GREEN << " [+] Successfully loaded " << flightCount << " live routes from CSV." << RESET << "\n";
    }

    void search(int s, int d, Passenger* pax, SortPreference pref) {
        if (s == -1 || d == -1) {
            cout << "\n" << BOLD_RED << " [!] Invalid Origin or Destination selected." << RESET << "\n";
            return;
        }

        all_itineraries.clear();
        tempPath.clear();
        visited.assign(100, false);
        
        dfs(s, d, 4); 

        if (all_itineraries.empty()) {
            cout << "\n" << BOLD_RED << " [!] No direct or connecting flights found for this route." << RESET << "\n";
            return;
        }

        sort(all_itineraries.begin(), all_itineraries.end(), [pref](const Itinerary& a, const Itinerary& b) {
            if (pref == TIME_SAVING) return a.total_time < b.total_time;
            return a.total_fare < b.total_fare;
        });

        int show = min((int)all_itineraries.size(), 3);
        cout << "\n" << BOLD_CYAN << " [*] FOUND " << show << " OPTIMAL ROUTE(S)" << RESET << "\n";
        for(int i = 0; i < show; i++) {
            renderItinerary(all_itineraries[i], i + 1, pax);
        }
    }

    void renderItinerary(const Itinerary& it, int rank, Passenger* pax) {
        double final_price = pax->getFinalFare(it.total_fare);
        
        cout << "\n" << BOLD_BLUE << " +----------------------------------------------------------+" << RESET << "\n";
        
        cout << BOLD_BLUE << " | " << RESET << BOLD_WHITE << "OPTION " << rank << RESET;
        if (rank == 1) cout << BOLD_YELLOW << " [RECOMMENDED] " << RESET << "                            ";
        else cout << "                                           ";
        cout << BOLD_BLUE << "|\n" << RESET;

        cout << BOLD_BLUE << " +----------------------------------------------------------+" << RESET << "\n";
        
        cout << BOLD_BLUE << " | " << RESET << BOLD_CYAN << "Route      : " << RESET;
        for(size_t i=0; i<it.path.size(); i++) {
            cout << BOLD_YELLOW << airports[it.path[i]].getCode() << RESET;
            if (i != it.path.size() - 1) cout << BOLD_WHITE << " -> " << RESET;
        }
        cout << "\n";

        cout << BOLD_BLUE << " | " << RESET << BOLD_CYAN << "Airlines   : " << RESET;
        for(size_t i=0; i<it.airlines.size(); i++) {
            cout << BOLD_MAGENTA << getAirlineName(it.airlines[i]) << RESET;
            if (i != it.airlines.size() - 1) cout << BOLD_WHITE << " -> " << RESET;
        }
        cout << "\n";

        cout << BOLD_BLUE << " | " << RESET << BOLD_CYAN << "Duration   : " << RESET << it.total_time / 60 << "h " << it.total_time % 60 << "m\n";
        cout << BOLD_BLUE << " | " << RESET << BOLD_CYAN << "Layovers   : " << RESET << it.num_layovers << "\n";
        cout << BOLD_BLUE << " | " << RESET << BOLD_CYAN << "Class      : " << RESET << (pax->getSeatType() == FIRST ? "First" : (pax->getSeatType() == BUSINESS ? "Business" : "Economy")) << "\n";
        
        cout << BOLD_BLUE << " | " << RESET << BOLD_CYAN << "Final Fare : " << BOLD_GREEN << "Rs. " << fixed << setprecision(2) << final_price << RESET << "\n";
        
        cout << BOLD_BLUE << " | " << RESET << BOLD_CYAN << "Status     : " << RESET;
        if(pax->getBalance() >= final_price)
            cout << BOLD_GREEN << "[+] READY TO BOOK" << RESET << " (Rem: Rs. " << pax->getBalance() - final_price << ")\n";
        else
            cout << BOLD_RED << "[-] INSUFFICIENT FUNDS" << RESET << " (Short: Rs. " << final_price - pax->getBalance() << ")\n";
            
        cout << BOLD_BLUE << " +----------------------------------------------------------+" << RESET << "\n";
    }

    void listAirports(bool domesticMode) {
        cout << "\n" << BOLD_CYAN << " --- AVAILABLE " << (domesticMode ? "DOMESTIC" : "GLOBAL") << " DESTINATIONS --- " << RESET << "\n";
        cout << BOLD_BLUE << " ---------------------------------------------" << RESET << "\n";
        
        int displayId = 1; 
        for(const auto& a : airports) {
            bool shouldDisplay = domesticMode ? a.isDomestic() : a.isInternational();
            if(shouldDisplay) {
                cout << "   " << BOLD_YELLOW << "[" << setw(2) << displayId << "]" << RESET 
                     << " " << BOLD_WHITE << left << setw(22) << a.getCity() << RESET 
                     << " " << BOLD_CYAN << "(" << a.getCode() << ")" << RESET << "\n";
                displayId++;
            }
        }
        cout << BOLD_BLUE << " ---------------------------------------------" << RESET << "\n";
    }

    int getActualAirportId(int displayId, bool domesticMode) {
        int current = 1;
        for (const auto& a : airports) {
            bool shouldDisplay = domesticMode ? a.isDomestic() : a.isInternational();
            if (shouldDisplay) {
                if (current == displayId) return a.getId();
                current++;
            }
        }
        return -1; 
    }
};

void printHeader() {
    cout << "\n";
    cout << BOLD_MAGENTA << "  ██╗   ██╗ █████╗  █████╗ ██╗   ██╗██╗   ██╗\n";
    cout << BOLD_RED     << "  ██║   ██║██╔══██╗██╔══██╗╚██╗ ██╔╝██║   ██║\n";
    cout << BOLD_YELLOW  << "  ██║   ██║███████║███████║ ╚████╔╝ ██║   ██║\n";
    cout << BOLD_GREEN   << "  ╚██╗ ██╔╝██╔══██║██╔══██║  ╚██╔╝  ██║   ██║\n";
    cout << BOLD_CYAN    << "   ╚████╔╝ ██║  ██║██║  ██║   ██║   ╚██████╔╝\n";
    cout << BOLD_BLUE    << "    ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝   ╚═╝    ╚═════╝ \n" << RESET;
    
    cout << "\n";
    cout << BOLD_WHITE << "               " << BOLD_YELLOW << "✈ WELCOME TO VAAYU ✈\n" << RESET;
    cout << BOLD_BLUE << " ====================================================\n" << RESET;
}

int main() {
    // Tell the Windows terminal to use UTF-8 encoding
    SetConsoleOutputCP(CP_UTF8);

    VaayuSystem fs;
    printHeader();
    
    fs.initializeAirports();
    fs.loadFlightsFromCSV("flights.csv");

    while (true) {
        int menuChoice;
        cout << "\n" << BOLD_MAGENTA << " [ MAIN MENU ]" << RESET << "\n";
        cout << BOLD_WHITE << "  [1] Search Flights\n";
        cout << BOLD_WHITE << "  [2] Exit System\n";
        cout << BOLD_CYAN << "  [>] Enter choice: " << BOLD_YELLOW;
        cin >> menuChoice;

        if (menuChoice == 2) {
            cout << "\n" << BOLD_GREEN << " [*] Terminating VAAYU System. Process completed." << RESET << "\n\n";
            break; // Exits the loop and ends the program
        }

        if (menuChoice != 1) {
            cout << "\n" << BOLD_RED << " [!] Invalid input. Please enter 1 or 2." << RESET << "\n";
            continue; // Skips the rest of the loop and asks again
        }

        int pType, sType, tType, pref, startDisplayID, endDisplayID;
        double bal;
        
        cout << "\n" << BOLD_WHITE << " [?] Passenger Type " << RESET << "(1: Frequent Flyer, 2: Guest): " << BOLD_YELLOW; 
        cin >> pType; 
        
        cout << BOLD_WHITE << " [?] Wallet Balance " << RESET << "(Rs.): " << BOLD_GREEN; 
        cin >> bal; 
        
        cout << BOLD_WHITE << " [?] Seat Type      " << RESET << "(1: Economy, 2: Business, 3: First): " << BOLD_YELLOW; 
        cin >> sType; 
        
        cout << BOLD_WHITE << " [?] Travel Type    " << RESET << "(1: Domestic, 2: International): " << BOLD_YELLOW; 
        cin >> tType; cout << RESET;

        bool domesticMode = (tType == 1);
        fs.listAirports(domesticMode);

        cout << "\n" << BOLD_WHITE << " [>] Enter Origin ID      : " << BOLD_YELLOW; cin >> startDisplayID;
        cout << BOLD_WHITE << " [>] Enter Destination ID : " << BOLD_YELLOW; cin >> endDisplayID;
        cout << BOLD_WHITE << " [>] Priority             " << RESET << "(1: Time Saving, 2: Economic): " << BOLD_YELLOW; cin >> pref; cout << RESET;

        Passenger* activePassenger = nullptr;
        if (pType == FREQUENT_FLYER) {
            activePassenger = new FrequentFlyer(bal, (SeatType)sType);
        } else {
            activePassenger = new Guest(bal, (SeatType)sType);
        }
        
        int trueStartID = fs.getActualAirportId(startDisplayID, domesticMode);
        int trueEndID = fs.getActualAirportId(endDisplayID, domesticMode);

        cout << "\n" << BOLD_MAGENTA << " [*] Calculating optimal routes using live CSV vectors..." << RESET << "\n";
        
        fs.search(trueStartID, trueEndID, activePassenger, (SortPreference)pref);

        delete activePassenger;
    }

    return 0;
}