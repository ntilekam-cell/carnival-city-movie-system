#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <cassert>
#include <algorithm>
#include <iomanip>

using namespace std;

const int MAX_RECORDS = 100;

// Class representing a client's Reserveation
class Client {
private:
        string surname;
        string name;
        string movieTitle;
        string movieType;
        string seatNumber;
        double price;
        string refNumber;


public:
       // Default Contructor
       Client() : surname(""), name(""), movieTitle(""), movieType(""), seatNumber(""), price(0.0), refNumber(""){}

       // Parameterized Contructor
       Client(string s, string n, string mt, string mType, string seat, double p, string ref)
            : surname(s), name(n), movieTitle(mt), movieType(mType), seatNumber(seat), price(p), refNumber(ref){}

       
       // Getters
       string getSurname() const { return surname; }
       string getName() const { return name; }
       string getMovieTitle() const { return movieTitle; }
       string getMovieType() const { return movieType; }
       string getSeatNumber() const { return seatNumber; }
       double getPrice() const { return price; }
       string getRefNumber() const { return refNumber; }

       // Setters
       void setSurname( const string & s) { surname = s; }
       void setName( const string & n) { name = n; }
       void setMovieTitle( const string & mt) { movieTitle = mt; }
       void setSeatNumber( const string & seat) { seatNumber = seat; }
       void setPrice( double p) { price = p; }
       void setRefNumber( const string & ref) { refNumber = ref; }

};

// ---- Function Declarations -----
bool loadClientsFromFile( const string & filename, Client clients[], int & count);
int countLinesInFile( const string & filename);
int countWordsInFile( const string & filename);
int countCharOccurrences( const string & filename, char ch);
void characterSearchLoop( const string & filename);
void replaceFirstTwoDigits(string & refNum, const string & newDigits);
void replaceRefDigitsForClients( Client[], int count, const string & newDigits);
int countSpecificMovieType( const Client clients[], int count, const string & targetType);
double calculateTotalAmountByMovieType( const Client clients[], int count, const string & targetType);
double calculateTotalAmountAll( const Client clients[], int count);
void generateSortedReport( const  string & outputFilename, Client clients[], int count);
void displayMenu();


// Trim leading and trailing whitespace
string trim( const string & str) {

    size_t first = str.find_first_not_of(" \t\r\n");
    if ( first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Main Fuction
int main() {

    string inputFilename, outputFilename;

    cout << "==============================================================\n";
    cout << "               CARNIVAL CITY MOVIE EVENT SYSTEM               \n";
    cout << "==============================================================\n";
    cout << "Enter input file name ( e.g., clients_info.dat): ";
    cin >> inputFilename;
    cout << "Enter output file name ( e.g., movie_report.txt): ";
    cin >> outputFilename;

    Client clients[MAX_RECORDS];
    int clientCount = 0;

    // Load and validate file data
    if ( !loadClientsFromFile(inputFilename, clients, clientCount)) {

        cout << "Error opening or processing file: " << inputFilename << ".Program exiting.\n";
        return 1;
    }

    int choice;

    do {

        displayMenu();
        cout << "Enter choice (1 - 9): ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int lines = countLinesInFile(inputFilename);
                cout << "\nTotal number of lines in file: " << lines << "\n";
                break;
            }

            case 2: {
                int words = countWordsInFile(inputFilename);
                cout<< "\nTotal number of words in file: " << words << "\n";
                break;
            }

            case 3: {
                characterSearchLoop(inputFilename);
                break;
            }

            case 4: {
                string newDigits;
                cout << "Enter two new digits to replace the first two digits of the reference numbers: ";
                cin >> newDigits;

                if( newDigits.length() == 2 && isdigit(newDigits[0]) && isdigit(newDigits[1])) {

                    replaceRefDigitsForClients( clients, clientCount, newDigits);
                    cout << "First two digits of reference  numbers successfully updated!\n";

                }

                else {

                    cout << "Invalid input. Must enter exactly 2 numeric digits.\n";
                }

                break;
            }

            case 5: {

                string type;
                cout << "Enter Movie type ( Action, Horror, Animation...): ";
                cin >> type;

                int count = countSpecificMovieType( clients, clientCount, type);
                cout << "Total reservation for ' " << type << " ': " << count << "\n";
                break;
            }

            case 6: {

                string type;
                cout << "Enter Movie type ( Action, Horror, Animation...): ";
                cin >> type;

                double total = calculateTotalAmountByMovieType( clients, clientCount, type);
                cout << "Total amount paid for '" << type << "': R" << fixed << setprecision(2) << total << "\n";
                break;
            }

            case 7: {

                double grandTotal = calculateTotalAmountAll( clients, clientCount);
                cout << "Total amount of all the movie reservations: R" << fixed << setprecision(2) << grandTotal << "\n";
                break;
            }

            case 8: {

                generateSortedReport( outputFilename, clients, clientCount);
                cout << "Report successfully generated and saved to " << outputFilename << "\n";
                break;
            }

            case 9: {

                cout << "Exiting system. Goodbye!\n";
                break;
                default:
                     cout << "Invalid choice! Please select between 1 and 9.\n";
            }

        
        }
    }
     
    while (choice != 9);

    return 0;

}

// Display Menu Options
void displayMenu() {

    cout << "\n----------------------------- MENU OPTIONS -------------------------------\n";
    cout << "1. Count total lines in input file\n";
    cout << "2. Count total words in input file\n";
    cout << "3. Search occurrence of a character (with loop)\n";
    cout << "4. Replace first two digits of reference numbers\n";
    cout << "5. Count reservations for a specific movie type\n";
    cout << "6. Calculate total amount for a specific movie type\n";
    cout << "7. Calculate total amount for all movie reservatiolns\n";
    cout << "8. Generates sorted report to output file\n";
    cout << "9. Exit\n";
    cout << "\n--------------------------------------------------------------------------\n";
    
}

// Task 1: Read client data line by line / character by character with file validation
bool loadClientsFromFile( const string & filename, Client clients[], int & count) {

    ifstream inFile(filename);
    if (!inFile.is_open() || inFile.fail()) {

        return false;
    }

    count = 0;
    string line;
    while ( getline(inFile, line) && count < MAX_RECORDS) {

        if (line.empty()) continue;

        // Processing character by character parsing
        stringstream ss(line);
        string surname, name, title, type, seat, priceStr, refNum;

        getline(ss, surname, ',');
        getline(ss, name, ',');
        getline(ss, title, ',');
        getline(ss, type, ',');
        getline(ss, seat, ',');
        getline(ss, priceStr, ',');
        getline(ss, refNum, ',');

        double price = 0.0;
        try {

            price = stod(trim(priceStr));
        }
        catch (...) {
            price = 0.0;
        }

        clients[count] = Client(trim(surname), trim(name), trim(title), trim(type), trim(seat), price, trim(refNum));
        count++;
    }

    inFile.close();
    return true;
}

// Task 2: Count number of lines in input file
int countLinesInFile( const string & filename) {
    ifstream inFile(filename);

if (!inFile.is_open()) return 0;

int lineCount = 0;
string line;
while ( getline(inFile, line)) {

    lineCount++;
}
inFile.close();
return lineCount;

}

// Task 3: Counting Total words in input file( delimited by whitespace, commas, carriage returns, etc..)
int countWordsInFile( const string & filename) {
    ifstream inFile(filename);

    if (!inFile.is_open()) return 0;

    int wordCount = 0;
    bool inWord = false;
    char ch;

    while (inFile.get(ch)) {

        // Delimiters: spaces, commas, newlines, tabs, carriage returns
        if ( isspace(ch) || ch == ',') {
            inWord = false;
        }
        else if (!inWord) {
            inWord = true;
            wordCount++;
        }
    }

    inFile.close();
    return wordCount;
}

// Task 4a: Count character occurrences in file (case-sensitive)
int countCharOccurrences( const string & filename, char target){
    ifstream inFile(filename);

    if (!inFile.is_open()) return 0;

    int count = 0;
    char ch;
    char lowerTarget = tolower(target);

    while (inFile.get(ch)) {

        if (tolower(ch) == lowerTarget) {

            count++;
        }
    }
    
    inFile.close();
    return count;

}

// Task 4b: character search loop with assert check
void characterSearchLoop( const string & filename) {
    char answer = 'y';

    while ( answer == 'y' || answer == 'Y') {
        char searchChar;
        cout << "Enter character to search for: ";
        cin >> searchChar;

        int occurrences = countCharOccurrences(filename, searchChar);
        cout << "Character '" << searchChar << "' occurred" << occurrences  << " " "times in file.\n";

        cout << "Do you wish to search for another character?(y/n): ";
        cin >> answer;

        // Assert Check: Terminates program if the user enters anything other than 'y', 'Y', 'n', 'N',
        assert ( answer == 'y' || answer == 'Y' || answer == 'n' || answer == 'N');

    }
}

// Task 5: Fuction to replace the first two digits of a reference number
void replaceFirstTwoDigits( string & refNum, const string & newDigits) {
    if (refNum.length() >= 2 && newDigits.length() == 2) {

        refNum[0] = newDigits[0];
        refNum[1] = newDigits[1];
    }
}

void replaceRefDigitsForClients( Client clients[], int count, const string & newDigits) {
    for (int i = 0; i < count; i++) {

        string ref = clients[i].getRefNumber();
        replaceFirstTwoDigits(ref, newDigits);
        clients[i].setRefNumber(ref);

    }
}

// Task 6: Function to count specific movie type 
int countSpecificMovieType( const Client clients[], int count, const string & targetType) {

    int total = 0;
    for (int i = 0; i < count; i++) {

        if ( clients[i].getMovieType() == targetType) {

            total++;
        }
    }

    return total;
} 

// Task 7: Calculate total amount of each movie type
double calculateTotalAmountByMovieType ( const Client clients[], int count , const string & targetType) {
    double totalAmount = 0.0;
    for ( int i = 0; i < count; i++) {

        if ( clients[i].getMovieType() == targetType) {

            totalAmount += clients[i].getPrice();
        }
    }

    return totalAmount;

}

// Task 8: Calculate total amount of all movie reservations
double calculateTotalAmountAll( const Client clients[], int count) {
    double totalAmount = 0.0;
    for ( int i = 0; i < count; i++) {

        totalAmount += clients[i].getPrice();
    }

    return totalAmount;
}

// Task 9: Sort records descending by surname and output to movie_report.txt
void generateSortedReport ( const string  & outputFilename, Client clients[], int count) {

    // Selection sort descending by surname
    for ( int i = 0; i < count; i++) {

        int maxIndex = i;
        for ( int j = i + 1; j < count ; j++ ) {

            if ( clients[j].getSurname() > clients[maxIndex].getSurname()) {

                maxIndex = j;
            }
        }

        if (maxIndex != i) {

            swap(clients[i], clients[maxIndex]);
        }
    }

    ofstream outFile(outputFilename);
    if (!outFile.is_open()) {

        cout << "Error: Could not open output file for writing.\n";
        return;
    }

    // Pre-calculate movie type totals
double totalAction = calculateTotalAmountByMovieType( clients, count, "Action");
double totalHorror = calculateTotalAmountByMovieType(clients, count, "Horror");
double totalAnimation = calculateTotalAmountByMovieType( clients, count, "Animation");

outFile << left << setw(15) << "Surname"
        << setw(15) << "Name"
        << setw(15) << "Movie type"
        << setw(20) << "Reference"
        << setw(12) << "Price (R)" << "\n";
outFile << string (77, '-') << "\n";


for (int i = 0; i < count; i++) {

    outFile << left << setw(15) << clients[i].getSurname()
                    << setw(15) << clients[i].getName()
                    << setw(20) << clients[i].getRefNumber()
                    << fixed << setprecision(2) << setw(12) << clients[i].getPrice() << "\n";

}

outFile << "\n" << string(77, '=') << "\n";
outFile << "TOTAL AMOUNT PAID BY MOVIE TYPE:\n";
outFile << "Action total    : R" << fixed << setprecision(2) << totalAction << "\n";
outFile << "Action Horror   : R" << fixed << setprecision(2) << totalHorror << "\n";
outFile << "Animation       : R" << fixed << setprecision(2) << totalAnimation << "\n";
outFile << "GRNAND TOTAL    : R" << fixed << setprecision(2) << calculateTotalAmountAll(clients, count) << "\n";


outFile.close();
}









