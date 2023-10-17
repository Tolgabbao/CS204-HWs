#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


/**
 * Searches for a given word in a matrix in a spiral clockwise manner, starting from the given position.
 * @param matrix The matrix to search in.
 * @param word The word to search for.
 * @param i The row index of the starting position.
 * @param j The column index of the starting position.
 * @return True if the word is found, false otherwise.
 */
bool searchWordInSpiralClockwise(vector<vector<char>>& matrix, string word, int i, int j) {
    int colSize = matrix[0].size();
    int rowSize = matrix.size();
    int wordSize = word.size();
    int rowDirection = 0;
    int colDirection = 1;
    if (i < 0 || i >= rowSize || j < 0 || j >= colSize) {
        return false;
    }
    if (matrix[i][j] != word[0]) {
        return false;
        
    }
    for(int k=0; k<4; k++){
        int rowIndex = i;
        int colIndex = j;
        int steps = 1;
        int directionChanges = 0;
        if (k == 0){
            rowDirection = 0;
            colDirection = 1;
        }
        else if (k == 1){
            rowDirection = 1;
            colDirection = 0;
        }
        else if (k == 2){
            rowDirection = 0;
            colDirection = -1;
        }
        else{
            rowDirection = -1;
            colDirection = 0;
        }
        int wordIndex = 0;
        while (wordIndex < wordSize){
            if (rowDirection == 0 && colDirection == 1){
                rowDirection = 1; colDirection = 0;
            }
            else if (rowDirection == 1 && colDirection == 0){
                rowDirection = 0; colDirection = -1;
            }
            else if (rowDirection == 0 && colDirection == -1){
                rowDirection = -1; colDirection = 0;
            }
            else if (rowDirection == -1 && colDirection == 0){
                rowDirection = 0; colDirection = 1;
            }
            for (int step = 0; step < steps; step++){
                if (wordIndex == wordSize) return true;
                rowIndex += rowDirection;
                colIndex += colDirection;
                wordIndex++;
                if (rowIndex >= 0 && rowIndex < rowSize && colIndex >= 0 && colIndex < colSize && matrix[rowIndex][colIndex] == word[wordIndex]){
                }
                else{
                    break;
                }
            }  
            directionChanges++;
            if (directionChanges == 2) {
                directionChanges = 0;
                steps++;
            }
            if (wordIndex == wordSize) return true;
            if (rowIndex >= 0 && rowIndex < rowSize && colIndex >= 0 && colIndex < colSize && matrix[rowIndex][colIndex] == word[wordIndex]){}
            else{
                break;}
        }
            
    }
    return false;
}

/**
 * Searches for a given word in a matrix in a spiral counterclockwise manner, starting from the given position.
 * @param matrix The matrix to search in.
 * @param word The word to search for.
 * @param i The row index of the starting position.
 * @param j The column index of the starting position.
 * @return True if the word is found, false otherwise.
 */
bool searchWordInSpiralCounterclockwise(vector<vector<char>>& matrix, string word, int i, int j) {
    int colSize = matrix[0].size();
    int rowSize = matrix.size();
    int wordSize = word.size();
    int rowDirection = 0;
    int colDirection = 1;
    if (i < 0 || i >= rowSize || j < 0 || j >= colSize) {
        return false;
    }
    if (matrix[i][j] != word[0]) {
        return false;
    }
    for(int k=0; k<4; k++){
        int rowIndex = i;
        int colIndex = j;
        int steps = 1;
        int directionChanges = 0;
        if (k == 0){
            rowDirection = 1;
            colDirection = 0;
        }
        else if (k == 1){
            rowDirection = 0;
            colDirection = -1;
        }
        else if (k == 2){
            rowDirection = -1;
            colDirection = 0;
        }
        else{
            rowDirection = 0;
            colDirection = 1;
        }
        int wordIndex = 0;
        while (wordIndex < wordSize){
            if (rowDirection == 1 && colDirection == 0){
                rowDirection = 0;
                colDirection = 1;
            }
            else if (rowDirection == 0 && colDirection == -1){
                rowDirection = 1;
                colDirection = 0;
            }
            else if (rowDirection == -1 && colDirection == 0){
                rowDirection = 0;
                colDirection = -1;
            }
            else if (rowDirection == 0 && colDirection == 1){
                rowDirection = -1;
                colDirection = 0;
            }
            for (int step = 0; step < steps; step++){
                if (wordIndex == wordSize) return true;
                rowIndex += rowDirection;
                colIndex += colDirection;
                wordIndex++;
                if (rowIndex >= 0 && rowIndex < rowSize && colIndex >= 0 && colIndex < colSize && matrix[rowIndex][colIndex] == word[wordIndex]){
                }
                else{
                    break;
                }
            }  
            directionChanges++;
            if (directionChanges == 2) {
                directionChanges = 0;
                steps++;
            }
            if (wordIndex == wordSize) return true;
            if (rowIndex >= 0 && rowIndex < rowSize && colIndex >= 0 && colIndex < colSize && matrix[rowIndex][colIndex] == word[wordIndex]){}
            else{
                break;
            }
        }
    }
    return false;
}


// Function to search for a word in a matrix in a spiral manner clockwise or counterclockwise starting from anywhere in the matrix
bool searchWordInMatrix(vector<vector<char>>& matrix, string word) {
    // Check if the matrix is empty
    if (matrix.empty()) {
        return false;
    }

    // Check if the word is empty
    if (word.empty()) {
        return false;
    }

    // Check if the matrix size is valid
    int n = matrix.size();
    if (n <= 0) {
        return false;
    }

    // Check if the matrix is square
    for (const auto& row : matrix) {
        if (row.size() != n) {
            return false;
        }
    }

    // Check if the word is longer than the matrix size
    if (word.length() > n * n) {
        return false;
    }

    // Check if the word contains only uppercase letters
    for (char c : word) {
        if (!isupper(c)) {
            return false;
        }
    }

    // Search for the word in the matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (searchWordInSpiralClockwise(matrix, word, i, j) || searchWordInSpiralCounterclockwise(matrix, word, i, j)) {
                return true;
            }
        }
    }
    return false;
}


// Print out the matrix for debugging purposes
void printMatrix(vector<vector<char>>& matrix) {
    for (const auto& row : matrix) {
        for (char c : row) {
            cout << c << " ";
        }
        cout << endl;
    }
}


// Function to read in a matrix from a file and search for words in it
void searchWordsInMatrixFromFile(string filename) {
    ifstream file(filename);
    while (!file.is_open()) {
        cout << "Could not open file " << filename << endl;
        cout << "Enter the name of the file" << endl;
        cin >> filename;
        file.open(filename);
    }

    // Read in the matrix size
    int n;
    file >> n;

    // Check if the matrix size is valid
    if (n <= 0) {
        // cout << "Invalid matrix size" << endl;
        cout << "Error: Input file is not in correct format!" << endl;
        return;
    }

    // Read in the matrix
    vector<vector<char>> matrix(n, vector<char>(n));
    string line;
    getline(file, line); // Consume the newline character after the matrix size
    for (int i = 0; i < n; i++) {
        getline(file, line);
        // Check if the line has the correct number of characters
        if (line.length() != n) {
            // cout << "Invalid matrix: line " << i + 1 << " does not have " << n << " characters" << endl;
            cout << "Error: Input file is not in correct format!" << endl;
            return;
        }
        // Check if the line contains only uppercase letters
        for (char c : line) {
            if (!isupper(c)) {
                // cout << "Invalid matrix: line " << i + 1 << " contains non-uppercase characters" << endl;
                cout << "Error: Input file is not in correct format!" << endl;
                return;
            }
        }
        // Store the characters in the matrix
        for (int j = 0; j < n; j++) {
            matrix[i][j] = line[j];
        }
    }

    // Read in the number of words to search for
    int numWords;
    file >> numWords;

    // Check if the number of words is valid
    if (numWords <= 0) {
        // cout << "Invalid number of words" << endl;
        cout << "Error: Input file is not in correct format!" << endl;
        return;
    }

    // Read in the words to search for
    vector<string> duplicateWords(numWords);
    for (int i = 0; i < numWords; i++) {
        file >> duplicateWords[i];
    }

    //Store the words to search for, removing duplicates because of matrix_30_8.solution
    vector<string> words;
    for (const string& word : duplicateWords) {
        bool isDuplicate = false;
        for (const string& existingWord : words) {
            if (existingWord == word) {
                isDuplicate = true;
                break;
            }
        }
        if (!isDuplicate) {
            words.push_back(word);
        }
    }

    // Search for the words in the matrix
    vector<string> foundWords;
    for (auto word : words) {
        if (searchWordInMatrix(matrix, word)) {
            foundWords.push_back(word);
        }
    }

    // Print out the results
    cout << foundWords.size() << " Words are Found: " << " ";
    // Print out the all the words for debugging
    /* for (const auto& word : words) {
        cout << word << " ";
    } */
    // Print out the matrix size and the matrix for debugging
    /* cout << "matrix size: " << n << "x" << n << " ";
    cout << "matrix:" << matrix.size() << "x" << matrix[0].size() << " " << endl;
    printMatrix(matrix);
    cout << "filename: " << filename << " "; */

    for (const auto& word : foundWords) {
        cout << word << " ";
    }
    cout << endl;

    file.close();
}

int main() {
    string filename;
    cout << "Enter the name of the file" << endl;
    cin >> filename;
    searchWordsInMatrixFromFile(filename);
    return 0;
}