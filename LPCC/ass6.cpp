#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace std;

unordered_map<string, vector<string>> macro_table;

// Function to expand macros in the code
vector<string> expand_macros(const vector<string>& code) {
    vector<string> expanded_code;

    for (const auto& line : code) {
        stringstream ss(line);
        string token;
        ss >> token;

        if (macro_table.count(token) > 0) {
            // Replace macro call with macro definition
            const vector<string>& macro_definition = macro_table[token];
            expanded_code.insert(expanded_code.end(), macro_definition.begin(), macro_definition.end());
        } else {
            // Regular instruction, add to expanded code
            expanded_code.push_back(line);
        }
    }

    return expanded_code;
}

// Function to generate Intermediate code from expanded code
vector<string> generate_intermediate_code(const vector<string>& code) {
    vector<string> intermediate_code;
    int line_num = 1;

    for (const auto& line : code) {
        intermediate_code.push_back(to_string(line_num) + ": " + line);
        line_num++;
    }

    return intermediate_code;
}

int main() {
    // Example Macro-Expanded Code
    vector<string> code = {
        "LOAD J",
        "STORE M",
        "MACRO EST",
        "LOAD P",
        "ADD V",
        "MEND",
        "LOAD S",
        "MACRO ADD7 P4, P5, P6",
        "LOAD P5",
        "LOAD e",
        "ADD d",
        "MEND",
        "LOAD S",
        "MACRO SUB4 ABC",
        "LOAD U",
        "STORE ABC",
        "MEND",
        "SUB4 XYZ",
        "SUB 8",
        "SUB 2",
        "STORE P4",
        "STORE P6",
        "MEND",
        "EST",
        "ADD7 C4, C5",
        "SUB4 z",
        "END"
    };

    // Populate macro table with macro definitions
    macro_table["EST"] = {"LOAD P", "ADD V"};
    macro_table["ADD7"] = {"LOAD P5", "LOAD e", "ADD d"};
    macro_table["SUB4"] = {"LOAD U", "STORE ABC"};

    // Expand macros in the code
    vector<string> expanded_code = expand_macros(code);

    // Generate Intermediate code from expanded code
    vector<string> intermediate_code = generate_intermediate_code(expanded_code);

    // Print Intermediate code
    cout << "Intermediate Code:" << endl;
    for (const auto& line : intermediate_code) {
        cout << line << endl;
    }

    return 0;
}
