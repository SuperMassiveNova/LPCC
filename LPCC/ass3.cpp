#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Function to generate Pool table from assembly code
vector<int> generatePoolTable(const string& assemblyCode) {
    vector<int> poolTable;
    int currentPoolStart = -1;
    int currentAddress = 0;

    // Split assembly code into lines
    stringstream ss(assemblyCode);
    string line;
    while (getline(ss, line, '\n')) {
        if (line.empty()) {
            continue;
        }

        stringstream lineStream(line);
        string token;
        lineStream >> token;

        // Check for LTORG directive to identify the end of a literal pool
        if (token == "LTORG") {
            if (currentPoolStart != -1) {
                // End of current pool, record the pool start address
                poolTable.push_back(currentPoolStart);
                currentPoolStart = -1;
            }
        } else if (token == "MOVER" || token == "MOVEM" || token == "ADD" || token == "COMP" || token == "SUB" || token == "PRINT") {
            string operand;
            while (lineStream >> operand) {
                size_t startQuotePos = operand.find("='");
                size_t endQuotePos = operand.find("'", startQuotePos + 2);
                if (startQuotePos != string::npos && endQuotePos != string::npos) {
                    // Literal found, check if it's the start of a new pool
                    if (currentPoolStart == -1) {
                        currentPoolStart = currentAddress; // Use current address as pool start
                    }
                }
            }
        }

        // Increment current address for each line
        currentAddress++;
    }

    // Check if there's an open pool at the end of the code
    if (currentPoolStart != -1) {
        poolTable.push_back(currentPoolStart);
    }

    return poolTable;
}

int main() {
    // Example Assembly Language Code
    string assemblyCode = R"(
START 100
READ A
MOVER AREG, ='1'
MOVEM AREG, B
MOVER BREG, ='6'
ADD AREG, BREG
COMP AREG, A
BC GT, LAST
LTORG
NEXT SUB AREG, ='1' MOVER CREG, B
ADD CREG, ='8'
MOVEM CREG, B
PRINT B
LAST STOP
A DS 1
B DS 1
END
)";

    // Generate pool table from assembly code
    vector<int> poolTable = generatePoolTable(assemblyCode);

    // Print pool table in the expected format
    cout << "-------------" << endl;
    cout << "Pool Address" << endl;
    cout << "-------------" << endl;
    for (size_t i = 0; i < poolTable.size(); ++i) {
        cout << "Pool " << i + 1 << ": " << poolTable[i] << endl;
    }

    return 0;
}
