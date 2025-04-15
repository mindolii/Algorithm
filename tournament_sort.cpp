#include <iostream>
#include <fstream>
#include <ctime>
#include <climits>
#include <cstdlib>

using namespace std;

int winner(float* tmp, int n, int p1, int p2) {
    // if p1 or p2 is leaf node, return p1 or p2
    // else, return tmp[p1] or tmp[p2]
    int u = p1 >= n ? p1 : tmp[p1];
    int v = p2 >= n ? p2 : tmp[p2];
    return ((int)tmp[u] < (int)tmp[v] ? u : v);  // returns the position of winner(smaller elem.)
}

// create tournament tree from input array, return first minimum value
void create_tree(float* arr, int size, float* tmp, float& val) {
    // fill the values in the leaf node
    for (int i = 0; i<size; i++) 
        tmp[size+i] = arr[i];
    // select parent node
    for (int i=2*size-1; i>1; i-=2) {
        int k = i/2;
        int j = i-1;
        tmp[k] = winner(tmp, size, i, j);
    }
    val = tmp[int(tmp[1])];
    tmp[int(tmp[1])] = INT_MAX;
}

// re... construct? the tree and calculate the nexe minimum value
void recreate(int size, float* tmp, float& val) {
    int i = tmp[1];
    // i: right node ind, j: left sibling ind, k: parent of i, j
    while (i>1) {
        int j, k = i/2;
        if (i%2 == 0 && i<2*size-1)
            j = i + 1;
        else
            j = i - 1;
        
        // store small value at tmp[k] (parent node)
        tmp[k] = winner(tmp, size, i, j);
        i = k;
    }
    val = tmp[int(tmp[1])];  // since tmp[1] is storing index of ... smallest elem.
    tmp[int(tmp[1])] = INT_MAX;
}

void tournamentSort(float* arr, int size, float* output) {
    float* tmp = new float[size*2];
    float val;

    create_tree(arr, size, tmp, val);
    for (int i=0; i<size; i++) {
        output[i] = val;
        recreate(size, tmp, val);
    }

    delete[] tmp;
}

float* readInput(const char* filename, int& size) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Error: Failed to open input file.\n";
        size = 0;
        return nullptr;
    }

    string line;
    getline(inputFile, line);
    inputFile.close();

    // Count commas
    size = 0;
    for (char ch : line) {
        if (ch == ',') size++;
    }
    size += 1;

    float* arr = new float[size];
    string numStr = "";
    int idx = 0;

    for (char ch : line) {
        if (ch == ',') {
            // Trim spaces (optional)
            while (!numStr.empty() && isspace(numStr.back()))
                numStr.pop_back();
            arr[idx++] = atof(numStr.c_str());
            numStr = "";
        } else {
            if (!(numStr.empty() && isspace(ch))) // skip leading space
                numStr += ch;
        }
    }

    if (!numStr.empty()) {
        while (!numStr.empty() && isspace(numStr.back()))
            numStr.pop_back();
        arr[idx++] = atof(numStr.c_str());
    }

    return arr;
}

void writeOutput(const char* filename, float* arr, int size) {
    ofstream outputFile(filename);
    outputFile << size << std::endl;

    for (int i = 0; i < size; i++) {
        outputFile << arr[i];
        if (i != size - 1)
            outputFile << ", ";
    }

    outputFile.close();
}

void writeAnalysis(const char* filename, double durationSec, int size) {
    ofstream analysisFile(filename, ios::app);
    analysisFile << "Tournament Sort running time: " << durationSec << " sec (size = " << size << ")\n";
    analysisFile.close();
}

int main(int argc, char* argv[]) {
    string input_type = argv[1];
    const char* input_file;
    const char* output_file;
    const char* analysis_file;

    if (input_type == "sorted_inc") {
        input_file = "input_data/sorted_inc.txt";
        output_file = "sorted_nums/tournament_sorted_inc.txt";
        analysis_file = "analysis/tournament_sorted_inc.txt";
    } else if (input_type == "sorted_dec") {
        input_file = "input_data/sorted_dec.txt";
        output_file = "sorted_nums/tournament_sorted_dec.txt";
        analysis_file = "analysis/tournament_sorted_dec.txt";
    } else if (input_type == "random") {
        input_file = "input_data/random.txt";
        output_file = "sorted_nums/tournament_random.txt";
        analysis_file = "analysis/tournament_random.txt";
    } else if (input_type == "partial") {
        input_file = "input_data/partial.txt";
        output_file = "sorted_nums/tournament_partial.txt";
        analysis_file = "analysis/tournament_partial.txt";
    } else if (input_type == "test") {
        input_file = "input_data/test.txt";
        output_file = "sorted_nums/tournament_test.txt";
        analysis_file = "analysis/tournament_test.txt";
    } else if (input_type == "stability") {
        input_file = "input_data/stability.txt";
        output_file = "sorted_nums/tournament_stability.txt";
        analysis_file = "analysis/tournament_stability.txt";
    }

    int size = 0;
    float* numbers = readInput(input_file, size);
    std::cout << "Input size: " << size << std::endl;

    float* result = new float[size];

    clock_t start = clock();
    tournamentSort(numbers, size, result);
    clock_t end = clock();

    numbers = result;

    bool sorted = true;
    int i;
    for (i=0; i<size-1; i++) {
        if ((int)numbers[i] > (int)numbers[i+1]) {
            std::cerr << "Sorting Failed in [" << i << "]: " << (int)numbers[i] << ">" << (int)numbers[i+1] << std::endl;
            sorted = false;
            break;
        }
    }
    std::cout << "Sorting Done" << std::endl;
    
    if ((i+1)!=size) {
        std::cerr << "Sorting Failed: only has " << i << "elements, but needed " << size << "elements" << std::endl;
    } else {
        std::cout << "All elements included" << std::endl;
    }


    bool stable = true;
    for (i=0; i<size-1; i++) {
        if (numbers[i] > numbers[i+1]) {
            std::cerr << "Not stable in [" << i << "]: " << numbers[i] << ">" << numbers[i+1] << std::endl;
            stable = false;
            break;
        }
    }
    if (stable)
        std::cout << "This algorithm is Stable" << std::endl;
    else
        std::cout << "This algorithm is NOT Stable" << std::endl;


    double duration = (double)(end - start) / CLOCKS_PER_SEC;

    writeOutput(output_file, numbers, size);
    writeAnalysis(analysis_file, duration, size);

    delete[] numbers;
    return 0;
}