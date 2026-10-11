
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

//Data that we will need in storage 

struct LogRecord {
    string originalLine;
    unsigned long long ipKey;
};

/**
 * Converts an IP address into a numeric key.
 * The port is not considered.
 * Time complexity: O(1)
 * Space complexity: O(1)
 */
long long createIPKey(const string &ipAddress) {
    string ip = ipAddress;
    //The port after the IP
    int portPosition = ip.find(':');

    if (portPosition != string::npos) {
        ip = ip.substr(0, portPosition);
    }

    //Evaluating the format of the IP
    stringstream ipStream(ip);
    int a, b, c, d;
    char dot1, dot2, dot3;

    if (!(ipStream >> a >> dot1 >> b >> dot2
                   >> c >> dot3 >> d)) {
        return -1;
    }

    //Every number on an IP can not be bigger than 255 
    if (dot1 != '.' || dot2 != '.' || dot3 != '.' ||
        a < 0 || a > 255 ||
        b < 0 || b > 255 ||
        c < 0 || c > 255 ||
        d < 0 || d > 255) {
        return -1;
    }

    char extra;
    if (ipStream >> extra) {
        return -1;
    }
    //We use the potencies of 256 to assing and multiply the value after a point
    return static_cast<long long>(a) * 16777216LL
         + static_cast<long long>(b) * 65536LL
         + static_cast<long long>(c) * 256LL
         + d;
}

/**
 * Reads valid records and stores them in a vector. So we can create bitacoraRangoIP.txt
 * Time complexity: O(n)
 * Space complexity: O(n)
 */
bool readFile(const string &fileName,
              vector<LogRecord> &records) {

    ifstream inputFile(fileName);
    string line;

    if (!inputFile.is_open()) {
        return false;
    }

    while (getline(inputFile, line)) {
        string month, time, ipAddress;
        int day;
        stringstream lineStream(line);

        if (lineStream >> month >> day >> time >> ipAddress) {
            long long ipKey = createIPKey(ipAddress);

            if (ipKey != -1) {
                LogRecord record = {
                    line,
                    static_cast<unsigned long long>(ipKey)
                };
                records.push_back(record);
            }
        }
    }

    inputFile.close();
    return true;
}

/**
 * Merges two sorted sections of the vector.
 * Time complexity: O(n)
 * Space complexity: O(n)
 */
void merge(vector<LogRecord> &records,
           int left, int middle, int right) {

    vector<LogRecord> temporary;
    int leftIndex = left;
    int rightIndex = middle + 1;

    while (leftIndex <= middle && rightIndex <= right) {

        if (records[leftIndex].ipKey <=
            records[rightIndex].ipKey) {

            temporary.push_back(records[leftIndex]);
            leftIndex++;
        } else {
            temporary.push_back(records[rightIndex]);
            rightIndex++;
        }
    }

    while (leftIndex <= middle) {
        temporary.push_back(records[leftIndex]);
        leftIndex++;
    }

    while (rightIndex <= right) {
        temporary.push_back(records[rightIndex]);
        rightIndex++;
    }

    for (int i = 0;
         i < static_cast<int>(temporary.size()); i++) {
        records[left + i] = temporary[i];
    }
}

/**
 * Sorts records numerically by IP using Merge Sort.
 * Time complexity: O(n log n)
 * Space complexity: O(n)
 */
void mergeSort(vector<LogRecord> &records,
               int left, int right) {

    if (left < right) {
        int middle = left + (right - left) / 2;

        mergeSort(records, left, middle);
        mergeSort(records, middle + 1, right);
        merge(records, left, middle, right);
    }
}

/**
 * Finds the first IP greater than or equal to target.
 * Time complexity: O(log n)
 * Space complexity: O(1)
 */
// We used the same method for IPs too, MergeSort to sort all the data 
// and binary search for the begining and end of the request, to avoid redundancy.
int binarySearchStart(const vector<LogRecord> &records,
                      unsigned long long targetKey) {

    int left = 0;
    int right = static_cast<int>(records.size());

    while (left < right) {
        int middle = left + (right - left) / 2;

        if (records[middle].ipKey < targetKey) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return left;
}

/**
 * Finds the first IP greater than the target.
 * Time complexity: O(log n)
 * Space complexity: O(1)
 */
int binarySearchEnd(const vector<LogRecord> &records,
                    unsigned long long targetKey) {

    int left = 0;
    int right = static_cast<int>(records.size());

    while (left < right) {
        int middle = left + (right - left) / 2;

        if (records[middle].ipKey <= targetKey) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return left;
}

/**
 * Requests a valid IP address from the user.
 * Time complexity: O(1)
 * Space complexity: O(1)
 */
unsigned long long requestIP(const string &message) {
    string ip;
    long long ipKey = -1;

    while (ipKey == -1) {
        cout << message << ": ";
        cin >> ip;

        ipKey = createIPKey(ip);

        if (ipKey == -1) {
            cout << "Invalid IP. Try again." << endl;
        }
    }

    return static_cast<unsigned long long>(ipKey);
}

/**
 * Saves all records in the vector to a file.
 * Time complexity: O(n)
 * Space complexity: O(1)
 */
bool writeFile(const string &fileName,
               const vector<LogRecord> &records) {

    ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        return false;
    }

    for (int i = 0;
         i < static_cast<int>(records.size()); i++) {
        outputFile << records[i].originalLine << endl;
    }

    outputFile.close();
    return true;
}

/**
 * Displays and saves records within the IP range.
 * Time complexity: O(k)
 * Space complexity: O(1)
 */
bool displayAndSaveRecords(
    const vector<LogRecord> &records,
    int firstIndex, int lastIndex,
    const string &fileName) {

    ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        return false;
    }

    if (firstIndex >= lastIndex) {
        cout << "No records found." << endl;
    } else {
        cout << endl << "--- Records found ---" << endl;

        for (int i = firstIndex; i < lastIndex; i++) {
            cout << records[i].originalLine << endl;
            outputFile << records[i].originalLine << endl;
        }
    }

    cout << endl;
    cout << "Total records found: "
         << lastIndex - firstIndex << endl;

    outputFile.close();
    return true;
}

/**
 * Main program.
 * Time complexity: O(n log n + k)
 * Space complexity: O(n)
 */
int main() {
    //Thanks to the Phase 1 of this project, we can have a sorted vector for dates and modifify it.
    const string INPUT_FILE = "bitacora.txt";
    const string OUTPUT_FILE = "bitacoraOrdenadaIP.txt";
    const string RANGE_FILE = "bitacoraRangoIP.txt";

    vector<LogRecord> records;

    if (!readFile(INPUT_FILE, records)) {
        cout << "Error opening file." << endl;
        return 1;
    }

    if (!records.empty()) {
        mergeSort(records, 0,
                  static_cast<int>(records.size()) - 1);
    }

    if (!writeFile(OUTPUT_FILE, records)) {
        cout << "Error creating sorted file." << endl;
        return 1;
    }

    cout << "File sorted by IP successfully." << endl;
    cout << "Total records: " << records.size() << endl;
    cout << "Saved in: " << OUTPUT_FILE << endl;

    unsigned long long startIP =
        requestIP("Enter start IP");

    unsigned long long endIP =
        requestIP("Enter end IP");

    if (startIP > endIP) {
        unsigned long long temp = startIP;
        startIP = endIP;
        endIP = temp;
    }

    int firstIndex = binarySearchStart(records, startIP);
    int lastIndex = binarySearchEnd(records, endIP);

    if (!displayAndSaveRecords(records, firstIndex,
                               lastIndex, RANGE_FILE)) {
        cout << "Error saving range file." << endl;
        return 1;
    }

    cout << "Range saved in: " << RANGE_FILE << endl;

    return 0;
}
