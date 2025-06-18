#pragma once // Include guard to prevent multiple inclusions
#include <vector> // For dynamic arrays
#include <string> // For string manipulation
#include <set> // For storing unique elements
#include <fstream> // For file input/output operations
#include <sstream> // For string stream operations
#include <algorithm> // For standard algorithms like sorting
#include "field.h" // Include the field header
#include "entry.h" // Include the entry header
using namespace std; // Use the standard namespace

// Class definition for the database organizer
class databaseorganizer {
public:
    vector<fieldformat> entry_format; // Vector to store the format of each field in an entry
    vector<entry> entries; // Vector to store the entries
    set<string> unique_first_fields; // Set to store unique first fields for duplicate detection
    vector<pair<string, string>> catalog_exceptions; // Vector to store exceptions during catalog parsing
    vector<string> catalog_lines; // Vector to store catalog lines
    vector<string> command_log; // Vector to store the command log

    // Method to parse data from a file
    void parsedata(const string& datafile);
    // Method to parse commands from a file
    void parsecommands(const string& cmdfile);
    // Method to write the log to a file
    void write_log(const string& outfile);

private:
    // Method to parse the format of an entry
    void parseformat(const string& line);
    // Method to parse an entry
    bool parseentry(const string& line, entry& e);
    // Method to format the names of the fields
    string format_names();
    // Method to find the index of a field
    int field_index(const string& name);
    // Method to handle the search command
    void handle_search(const string& line, vector<string>& output);
    // Method to handle the sort command
    void handle_sort(const string& line, vector<string>& output);
};

// --- Implementation ---

// Implementation of the parsedata method
inline void databaseorganizer::parsedata(const string& datafile) {
    ifstream fin(datafile); // Open the data file for reading
    string line; // String to store each line

    getline(fin, line); // Read the first line (format)
    parseformat(line); // Parse the format

    catalog_lines.clear(); // Clear previous catalog lines
    catalog_exceptions.clear(); // Clear previous catalog exceptions
    catalog_lines.push_back(format_names()); // Add the formatted field names to catalog lines

    while (getline(fin, line)) { // Read each line from the file
        if (line.empty()) continue; // Skip empty lines

        entry e; // Create an entry object
        if (!parseentry(line, e)) { // If parsing the entry fails
            catalog_exceptions.push_back({ "Exception: missing field", line }); // Add an exception
            continue; // Skip to the next line
        }

        string first = e.first_field_str(); // Get the first field as a string
        if (unique_first_fields.count(first)) { // If the first field is not unique
            catalog_exceptions.push_back({ "Exception: duplicate entry", e.tostring(entry_format) }); // Add a duplicate entry exception
            continue; // Skip to the next line
        }

        unique_first_fields.insert(first); // Insert the first field into the unique set
        entries.push_back(e); // Add the entry to the vector of entries
    }

    ostringstream oss; // Create a string stream
    oss << entries.size() << " unique entries"; // Add the number of unique entries to the stream
    catalog_lines.push_back(oss.str()); // Add the string to the catalog lines
}

// Implementation of the parsecommands method
inline void databaseorganizer::parsecommands(const string& cmdfile) {
    ifstream fin(cmdfile); // Open the command file for reading
    string line; // String to store each line

    while (getline(fin, line)) { // Read each line from the file
        if (line.empty()) continue; // Skip empty lines

        vector<string> cmd_output; // Vector to store the output of the command
        cmd_output.push_back(line); // Add the command to the output

        if (line.find("search") == 0) { // If the command is "search"
            handle_search(line, cmd_output); // Handle the search command
        } else if (line.find("sort") == 0) { // If the command is "sort"
            handle_sort(line, cmd_output); // Handle the sort command
        } else { // If the command is not recognized
            cmd_output.push_back("Exception: command is wrong"); // Add an exception
        }

        command_log.insert(command_log.end(), cmd_output.begin(), cmd_output.end()); // Add the command output to the command log
    }
}

// Implementation of the write_log method
inline void databaseorganizer::write_log(const string& outfile) {
    ofstream fout(outfile); // Open the output file for writing

    fout << catalog_lines[0] << endl; // Write the formatted field names
    for (const auto& ex : catalog_exceptions) { // Write each exception
        fout << ex.first << endl; // Write the exception message
        fout << ex.second << endl; // Write the line where the exception occurred
    }
    fout << catalog_lines[1] << endl; // Write the number of unique entries
    for (const auto& l : command_log) fout << l << endl; // Write each command log entry
}

// Implementation of the parseformat method
inline void databaseorganizer::parseformat(const string& line) {
    entry_format.clear(); // Clear previous entry formats
    stringstream ss(line); // Create a string stream from the line
    string token; // String to store each token

    while (getline(ss, token, '|')) { // Split the line by '|'
        stringstream fs(token); // Create a string stream from the token
        string name, type, sm; // Strings to store the name, type, and single/multi
        getline(fs, name, ':'); // Split the token by ':' to get the name
        getline(fs, type, ':'); // Split the token by ':' to get the type
        getline(fs, sm, ':'); // Split the token by ':' to get the single/multi

        dataT dt; // Data type
        if (type == "string") dt = dataT::S; // If the type is "string"
        else if (type == "integer") dt = dataT::I; // If the type is "integer"
        else if (type == "double") dt = dataT::D; // If the type is "double"
        else dt = dataT::B; // Otherwise, it's boolean

        fieldT ft = (sm == "single") ? fieldT::single : fieldT::multi; // Field type (single or multi)
        entry_format.push_back({ name, dt, ft }); // Add the field format to the vector
    }
}

// Implementation of the parseentry method
inline bool databaseorganizer::parseentry(const string& line, entry& e) {
    vector<string> tokens; // Vector to store the tokens
    stringstream ss(line); // Create a string stream from the line
    string token; // String to store each token

    while (getline(ss, token, '|')) tokens.push_back(token); // Split the line by '|' and store the tokens
    if (tokens.size() != entry_format.size()) return false; // If the number of tokens doesn't match the format, return false

    for (size_t i = 0; i < entry_format.size(); ++i) { // Iterate over each token
        // Check if the token is empty - this will be considered a missing field
        if (tokens[i].empty()) {
            return false; // Return false to indicate missing field
        }

        const auto& fmt = entry_format[i]; // Get the field format
        
        if (fmt.sm == fieldT::single) { // If the field is single
            if (fmt.type == dataT::S) // If the type is string
                e.fields.push_back(make_shared<FieldSingle<string>>(tokens[i])); // Create a FieldSingle<string> and add it to the entry
            else if (fmt.type == dataT::I) // If the type is integer
                e.fields.push_back(make_shared<FieldSingle<int>>(stoi(tokens[i]))); // Create a FieldSingle<int> and add it to the entry
            else if (fmt.type == dataT::D) // If the type is double
                e.fields.push_back(make_shared<FieldSingle<double>>(stod(tokens[i]))); // Create a FieldSingle<double> and add it to the entry
            else if (fmt.type == dataT::B) // If the type is boolean
                e.fields.push_back(make_shared<FieldSingle<bool>>(tokens[i] == "true")); // Create a FieldSingle<bool> and add it to the entry
        } else { // If the field is multi
            vector<string> vals; // Vector to store the values
            stringstream ms(tokens[i]); // Create a string stream from the token
            string val; // String to store each value

            while (getline(ms, val, ':')) { // Split the token by ':'
                if (fmt.type == dataT::S) vals.push_back(val); // If the type is string, add the value to the vector
                else if (fmt.type == dataT::I) vals.push_back(to_string(stoi(val))); // If the type is integer, convert to string and add
                else if (fmt.type == dataT::D) vals.push_back(to_string(stod(val))); // If the type is double, convert to string and add
                else if (fmt.type == dataT::B) vals.push_back(val == "true" ? "true" : "false"); // If the type is boolean, convert to string and add
            }

            if (fmt.type == dataT::S) // If the type is string
                e.fields.push_back(make_shared<FieldMulti<string>>(vals)); // Create a FieldMulti<string> and add it
            else if (fmt.type == dataT::I) { // If the type is integer
                vector<int> ivals; // Vector to store integer values
                for (auto& v : vals) ivals.push_back(stoi(v)); // Convert each value to integer
                e.fields.push_back(make_shared<FieldMulti<int>>(ivals)); // Create a FieldMulti<int> and add it
            } else if (fmt.type == dataT::D) { // If the type is double
                vector<double> dvals; // Vector to store double values
                for (auto& v : vals) dvals.push_back(stod(v)); // Convert each value to double
                e.fields.push_back(make_shared<FieldMulti<double>>(dvals)); // Create a FieldMulti<double> and add it
            } else if (fmt.type == dataT::B) { // If the type is boolean
                vector<bool> bvals; // Vector to store boolean values
                for (auto& v : vals) bvals.push_back(v == "true"); // Convert each value to boolean
                e.fields.push_back(make_shared<FieldMulti<bool>>(bvals)); // Create a FieldMulti<bool> and add it
            }
        }
    }
    return true; // Return true if parsing was successful
}

// Implementation of the format_names method
inline string databaseorganizer::format_names() {
    string s; // String to store the formatted names
    for (size_t i = 0; i < entry_format.size(); ++i) { // Iterate over each field format
        if (i) s += "|"; // Add a '|' separator if it's not the first field
        s += entry_format[i].name; // Add the field name
    }
    return s; // Return the formatted names
}

// Implementation of the field_index method
inline int databaseorganizer::field_index(const string& name) {
    for (size_t i = 0; i < entry_format.size(); ++i) // Iterate over each field format
        if (entry_format[i].name == name) return i; // If the name matches, return the index
    return -1; // If no match is found, return -1
}

// Implementation of the handle_search method
inline void databaseorganizer::handle_search(const string& line, vector<string>& output) {
    size_t q1 = line.find('"'), q2 = line.find('"', q1 + 1); // Find the positions of the quotes
    if (q1 == string::npos || q2 == string::npos) { // If the quotes are not found
        output.push_back("Exception: command is wrong"); // Add an exception
        return; 
    }

    string value = line.substr(q1 + 1, q2 - q1 - 1); // Extract the value between the quotes
    size_t inpos = line.find("in", q2); // Find the position of "in"
    if (inpos == string::npos) { // If "in" is not found
        output.push_back("Exception: command is wrong"); // Add an exception
        return; 
    }
    string field = line.substr(inpos + 3); // Extract the field name
    field.erase(remove(field.begin(), field.end(), '"'), field.end()); // Remove the quotes from the field name
    
    int idx = field_index(field); // Get the index of the field
    if (idx == -1) { // If the field is not found
        output.push_back("Exception: command is wrong"); // Add an exception
        return; 
    }
    
    for (const auto& e : entries) { // Iterate over each entry
        auto& f = e.fields[idx]; // Get the field
        
        // Special case for boolean fields
        if (entry_format[idx].type == dataT::B) { // If the field is boolean
            string field_val = f->tostring(); // Get the field value as string
            bool is_match = false; // Flag to indicate if there's a match
            
            // For boolean fields, compare with both string formats
            if ((value == "true" && (field_val == "true" || field_val == "1")) ||
                (value == "false" && (field_val == "false" || field_val == "0"))) {
                is_match = true; // There's a match
            }
            
            if (is_match) {
                output.push_back(e.tostring(entry_format)); // Add the entry to the output
            }
        }
        // For non-boolean fields, use the default string comparison
        else if (f->tostring().find(value) != string::npos) { // If the value is found in the field
            output.push_back(e.tostring(entry_format)); // Add the entry to the output
        }
    }
}

// Implementation of the handle_sort method
inline void databaseorganizer::handle_sort(const string& line, vector<string>& output) {
    size_t q1 = line.find('"'), q2 = line.find('"', q1 + 1); // Find the positions of the quotes
    if (q1 == string::npos || q2 == string::npos) { // If the quotes are not found
        output.push_back("Exception: command is wrong"); // Add an exception
        return; 
    }

    string field = line.substr(q1 + 1, q2 - q1 - 1); // Extract the field name
    int idx = field_index(field); // Get the index of the field
    if (idx == -1) { // If the field is not found
        output.push_back("Exception: command is wrong"); // Add an exception
        return; 
    }

    auto cmp = [this, idx](const entry& e1, const entry& e2) { // Define a lambda for comparison
        auto& ff = entry_format[idx]; // Get the field format
        if (ff.sm == fieldT::single) { // If the field is single
            return e1.fields[idx]->tostring() < e2.fields[idx]->tostring(); // Compare the fields
        } else { // If the field is multi
            return e1.fields[idx]->tostring() < e2.fields[idx]->tostring(); // Compare the fields
        }
    };
    
    vector<entry> sorted = entries; // Create a copy of the entries
    sort(sorted.begin(), sorted.end(), cmp); // Sort the entries
    for (const auto& e : sorted) // Iterate over the sorted entries
        output.push_back(e.tostring(entry_format)); // Add the entry to the output
}