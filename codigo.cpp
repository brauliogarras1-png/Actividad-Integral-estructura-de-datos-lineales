/**
 * Program: Act 1.3a - Comprehensive Activity of Basic Concepts and Fundamental Algorithms
 * Description: This program reads log records from a file, sorts them by date,
 *              searches for records within a user-specified date range, and
 *              stores the sorted records in an output file.
 * Student IDs: A01648054, A01649125, A01642979, A01641343
 * Date: September 10th, 2026
 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct LogRecord {
    string originalLine;
    long long dateKey;
};

/**
 * Converts a three-letter month abbreviation to its numeric value.
 * Parameter month: the three-letter month abbreviation.
 * Returns: the month number from 1 to 12, or 0 if the month is invalid.
 * Time complexity: O(1)
 * Space complexity: O(1)
 */
int monthToNumber(const string &month) {
    int monthNumber = 0;

    if (month == "Jan") {
        monthNumber = 1;
    } else if (month == "Feb") {
        monthNumber = 2;
    } else if (month == "Mar") {
        monthNumber = 3;
    } else if (month == "Apr") {
        monthNumber = 4;
    } else if (month == "May") {
        monthNumber = 5;
    } else if (month == "Jun") {
        monthNumber = 6;
    } else if (month == "Jul") {
        monthNumber = 7;
    } else if (month == "Aug") {
        monthNumber = 8;
    } else if (month == "Sep") {
        monthNumber = 9;
    } else if (month == "Oct") {
        monthNumber = 10;
    } else if (month == "Nov") {
        monthNumber = 11;
    } else if (month == "Dec") {
        monthNumber = 12;
    }

    return monthNumber;
}

/**
 * Converts a month, day, and time into a numeric key that can be compared.
 * Parameter month: the three-letter month abbreviation.
 * Parameter day: the day of the month.
 * Parameter time: the time in hh:mm:ss format.
 * Returns: a numeric key in MMDDhhmmss order, or -1 if the data is invalid.
 * Time complexity: O(1)
 * Space complexity: O(1)
 */
long long createDateKey(const string &month, int day, const string &time) {
    int monthNumber = monthToNumber(month);
    int hour = 0;
    int minute = 0;
    int second = 0;
    char firstColon = ':';
    char secondColon = ':';

    stringstream timeStream(time);
    timeStream >> hour >> firstColon >> minute >> secondColon >> second;

    if (monthNumber == 0 || day < 1 || day > 31 || !timeStream || firstColon != ':' ||
        secondColon != ':' || hour < 0 || hour > 23 || minute < 0 || minute > 59 ||
        second < 0 || second > 59) {
        return -1;
    }

    // Creates a comparable key in MMDDhhmmss order, using long long to avoid overflow for large values
    return static_cast<long long>(monthNumber) * 100000000LL
        + static_cast<long long>(day) * 1000000LL
        + static_cast<long long>(hour) * 10000LL
        + static_cast<long long>(minute) * 100LL
        + second;
}

/**
 * Reads all valid records from the input file and stores them in a vector.
 * Parameter fileName: the name of the input file.
 * Parameter records: the vector where the records will be stored.
 * Returns: true if the file was opened successfully, otherwise false.
 * Time complexity: O(n)
 * Space complexity: O(n)
 */
bool readFile(const string &fileName, vector<LogRecord> &records) {
    ifstream inputFile(fileName);
    string line = "";

    if (!inputFile.is_open()) {
        return false;
    }

    while (getline(inputFile, line)) {
        string month = "";
        string time = "";
        int day = 0;
        long long dateKey = -1;
        stringstream lineStream(line);

        lineStream >> month >> day >> time;
        dateKey = createDateKey(month, day, time);

        if (dateKey != -1) {
            LogRecord record = {line, dateKey};
            records.push_back(record);
        }
    }

    inputFile.close();
    return true;
}

/**
 * Merges two sorted sections of the vector into one sorted section.
 * Parameter records: the vector containing the log records.
 * Parameter left: the first index of the section to merge.
 * Parameter middle: the last index of the left half.
 * Parameter right: the last index of the section to merge.
 * Returns: nothing.
 * Time complexity: O(n)
 * Space complexity: O(n)
 */
void merge(vector<LogRecord> &records, int left, int middle, int right) {
    vector<LogRecord> temporary;
    int leftIndex = left;
    int rightIndex = middle + 1;

    while (leftIndex <= middle && rightIndex <= right) {
        if (records[leftIndex].dateKey <= records[rightIndex].dateKey) {
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

    for (int index = 0; index < static_cast<int>(temporary.size()); index++) {
        records[left + index] = temporary[index];
    }
}

/**
 * Sorts a section of the vector chronologically using merge sort.
 * Parameter records: the vector containing the log records.
 * Parameter left: the first index of the section to sort.
 * Parameter right: the last index of the section to sort.
 * Returns: nothing.
 * Time complexity: O(n log n)
 * Space complexity: O(n)
 */
void mergeSort(vector<LogRecord> &records, int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        mergeSort(records, left, middle);
        mergeSort(records, middle + 1, right);
        merge(records, left, middle, right);
    }
}

/**
 * Writes all sorted records to an output file.
 * Parameter fileName: the name of the output file.
 * Parameter records: the sorted vector of log records.
 * Returns: true if the file was created successfully, otherwise false.
 * Time complexity: O(n)
 * Space complexity: O(1)
 */
bool writeFile(const string &fileName, const vector<LogRecord> &records) {
    ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        return false;
    }

    for (int index = 0; index < static_cast<int>(records.size()); index++) {
        outputFile << records[index].originalLine << endl;
    }

    outputFile.close();
    return true;
}

/**
 * Uses binary search to find the first record whose date is greater than or equal to the target date.
 * Parameter records: the sorted vector of log records.
 * Parameter targetKey: the numeric date key to search for.
 * Returns: the index of the first matching position.
 * Time complexity: O(log n)
 * Space complexity: O(1)
 */
int binarySearchStart(const vector<LogRecord> &records, long long targetKey) {
    int left = 0;
    int right = static_cast<int>(records.size());

    while (left < right) {
        int middle = left + (right - left) / 2;

        if (records[middle].dateKey < targetKey) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return left;
}

/**
 * Uses binary search to find the first record whose date is greater than the target date.
 * Parameter records: the sorted vector of log records.
 * Parameter targetKey: the numeric date key to search for.
 * Returns: the index immediately after the last record in the range.
 * Time complexity: O(log n)
 * Space complexity: O(1)
 */
int binarySearchEnd(const vector<LogRecord> &records, long long targetKey) {
    int left = 0;
    int right = static_cast<int>(records.size());

    while (left < right) {
        int middle = left + (right - left) / 2;

        if (records[middle].dateKey <= targetKey) {
            left = middle + 1;
        } else {
            right = middle;
        }
    }

    return left;
}

/**
 * Requests a valid date and time from the user and converts it to a numeric key.
 * Parameter message: the prompt shown to the user.
 * Returns: the numeric key corresponding to the entered date and time.
 * Time complexity: O(1)
 * Space complexity: O(1)
 */
long long requestDate(const string &message) {
    string month = "";
    string time = "";
    int day = 0;
    long long dateKey = -1;

    while (dateKey == -1) {
        cout << message << " (Mon DD hh:mm:ss): ";
        cin >> month >> day >> time;
        dateKey = createDateKey(month, day, time);

        if (dateKey == -1) {
            cout << "Invalid date. Please try again." << endl;
        }
    }

    return dateKey;
}

/**
 * Displays all records located between two vector positions.
 * Parameter records: the sorted vector of log records.
 * Parameter firstIndex: the first index to display.
 * Parameter lastIndex: the index immediately after the final record to display.
 * Returns: nothing.
 * Time complexity: O(k), where k is the number of displayed records.
 * Space complexity: O(1)
 */
void displayRecords(const vector<LogRecord> &records, int firstIndex, int lastIndex) {
    if (firstIndex >= lastIndex) {
        cout << "No records were found in the requested range." << endl;
        return;
    }

    cout << endl;
    cout << "--- Records found ---" << endl;

    for (int index = firstIndex; index < lastIndex; index++) {
        cout << records[index].originalLine << endl;
    }

    cout << endl;
    cout << "Total records found: " << lastIndex - firstIndex << endl;
}

/**
 * Controls the execution of the program.
 * Returns: 0 when the program finishes successfully, otherwise 1.
 * Time complexity: O(n log n)
 * Space complexity: O(n)
 */
int main() {
    const string INPUT_FILE = "bitacora.txt";
    const string OUTPUT_FILE = "bitacoraOrdenada.txt";
    vector<LogRecord> records;
    long long startDate = -1;
    long long endDate = -1;
    int firstIndex = 0;
    int lastIndex = 0;

    if (!readFile(INPUT_FILE, records)) {
        cout << "Error: Could not open " << INPUT_FILE << "." << endl;
        return 1;
    }

    if (!records.empty()) {
        mergeSort(records, 0, static_cast<int>(records.size()) - 1);
    }

    if (!writeFile(OUTPUT_FILE, records)) {
        cout << "Error: Could not create " << OUTPUT_FILE << "." << endl;
        return 1;
    }

    cout << "The file was read and sorted successfully." << endl;
    cout << "The sorted records were saved in " << OUTPUT_FILE << "." << endl;
    cout << endl;

    startDate = requestDate("Enter the start date");
    endDate = requestDate("Enter the end date");

    if (startDate > endDate) {
        long long temporaryDate = startDate;
        startDate = endDate;
        endDate = temporaryDate;
    }

    firstIndex = binarySearchStart(records, startDate);
    lastIndex = binarySearchEnd(records, endDate);
    displayRecords(records, firstIndex, lastIndex);

    return 0;
}
