#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace std;

// Function to generate MNT and MDT
pair<unordered_map<string, pair<int, vector<string>>>, unordered_map<int, vector<string>>> generate_mnt_mdt(const vector<string>& code) {
    unordered_map<string, pair<int, vector<string>>> mnt;  // Macro Name Table (MNT)
    unordered_map<int, vector<string>> mdt;  // Macro Definition Table (MDT)
    int mdt_index = 1;

    for (int i = 0; i < code.size(); ++i) {
        string line = code[i];
        stringstream ss(line);
        string token;
        ss >> token;

        if (token == "MACRO") {
            // Macro definition found
            ss >> token; // Read macro name
            string macro_name = token;
            vector<string> parameters;

            // Read macro parameters
            while (ss >> token) {
                if (token == "MEND") {
                    break;
                }
                parameters.push_back(token);
            }

            // Collect macro definition
            vector<string> macro_definition;
            ++i; // Move to next line after "MACRO"
            while (i < code.size() && code[i] != "MEND") {
                macro_definition.push_back(code[i]);
                ++i;
            }

            // Record macro details in MNT
            mnt[macro_name] = {mdt_index, parameters};

            // Record macro definition in MDT
            mdt[mdt_index] = macro_definition;
            ++mdt_index;
        }
    }

    return make_pair(mnt, mdt);
}

int main() {
    // Example Macro-Expanded Code
    vector<string> code = {
        "LOAD A",
        "STORE B",
        "MACRO ABC",
        "LOAD p",
        "SUB q",
        "MEND",
        "MACRO ADD1 ARG",
        "LOAD X",
        "STORE ARG",
        "MEND",
        "MACRO ADD5 A1, A2, A3",
        "STORE A2",
        "ADD1 5",
        "ADD1 10",
        "LOAD A1",
        "LOAD A3",
        "MEND",
        "ABC",
        "ADD5 D1, D2, D3",
        "END"
    };

    // Generate MNT and MDT
    auto result = generate_mnt_mdt(code);
    unordered_map<string, pair<int, vector<string>>> mnt = result.first;
    unordered_map<int, vector<string>> mdt = result.second;

    // Print Macro Name Table (MNT)
    cout << "Macro Name Table (MNT):" << endl;
    cout << "Macro Name\tMDT Index\tParameters" << endl;
    cout << "----------------------------------------------" << endl;
    for (const auto& entry : mnt) {
        const string& macro_name = entry.first;
        const auto& details = entry.second;
        cout << macro_name << "\t\t" << details.first << "\t\t";
        if (details.second.empty()) {
            cout << "None";
        } else {
            for (const auto& param : details.second) {
                cout << param << " ";
            }
        }
        cout << endl;
    }

    // Print Macro Definition Table (MDT)
    cout << endl << "Macro Definition Table (MDT):" << endl;
    cout << "MDT Index\tMacro Definition" << endl;
    cout << "----------------------------------------------" << endl;
    for (const auto& entry : mdt) {
        int index = entry.first;
        const vector<string>& definition = entry.second;
        cout << index << "\t\t";
        for (const auto& line : definition) {
            cout << line << endl << "\t\t";
        }
        cout << endl;
    }

    return 0;
}
