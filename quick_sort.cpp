#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

int partition(float* arr, int left, int right) {
    float x = arr[right];
    int i = left - 1;
    
    float a, b;
    for (int j=left; j<right; j++) {
        if ((int)arr[j] < (int)x) {
            i += 1;

            a = arr[i];
            b = arr[j];

            arr[i] = b;
            arr[j] = a;
        } else if ((int)arr[j] >= (int)x) {
            continue;
        }
    }

    a = arr[i+1];
    b = arr[right];

    arr[i+1] = b;
    arr[right] = a;

    return (i+1);
}

void quickSort(float* arr, int left, int right) {
    int mid;
    if (left < right) {
        mid = partition(arr, left, right);
        quickSort(arr, left, mid-1);
        quickSort(arr, mid+1, right);
    }
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

    analysisFile << "Quick Sort running time: " << durationSec << " sec (size = " << size << ")\n";
    analysisFile.close();
}

int main(int argc, char* argv[]) {
    string input_type = argv[1];
    const char* input_file;
    const char* output_file;
    const char* analysis_file;

    if (input_type == "sorted_inc") {
        input_file = "input_data/sorted_inc.txt";
        output_file = "sorted_nums/quick_sorted_inc.txt";
        analysis_file = "analysis/quick_sorted_inc.txt";
    } else if (input_type == "sorted_dec") {
        input_file = "input_data/sorted_dec.txt";
        output_file = "sorted_nums/quick_sorted_dec.txt";
        analysis_file = "analysis/quick_sorted_dec.txt";
    } else if (input_type == "random") {
        input_file = "input_data/random.txt";
        output_file = "sorted_nums/quick_random.txt";
        analysis_file = "analysis/quick_random.txt";
    } else if (input_type == "partial") {
        input_file = "input_data/partial.txt";
        output_file = "sorted_nums/quick_partial.txt";
        analysis_file = "analysis/quick_partial.txt";
    } else if (input_type == "test") {
        input_file = "input_data/test.txt";
        output_file = "sorted_nums/quick_test.txt";
        analysis_file = "analysis/quick_test.txt";
    } else if (input_type == "stability") {
        input_file = "input_data/stability.txt";
        output_file = "sorted_nums/quick_stability.txt";
        analysis_file = "analysis/quick_stability.txt";
    }

    int size = 0;
    float* numbers = readInput(input_file, size);
    std::cout << "Input size: " << size << std::endl;

    clock_t start = clock();
    quickSort(numbers, 0, size);
    clock_t end = clock();

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