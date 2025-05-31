#include <iostream>      // For std::cout, std::cin, std::endl
#include <iomanip>       // For std::setprecision, std::fixed, std::setw, std::left
#include <fstream>       // For file operations: std::ifstream, std::ofstream
#include <limits>        // For std::numeric_limits
#include <vector>        // For std::vector
#include <string>        // For std::string, std::getline, std::to_string
#include <sstream>       // For std::stringstream
#include <algorithm>     // For std::remove_if, std::sort
#include <cctype>        // For toupper
#include <cmath>         // For round in displayAverageMarksBarGraph

// Conditional include for Windows specific console settings
#ifdef _WIN32
#include <windows.h> // Required for SetConsoleOutputCP
#endif

using namespace std;

// Helper function to print an ASCII bar graph for a given integer value (character ASCII) with color coding
void printAsciiBarGraph(int ascii_val, int mark) {
    if (ascii_val < 0 || ascii_val > 127) { // Standard ASCII range
        cout << " (Value out of standard ASCII range)";
        return;
    }
    
    // Define color codes
    const string RED = "\033[31m";    // Red for marks < 50
    const string YELLOW = "\033[33m"; // Yellow for marks 50-75
    const string GREEN = "\033[32m";  // Green for marks > 75
    const string RESET = "\033[0m";   // Reset color

    // Choose color based on the mark
    string color;
    if (mark < 50) {
        color = RED;
    } else if (mark < 75) {
        color = YELLOW;
    } else {
        color = GREEN;
    }

    string bar = "";
    int scale_factor = 2; // Adjust to make the bars longer or shorter
    for (int i = 0; i < ascii_val / 5; ++i) { // Divide by 5 to keep bars manageable length
        bar += "#";
    }
    
    cout << color << bar << RESET; // Print the bar in the chosen color
}

class Student {
private:
    string name;
    string roll_no;
    int C;
    int Cpp;
    int Maths1;
    int Maths2;
    int Physics;
    int Chemistry;
    int Problem_solving;
    int Communication;
    int BEEE;
    int BME;

public:
    // Default constructor
    Student() : name(""), roll_no(""), C(0), Cpp(0), Maths1(0), Maths2(0), Physics(0),
                Chemistry(0), Problem_solving(0), Communication(0), BEEE(0), BME(0) {}

    // Input details - Modification here for roll number input
    // This method will now take the existing students vector to check for duplicates.
    void inputDetailsAndMarks(const vector<Student>& existingStudents) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer before getline
        cout << "Enter student name: ";
        getline(cin, name);

        // --- Start of Roll Number Validation Logic ---
        bool rollNoExists;
        do {
            rollNoExists = false;
            cout << "Enter student roll number: ";
            getline(cin, roll_no);

            // Check if the entered roll number already exists
            for (const auto& s : existingStudents) {
                if (s.getRollNo() == roll_no) {
                    cout << "Error: Roll number '" << roll_no << "' already exists. Please enter a unique roll number." << endl;
                    rollNoExists = true;
                    break;
                }
            }
        } while (rollNoExists);
        // --- End of Roll Number Validation Logic ---

        getAllMarks();
    }

    // Function to get all marks (used for initial input)
    void getAllMarks() {
        auto getValidMark = [](const string& subjectName) {
            float mark;
            while (true) {
                cout << "Enter marks of student in " << subjectName << ": ";
                cin >> mark;

                if (cin.fail() || mark < 0 || mark > 100) {
                    cout << "Invalid input. Please enter a number between 0 and 100." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                } else {
                    return static_cast<int>(mark); // Store as int as per class members
                }
            }
        };

        C = getValidMark("C language");
        Cpp = getValidMark("Cpp language");
        Maths1 = getValidMark("Maths1");
        Maths2 = getValidMark("Maths2");
        Physics = getValidMark("Physics");
        Chemistry = getValidMark("Chemistry");
        Problem_solving = getValidMark("Problem Solving");
        Communication = getValidMark("Communication");
        BEEE = getValidMark("BEEE");
        BME = getValidMark("BME");
    }

    // Function to allow editing specific marks
    void editSpecificMark() {
        cout << "\nEditing marks for student: " << name << " (Roll No: " << roll_no << ")" << endl;
        string subChoiceStr;
        do {
            cout << "\nSelect subject to edit:\n";
            cout << " 1. C Language\n";
            cout << " 2. Cpp Language\n";
            cout << " 3. Maths1\n";
            cout << " 4. Maths2\n";
            cout << " 5. Physics\n";
            cout << " 6. Chemistry\n";
            cout << " 7. Problem Solving\n";
            cout << " 8. Communication\n";
            cout << " 9. BEEE\n";
            cout << "10. BME\n";
            cout << "B. Back to main edit menu\n";
            cout << "Enter choice: ";
            cin >> subChoiceStr;

            auto getValidMarkInput = []() {
                float mark;
                while (true) {
                    cout << "Enter new mark (0-100): ";
                    cin >> mark;
                    if (cin.fail() || mark < 0 || mark > 100) {
                        cout << "Invalid input. Please enter a number between 0 and 100." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    } else {
                        return static_cast<int>(mark); // Store as int
                    }
                }
            };

            if (subChoiceStr == "1") { C = getValidMarkInput(); cout << "C mark updated.\n"; }
            else if (subChoiceStr == "2") { Cpp = getValidMarkInput(); cout << "Cpp mark updated.\n"; }
            else if (subChoiceStr == "3") { Maths1 = getValidMarkInput(); cout << "Maths1 mark updated.\n"; }
            else if (subChoiceStr == "4") { Maths2 = getValidMarkInput(); cout << "Maths2 mark updated.\n"; }
            else if (subChoiceStr == "5") { Physics = getValidMarkInput(); cout << "Physics mark updated.\n"; }
            else if (subChoiceStr == "6") { Chemistry = getValidMarkInput(); cout << "Chemistry mark updated.\n"; }
            else if (subChoiceStr == "7") { Problem_solving = getValidMarkInput(); cout << "Problem Solving mark updated.\n"; }
            else if (subChoiceStr == "8") { Communication = getValidMarkInput(); cout << "Communication mark updated.\n"; }
            else if (subChoiceStr == "9") { BEEE = getValidMarkInput(); cout << "BEEE mark updated.\n"; }
            else if (subChoiceStr == "10") { BME = getValidMarkInput(); cout << "BME mark updated.\n"; }
            else if (toupper(subChoiceStr[0]) == 'B' && subChoiceStr.length() == 1) {
                            cout << "Returning to previous menu.\n";
            }
            else { cout << "Invalid subject choice. Please try again.\n"; }

            if (subChoiceStr != "B" && toupper(subChoiceStr[0]) != 'B') {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

        } while (!(toupper(subChoiceStr[0]) == 'B' && subChoiceStr.length() == 1));
    }


    string calculateGrade(float score) const {
        if (score >= 90) return "A";
        else if (score >= 80) return "B";
        else if (score >= 70) return "C";
        else if (score >= 60) return "D";
        else if (score >= 50) return "E";
        else return "F";
    }

    float calculateAverageMarks() const {
        return (C + Cpp + Maths1 + Maths2 + Physics + Chemistry +
                Problem_solving + Communication + BEEE + BME) / 10.0;
    }

    int calcBacklogCount() const {
        int backlogs = 0;
        if (calculateGrade(C) == "F") backlogs++;
        if (calculateGrade(Cpp) == "F") backlogs++;
        if (calculateGrade(Maths1) == "F") backlogs++;
        if (calculateGrade(Maths2) == "F") backlogs++;
        if (calculateGrade(Physics) == "F") backlogs++;
        if (calculateGrade(Chemistry) == "F") backlogs++;
        if (calculateGrade(Problem_solving) == "F") backlogs++;
        if (calculateGrade(Communication) == "F") backlogs++;
        if (calculateGrade(BEEE) == "F") backlogs++;
        if (calculateGrade(BME) == "F") backlogs++;
        return backlogs;
    }

    string getRollNo() const {
        return roll_no;
    }

    string getName() const {
        return name;
    }

    // Helper to get ASCII string for a mark
    string getMarkAsciiString(float mark) const {
        stringstream ss;
        ss << fixed << setprecision(0) << mark; // Format to 0 decimal places for marks like 85
        string mark_str = ss.str();
        string ascii_output = "[";
        for (size_t i = 0; i < mark_str.length(); ++i) {
            ascii_output += to_string(static_cast<int>(mark_str[i]));
            if (i < mark_str.length() - 1) {
                ascii_output += ", ";
            }
        }
        ascii_output += "]";
        return ascii_output;
    }

    // Consolidated function to display marks, grades, and ASCII values for a single student
    void displayFullReport(ostream& os) const {
        os << "\n--- Student Full Report for " << name << " (Roll No: " << roll_no << ") ---" << endl;
        os << string(70, '-') << endl;
        os << left << setw(20) << "Subject"
           << setw(10) << "Marks"
           << setw(10) << "Grade"
           << setw(30) << "ASCII Values" << endl;
        os << string(70, '-') << endl;

        auto printRow = [&](const string& subjName, float mark) {
            os << left << setw(20) << subjName
               << setw(10) << mark
               << setw(10) << calculateGrade(mark)
               << setw(30) << getMarkAsciiString(mark) << endl;
        };

        printRow("C", C);
        printRow("Cpp", Cpp);
        printRow("Maths1", Maths1);
        printRow("Maths2", Maths2);
        printRow("Physics", Physics);
        printRow("Chemistry", Chemistry);
        printRow("Problem Solving", Problem_solving);
        printRow("Communication", Communication);
        printRow("BEEE", BEEE);
        printRow("BME", BME);
        os << string(70, '-') << endl;

        float avg = calculateAverageMarks();
        os << "\nOverall Average Marks: " << fixed << setprecision(2) << avg << endl;
        os << "Overall Grade: " << calculateGrade(avg) << endl;
        os << "Total Backlogs: " << calcBacklogCount() << endl;
        os << "\n----------------------------------------\n";
    }

    // Function to display average marks and its ASCII value
    void displayAverageMarksASCII(ostream& os) const {
        float avg = calculateAverageMarks();
        os << left << setw(20) << name
           << setw(15) << roll_no
           << fixed << setprecision(2) << setw(15) << avg
           << setw(30) << getMarkAsciiString(avg) << endl;
    }

    // Function to display ASCII bar graph for average marks with color coding
    void displayAverageMarksBarGraph(ostream& os) const {
        os << "\n--- ASCII Bar Graph for Average Marks: " << name << " (Roll No: " << roll_no << ") ---" << endl;
        float avg = calculateAverageMarks();
        os << "Average Marks: " << fixed << setprecision(2) << avg << endl;

        string avg_str = to_string(static_cast<int>(round(avg))); // Convert average to integer part for ASCII
        os << "ASCII values of average (" << avg_str << "):" << endl;

        for (char c : avg_str) {
            os << "  '" << c << "' (ASCII " << static_cast<int>(c) << "): ";
            printAsciiBarGraph(static_cast<int>(c), static_cast<int>(avg)); // Pass the average mark for color
            os << endl;
        }
        os << "---------------------------------------------------------\n";
    }


    // Function to save student data to the main data file in a parsable format
    void saveToFile(ofstream& outFile) const {
        outFile << name << endl;
        outFile << roll_no << endl;
        outFile << C << endl;
        outFile << Cpp << endl;
        outFile << Maths1 << endl;
        outFile << Maths2 << endl;
        outFile << Physics << endl;
        outFile << Chemistry << endl;
        outFile << Problem_solving << endl;
        outFile << Communication << endl;
        outFile << BEEE << endl;
        outFile << BME << endl;
    }

    // Function to load student data from the main data file
    bool loadFromFile(ifstream& inFile) {
        if (!getline(inFile, name)) return false;
        if (!getline(inFile, roll_no)) return false;

        float temp_mark; // Read into float then cast to int
        if (!(inFile >> temp_mark)) return false; C = static_cast<int>(temp_mark);
        if (!(inFile >> temp_mark)) return false; Cpp = static_cast<int>(temp_mark);
        if (!(inFile >> temp_mark)) return false; Maths1 = static_cast<int>(temp_mark);
        if (!(inFile >> temp_mark)) return false; Maths2 = static_cast<int>(temp_mark);
        if (!(inFile >> temp_mark)) return false; Physics = static_cast<int>(temp_mark);
        if (!(inFile >> temp_mark)) return false; Chemistry = static_cast<int>(temp_mark);
        if (!(inFile >> temp_mark)) return false; Problem_solving = static_cast<int>(temp_mark);
        if (!(inFile >> temp_mark)) return false; Communication = static_cast<int>(temp_mark);
        if (!(inFile >> temp_mark)) return false; BEEE = static_cast<int>(temp_mark);
        if (!(inFile >> temp_mark)) return false; BME = static_cast<int>(temp_mark);

        inFile.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume the rest of the line
        return true;
    }

    // Accessors for consolidated display (getting integer marks)
    int getC() const { return C; }
    int getCpp() const { return Cpp; }
    int getMaths1() const { return Maths1; }
    int getMaths2() const { return Maths2; }
    int getPhysics() const { return Physics; }
    int getChemistry() const { return Chemistry; }
    int getProblemSolving() const { return Problem_solving; }
    int getCommunication() const { return Communication; }
    int getBEEE() const { return BEEE; }
    int getBME() const { return BME; }

    // Suggest Specialization method
    void suggestSpecialization() const {
        cout << "\n--- Specialization Suggestion for " << name << " (Roll No: " << roll_no << ") ---\n";
        float avg = calculateAverageMarks();
        int backlogs = calcBacklogCount();

         if (avg >= 85 && Cpp >= 90 && Problem_solving >= 85) {
            cout << "Based on your strong performance in C++, Problem Solving, and overall average," << endl;
            cout << "Consider pursuing your bachelors in AIML or FULL STACKS DEVELOPMENT." << endl;
        } else if (avg >= 80 && Maths1 >= 85 && Maths2 >= 85 && Problem_solving >= 80) {
            cout << "With excellent marks in Mathematics and Problem Solving, " << endl;
            cout << "Consider pursuing your bachelors in DATA SCIENCE." << endl;
        } else if (avg >= 75 && Physics >= 80 && BEEE >= 75 && BME >= 75) {
            cout << "Your good performance in Physics, BEEE, and BME suggests," << endl;
            cout << "Consider pursuing your career in IOT AND ELCECTRONICS or SEMICONDUCTORS." << endl;
        } else if (avg <75 ) {
            cout << "Based on your overall performance," << endl;
            cout << "Consider pursuing your bachelors in CYBERSECURITY." << endl;
        }
        cout << "-----------------------------------------------------\n";
    }
};

// Function to read all student records from the main data file
vector<Student> loadAllStudentsFromFile(const string& filename) {
    vector<Student> students;
    ifstream inFile(filename);

    if (!inFile.is_open()) {
        cerr << "Warning: Could not open data file '" << filename << "' for reading. Attempting to create it." << endl;
        ofstream create_file(filename); // Try to create the file
        if (!create_file.is_open()) {
            cerr << "Error: Failed to create data file '" << filename << "'." << endl;
        }
        create_file.close(); // Close the newly created file
        return students;
    }

    Student s;
    while (inFile.peek() != EOF) { // Check if not at end of file
        if (s.loadFromFile(inFile)) {
            students.push_back(s);
        } else {
            // If a record is malformed, clear error flags and try to move past it
            inFile.clear();
            string dummy;
            getline(inFile, dummy); // Read and discard the rest of the current line
            cerr << "Warning: Skipping potentially malformed record in data file. Data might be corrupted." << endl;
        }
    }
    inFile.close();
    return students;
}

// Save all student records to the main data file (overwrite)
void saveAllStudentsToFile(const vector<Student>& students, const string& filename) {
    ofstream outFile(filename, ios::trunc); // ios::trunc ensures file is cleared before writing
    if (!outFile.is_open()) {
        cerr << "Error opening data file '" << filename << "' for writing!" << endl;
        return;
    }

    for (const auto& s : students) {
        s.saveToFile(outFile);
    }
    outFile.close();
}

// Function to display all student data in a single table (console or file)
void displayAllStudentsInTable(const vector<Student>& students, ostream& os) {
    if (students.empty()) {
        os << "No student data available to display." << endl;
        return;
    }

    os << "\n--- Consolidated Student Marks Report ---" << endl;
    os << string(150, '=') << endl;

    // Original, compact headers
    os << left << setw(20) << "Name"
           << setw(15) << "Roll No"
           << setw(7) << "C"
           << setw(7) << "Cpp"
           << setw(7) << "Math1"
           << setw(7) << "Math2"
           << setw(7) << "Phy"
           << setw(7) << "Chem"
           << setw(7) << "PS"
           << setw(7) << "Comm"
           << setw(7) << "BEEE"
           << setw(7) << "BME"
           << setw(10) << "Avg"
           << setw(7) << "Grade"
           << setw(10) << "Backlogs" << endl;
    os << string(150, '=') << endl;

    for (const auto& s : students) {
        os << left << setw(20) << s.getName()
                << setw(15) << s.getRollNo()
                << setw(7) << s.getC()
                << setw(7) << s.getCpp()
                << setw(7) << s.getMaths1()
                << setw(7) << s.getMaths2()
                << setw(7) << s.getPhysics()
                << setw(7) << s.getChemistry()
                << setw(7) << s.getProblemSolving()
                << setw(7) << s.getCommunication()
                << setw(7) << s.getBEEE()
                << setw(7) << s.getBME()
                << fixed << setprecision(2) << setw(10) << s.calculateAverageMarks()
                << setw(7) << s.calculateGrade(s.calculateAverageMarks())
                << setw(10) << s.calcBacklogCount() << endl;
    }
    os << string(150, '=') << endl;
}

// Display All Students' Marks with ASCII Values
void displayAllStudentsMarksWithASCII(const vector<Student>& students, ostream& os) {
    if (students.empty()) {
        os << "No student data available to display ASCII values." << endl;
        return;
    }

    os << "\n--- Detailed Student Marks Report with ASCII Values ---" << endl;
    os << "This report shows individual subject marks with their ASCII representations." << endl;
    os << string(80, '=') << endl;

    for (const auto& s : students) {
        s.displayFullReport(os); // This function already formats well with ASCII
    }
    os << string(80, '=') << endl;
    os << "End of Detailed Report." << endl;
}

// Display All Students' Average Marks with ASCII Values
void displayAllStudentsAverageMarksASCII(const vector<Student>& students, ostream& os) {
    if (students.empty()) {
        os << "No student data available to display average marks with ASCII values." << endl;
        return;
    }

    os << "\n--- Student Average Marks with ASCII Values ---" << endl;
    os << string(80, '=') << endl;
    os << left << setw(20) << "Name"
       << setw(15) << "Roll No"
       << setw(15) << "Average Marks"
       << setw(30) << "ASCII Values" << endl;
    os << string(80, '=') << endl;

    for (const auto& s : students) {
        s.displayAverageMarksASCII(os);
    }
    os << string(80, '=') << endl;
    os << "End of Average Marks ASCII Report." << endl;
}


// Function to save the full report to a separate file (this now uses the compact table)
void saveFullReportToFile(const vector<Student>& students, const string& reportFilename) {
    ofstream reportFile(reportFilename, ios::trunc);
    if (!reportFile.is_open()) {
        cerr << "Error opening report file '" << reportFilename << "' for writing!" << endl;
        return;
    }

    reportFile << "--- STUDENT PERFORMANCE REPORT ---" << endl;
 //   reportFile << "Generated on: " << DATE << " at " << TIME << endl;
    reportFile << string(40, '=') << endl << endl;

    // Call the unified table display function, directing its output to the file
    displayAllStudentsInTable(students, reportFile); // Using the compact table

    reportFile.close();
    cout << "\nFull student report (compact table) saved to '" << reportFilename << "'." << endl;
}

// Save detailed ASCII report to a separate file
void saveASCIIReportToFile(const vector<Student>& students, const string& asciiReportFilename) {
    ofstream asciiReportFile(asciiReportFilename, ios::trunc);
    if (!asciiReportFile.is_open()) {
        cerr << "Error opening ASCII report file '" << asciiReportFilename << "' for writing!" << endl;
        return;
    }

    asciiReportFile << "--- DETAILED STUDENT MARKS WITH ASCII REPORT ---" << endl;
   // asciiReportFile << "Generated on: " << DATE << " at " << TIME << endl;
    asciiReportFile << string(60, '=') << endl << endl;

    displayAllStudentsMarksWithASCII(students, asciiReportFile); // Use the new function for file output

    asciiReportFile.close();
    cout << "\nDetailed ASCII marks report saved to '" << asciiReportFilename << "'." << endl;
}


// Function to display the leaderboard (ranked by average marks)
void displayLeaderboard(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No student data available to generate a leaderboard." << endl;
        return;
    }

    // Create a copy of students and sort them by average marks in descending order
    vector<Student> sortedStudents = students;
    sort(sortedStudents.begin(), sortedStudents.end(), [](const Student& a, const Student& b) {
        return a.calculateAverageMarks() > b.calculateAverageMarks();
    });

    cout << "\n--- Leaderboard (Ranked by Average Marks) ---" << endl;
    cout << string(65, '-') << endl;
    // Adjusted setw for Name to accommodate emojis
    cout << left << setw(7) << "Rank"
         << setw(27) << "Name" // Increased width slightly for emojis
         << setw(15) << "Roll No"
         << fixed << setprecision(2) << setw(15) << "Average Marks" << endl;
    cout << string(65, '-') << endl;

    for (int i = 0; i < sortedStudents.size(); ++i) {
        string displayName = sortedStudents[i].getName();
        if (i == 0) {
            displayName = "⭐ " + displayName; // Star for 1st rank
        } else if (i == 1) {
            displayName = "🌟 " + displayName; // Star for 2nd rank
        } else if (i == 2) {
            displayName = "✨ " + displayName; // Star for 3rd rank
        }

        cout << left << setw(7) << (i + 1)
             << setw(27) << displayName // Use the modified display name with adjusted width
             << setw(15) << sortedStudents[i].getRollNo()
             << setw(15) << sortedStudents[i].calculateAverageMarks() << endl;
    }
    cout << string(65, '-') << endl;
}


// Function to delete a student by roll number
void deleteStudent(vector<Student>& students, const string& dataFilename) {
    if (students.empty()) {
        cout << "No student data available to delete." << endl;
        return;
    }

    string rollToDelete;
    cout << "Enter roll number of student to delete: ";
    getline(cin, rollToDelete);

    auto originalSize = students.size();
    students.erase(std::remove_if(students.begin(), students.end(),
                                   [&](const Student& s) {
                                       return s.getRollNo() == rollToDelete;
                                   }),
                   students.end());

    if (students.size() < originalSize) {
        cout << "Student with Roll No: " << rollToDelete << " deleted successfully." << endl;
        saveAllStudentsToFile(students, dataFilename);
    } else {
        cout << "Student with roll number " << rollToDelete << " not found." << endl;
    }
}

// NEW FUNCTION: Display students recommended for mentorship
void displayMentorshipRecommendations(const vector<Student>& students) {
    cout << "\n--- Students Recommended for Mentorship (Average Marks < 50) ---\n";
    cout << string(60, '-') << endl;
    cout << left << setw(25) << "Name"
         << setw(15) << "Roll No"
         << setw(15) << "Average Marks" << endl;
    cout << string(60, '-') << endl;

    bool found = false;
    for (const auto& s : students) {
        if (s.calculateAverageMarks() < 50.0) {
            cout << left << setw(25) << s.getName()
                 << setw(15) << s.getRollNo()
                 << fixed << setprecision(2) << setw(15) << s.calculateAverageMarks() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No students currently recommended for mentorship." << endl;
    }
    cout << string(60, '-') << endl;
}

// NEW FUNCTION: Display students eligible for Dean's List
void displayDeansList(const vector<Student>& students) {
    cout << "\n--- Dean's List (Average Marks > 85) ---\n";
    cout << string(60, '-') << endl;
    cout << left << setw(25) << "Name"
         << setw(15) << "Roll No"
         << setw(15) << "Average Marks" << endl;
    cout << string(60, '-') << endl;

    bool found = false;
    for (const auto& s : students) {
        if (s.calculateAverageMarks() > 85.0) {
            cout << left << setw(25) << s.getName()
                 << setw(15) << s.getRollNo()
                 << fixed << setprecision(2) << setw(15) << s.calculateAverageMarks() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No students currently eligible for the Dean's List." << endl;
    }
    cout << string(60, '-') << endl;
}


int main() {
    // Enable UTF-8 output for Windows console
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    // Optional: Set input code page if you expect UTF-8 input, though less common for simple inputs.
    // SetConsoleCP(CP_UTF8);
    #endif

    vector<Student> students;
    const string dataFilename = "student_data.txt";
    const string reportFilename = "student_full_report.txt"; // For the compact table
    const string asciiReportFilename = "student_ascii_report.txt"; // New file for ASCII details
    const string avgAsciiReportFilename = "student_avg_ascii_report.txt"; // New file for average ASCII details

    students = loadAllStudentsFromFile(dataFilename);

    string mainChoice; // Change to string to handle "10" input easily
    do {
        cout << "\n--- Student Management System Menu ---\n";
        cout << "1. Add New Student\n";
        cout << "2. Edit Existing Student\n";
        cout << "3. Display All Students (Compact Table)\n";
        cout << "4. Display All Students' Marks with ASCII (Detailed)\n";
        cout << "5. Display All Students' Average Marks with ASCII\n";
        cout << "6. View ASCII Bar Graph for Student's Average Marks\n";
        cout << "7. Suggest Specialization for a Student\n";
        cout << "8. Display Leaderboard\n";
        cout << "9. Mentorship Recommendations\n"; // NEW OPTION
        cout << "10. Dean's List\n";              // NEW OPTION
        cout << "11. Delete Student\n";            // Shifted delete
        cout << "12. Exit\n";                      // Shifted exit
        cout << "Enter choice: ";
        cin >> mainChoice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Consume newline

        if (mainChoice == "1") {
            Student s;
            s.inputDetailsAndMarks(students);
            students.push_back(s);
            saveAllStudentsToFile(students, dataFilename);
            cout << "\nStudent added and data saved to '" << dataFilename << "'." << endl;
        }
        else if (mainChoice == "2") {
            if (students.empty()) {
                cout << "No student data available to edit." << endl;
                continue;
            }
            string roll;
            cout << "Enter roll number of student to edit: ";
            getline(cin, roll);

            bool found = false;
            for (auto& s : students) {
                if (s.getRollNo() == roll) {
                    s.editSpecificMark();
                    saveAllStudentsToFile(students, dataFilename);
                    found = true;
                    cout << "Student with Roll No: " << roll << " updated successfully." << endl;
                    break;
                }
            }
            if (!found) {
                cout << "Student with roll number " << roll << " not found." << endl;
            }
        }
        else if (mainChoice == "3") {
            displayAllStudentsInTable(students, cout); // Display compact table on console
            saveFullReportToFile(students, reportFilename); // Save the compact table to file
        }
        else if (mainChoice == "4") {
            displayAllStudentsMarksWithASCII(students, cout); // Display detailed ASCII report on console
            saveASCIIReportToFile(students, asciiReportFilename); // Save detailed ASCII report to new file
        }
        else if (mainChoice == "5") {
            displayAllStudentsAverageMarksASCII(students, cout); // Display on console
            ofstream avgAsciiReportFile(avgAsciiReportFilename, ios::trunc);
            if (avgAsciiReportFile.is_open()) {
                displayAllStudentsAverageMarksASCII(students, avgAsciiReportFile);
                avgAsciiReportFile.close();
                cout << "\nAverage marks ASCII report saved to '" << avgAsciiReportFilename << "'." << endl;
            } else {
                cerr << "Error opening average marks ASCII report file '" << avgAsciiReportFilename << "' for writing!" << endl;
            }
        }
        else if (mainChoice == "6") {
            if (students.empty()) {
                cout << "No student data available to generate a bar graph." << endl;
                continue;
            }
            string roll;
            cout << "Enter roll number of student to view average marks bar graph: ";
            getline(cin, roll);

            bool found = false;
            for (const auto& s : students) {
                if (s.getRollNo() == roll) {
                    s.displayAverageMarksBarGraph(cout);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Student with roll number " << roll << " not found." << endl;
            }
        }
        else if (mainChoice == "7") {
            if (students.empty()) {
                cout << "No student data available for specialization suggestion." << endl;
                continue;
            }
            string roll;
            cout << "Enter roll number of student to suggest specialization for: ";
            getline(cin, roll);

            bool found = false;
            for (const auto& s : students) {
                if (s.getRollNo() == roll) {
                    s.suggestSpecialization();
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Student with roll number " << roll << " not found." << endl;
            }
        }
        else if (mainChoice == "8") {
            displayLeaderboard(students);
        }
        else if (mainChoice == "9") { // NEW: Mentorship Recommendations
            displayMentorshipRecommendations(students);
        }
        else if (mainChoice == "10") { // NEW: Dean's List
            displayDeansList(students);
        }
        else if (mainChoice == "11") { // Shifted delete option
            deleteStudent(students, dataFilename);
        }
        else if (mainChoice != "12") { // Shifted exit option
            cout << "Invalid choice. Please enter a number between 1 and 12." << endl;
        }

    } while (mainChoice != "12");

    cout << "\nExiting program. All raw student data saved to '" << dataFilename << "'." << endl;
    return 0;
}
