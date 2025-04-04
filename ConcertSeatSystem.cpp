#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

class ConcertSeatSystem {
private:
    int rows, cols;
    double totalRevenue;
    vector<vector<bool> > seats;
    vector<vector<string> > names;

public:
    ConcertSeatSystem(int r = 10, int c = 10) : rows(r), cols(c), totalRevenue(0.0) {
        seats.resize(rows, vector<bool>(cols, false));
        names.resize(rows, vector<string>(cols, ""));
        loadSeatsFromFile();
    }

    ~ConcertSeatSystem() {
        saveSeatsToFile();
    }

    double getSeatPrice(int row) {
        if (row < 3) return 60.0; // VIP
        if (row < 7) return 40.0; // Regular
        return 20.0;              // Budget
    }

    string getSeatTier(int row) {
        if (row < 3) return "VIP";
        if (row < 7) return "Regular";
        return "Budget";
    }

    void displaySeats() {
    cout << "\n🎭 Concert Seat Map (O = Available, X = Taken)\n";
    cout << "   ";
    for (int col = 0; col < cols; ++col)
        cout << setw(3) << col;
    cout << "\n";

    for (int row = 0; row < rows; ++row) {
        cout << setw(2) << row << " ";
        for (int col = 0; col < cols; ++col) {
            if (seats[row][col]) {
                cout << "[\033[31mX\033[0m]"; // Red X
            } else {
                cout << "[O]";
            }
        }
        cout << "  ← " << getSeatTier(row) << "\n";
    }
}


    void buySeat() {
        int row, col;
        string buyerName;

        cout << "\nEnter seat row and column to buy (e.g., 2 3): ";
        cin >> row >> col;
        cin.ignore(); // flush newline
        if (row < 0 || row >= rows || col < 0 || col >= cols) {
            cout << "Invalid seat selection.\n";
            return;
        }

        if (seats[row][col]) {
            cout << "Seat already taken by " << names[row][col] << ". Please choose another.\n";
            return;
        }

        cout << "Enter your name: ";
        getline(cin, buyerName);

        double price = getSeatPrice(row);
        seats[row][col] = true;
        names[row][col] = buyerName;
        totalRevenue += price;

        cout << "Seat (" << row << ", " << col << ") purchased for $"
             << fixed << setprecision(2) << price << " by " << buyerName << ".\n";
    }

    void showStats() {
        int booked = 0;
        for (auto& row : seats)
            for (bool seat : row)
                if (seat) booked++;

        int total = rows * cols;
        cout << "\n📊 Seat Stats:\n";
        cout << "Total Seats: " << total << "\n";
        cout << "Booked Seats: " << booked << "\n";
        cout << "Available Seats: " << total - booked << "\n";
        cout << "Total Revenue: $" << fixed << setprecision(2) << totalRevenue << "\n";
    }

    void saveSeatsToFile() {
        ofstream file("seats.txt");
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                if (seats[i][j])
                    file << i << " " << j << " " << names[i][j] << "\n";
    }

    void loadSeatsFromFile() {
        ifstream file("seats.txt");
        if (!file) return;

        int row, col;
        string name;
        while (file >> row >> col) {
            getline(file, name);
            name = name.substr(1); // remove leading space
            seats[row][col] = true;
            names[row][col] = name;
            totalRevenue += getSeatPrice(row);
        }
    }

    void run() {
        int choice;
        do {
            cout << "\n🎫 Concert Ticket System Menu:\n";
            cout << "1. View Seat Map\n";
            cout << "2. Buy a Seat\n";
            cout << "3. View Stats\n";
            cout << "4. Enter Admin Mode\n";
            cout << "5. Save & Exit\n";
            cout << "Select an option: ";
            cin >> choice;

            switch (choice) {
                case 1: displaySeats(); break;
                case 2: buySeat(); break;
                case 3: showStats(); break;
                case 4: runAdminMode(); break;
                case 5: saveSeatsToFile(); cout << "Exiting...\n"; break;
                default: cout << "Invalid option.\n";
            }

        } while (choice != 5);
    }

    void runAdminMode() {
        string pin;
        cout << "Enter admin PIN: ";
        cin >> pin;

        if (pin != "1234") {
            cout << "Access denied.\n";
            return;
        }

        int option;
        do {
            cout << "\n🛠 Admin Panel:\n";
            cout << "1. View All Booked Seats\n";
            cout << "2. Reset All Seats\n";
            cout << "3. Export Report\n";
            cout << "4. Back to Main Menu\n";
            cout << "Choose: ";
            cin >> option;

            switch (option) {
                case 1: viewAllBookings(); break;
                case 2: resetAllSeats(); break;
                case 3: exportReport(); break;
                case 4: return;
                default: cout << "Invalid admin option.\n";
            }

        } while (option != 4);
    }

    void viewAllBookings() {
        cout << "\n📋 Booked Seats:\n";
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                if (seats[i][j])
                    cout << "Seat (" << i << "," << j << ") - " << names[i][j] << " - $" << getSeatPrice(i) << "\n";
    }

    void resetAllSeats() {
        char confirm;
        cout << "Are you sure you want to clear all bookings? (y/n): ";
        cin >> confirm;
        if (confirm != 'y' && confirm != 'Y') return;

        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j) {
                seats[i][j] = false;
                names[i][j] = "";
            }
        totalRevenue = 0.0;
        ofstream("seats.txt").close(); // wipe file
        cout << "All bookings reset.\n";
    }

    void exportReport() {
        ofstream report("report.txt");
        report << "📄 Ticket Sales Report\n";
        report << "--------------------------\n";
        for (int i = 0; i < rows; ++i)
            for (int j = 0; j < cols; ++j)
                if (seats[i][j])
                    report << "Seat (" << i << "," << j << ") - " << names[i][j]
                           << " - $" << getSeatPrice(i) << "\n";
        report << "\nTotal Revenue: $" << fixed << setprecision(2) << totalRevenue << "\n";
        report.close();
        cout << "Report saved to report.txt\n";
    }
};

int main() {
    ConcertSeatSystem system;
    system.run();
    return 0;
}
