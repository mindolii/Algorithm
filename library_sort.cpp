#include <iostream>
#include <fstream>
#include <climits>

using namespace std;

const int gap = INT_MAX;

float binary_search(float* sorted, int size, float x) {
    int left = 0, right = size - 1;
    int pos = size; 

    while (left <= right) {
        int mid = (left + right) / 2;

        if (sorted[mid] == gap || (int)sorted[mid] >= (int)x) {
            pos = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    return pos;
}

void insert_with_gap(float*sorted, int&count, float x, int total_size) {
    int pos = binary_search(sorted, total_size, x);

    int left = pos;
    while (left >= 0 && sorted[left] != gap)
        left--;

    if (left >= 0) {
        for (int i=pos; i>left; i--)
            sorted[i] = sorted[i-1];
        sorted[left] = x;
    } else {
        int right = pos;
        while (right < total_size && sorted[right] != gap)
            right++;
        
        for (int i=right; i>pos; i--)
            sorted[i] = sorted[i-1];
        sorted[pos] = x;
    }
    count++;
}

void rebalance(float* sorted, int& total_size, int count) {
    int new_size = int(total_size * 2);
    float* new_sorted = new float[new_size];
    for (int i=0; i<new_size; i++)
        new_sorted[i] = gap;

    int block = new_size / (count+1);
    int j = block;

    for (int i=0; i<total_size; i++) {
        if (sorted[i] != gap) {
            new_sorted[j] = sorted[i];
            j += block;
        }
    }

    delete[] sorted;
    sorted = new_sorted;
    total_size = new_size;
}

void librarySort(float* arr, int size, float* output) {
    int cap = int(size*2) + 1;
    float* sorted = new float[cap];
    for (int i=0; i<cap; i++)
        sorted[i] = gap;

    int count = 0;

    for (int i=0; i<size; i++) {
        insert_with_gap(sorted, count, arr[i], cap);

        if (count*2 > cap)
            rebalance(sorted, cap, count);
    }

    int ind;
    for (int i=0; i<cap; i++) {
        if (sorted[i] != gap)
            output[ind++] = sorted[i];
    }

    delete[] sorted;
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

    analysisFile << "Library Sort running time: " << durationSec << " sec (size = " << size << ")\n";
    analysisFile.close();
}

int main(int argc, char* argv[]) {
    string input_type = argv[1];
    const char* input_file;
    const char* output_file;
    const char* analysis_file;

    if (input_type == "sorted_inc") {
        input_file = "input_data/sorted_inc.txt";
        output_file = "sorted_nums/library_sorted_inc.txt";
        analysis_file = "analysis/library_sorted_inc.txt";
    } else if (input_type == "sorted_dec") {
        input_file = "input_data/sorted_dec.txt";
        output_file = "sorted_nums/library_sorted_dec.txt";
        analysis_file = "analysis/library_sorted_dec.txt";
    } else if (input_type == "random") {
        input_file = "input_data/random.txt";
        output_file = "sorted_nums/library_random.txt";
        analysis_file = "analysis/library_random.txt";
    } else if (input_type == "partial") {
        input_file = "input_data/partial.txt";
        output_file = "sorted_nums/library_partial.txt";
        analysis_file = "analysis/library_partial.txt";
    } else if (input_type == "test") {
        input_file = "input_data/test.txt";
        output_file = "sorted_nums/library_test.txt";
        analysis_file = "analysis/library_test.txt";
    } else if (input_type == "stability") {
        input_file = "input_data/stability.txt";
        output_file = "sorted_nums/library_stability.txt";
        analysis_file = "analysis/library_stability.txt";
    }

    int size = 0;
    float* numbers = readInput(input_file, size);
    std::cout << "Input size: " << size << std::endl;

    float* result = new float[size];

    clock_t start = clock();
    librarySort(numbers, size, result);
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