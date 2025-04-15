#include <iostream>
#include <cmath>
#include <fstream>
#include <ctime>

using namespace std;

const int INSERTION_SORT_THRESHOLD = 16;

// Insertion Sort
void insertionSort(float* arr, int left, int right) {
    for (int i = left + 1; i <= right; ++i) {
        float key = arr[i];
        int j = i - 1;
        while (j >= left && (int)arr[j] > (int)key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int partition(float* arr, int left, int right) {
    float x = arr[right];
    int i = left - 1;
    
    float a, b;
    for (int j=left; j<right; j++) {
        if ((int)arr[j] <= (int)x) {
            i += 1;

            a = arr[i];
            b = arr[j];

            arr[i] = b;
            arr[j] = a;
        }
    }

    a = arr[i+1];
    b = arr[right];

    arr[i+1] = b;
    arr[right] = a;

    return (i+1);
}

void max_heapify(float* arr, int heap_size, int ind) {
    int left = 2*ind + 1;
    int right = 2*ind + 2;

    int largest;
    if ((left < heap_size) && ((int)arr[left]>(int)arr[ind])) {
        largest = left;
    } else {
        largest = ind;
    }

    if ((right < heap_size) && ((int)arr[right] > (int)arr[largest])) {
        largest = right;
    }

    if (largest != ind) {
        float a = arr[ind];
        float b = arr[largest];
        
        arr[ind] = b;
        arr[largest] = a;

        max_heapify(arr, heap_size, largest);
    }
}

void build_max_heap(float* arr, int size) {
    for (int i=(size-1)/2; i>-1; i--)
        max_heapify(arr, size, i);
}

void heapSort(float* arr, int size) {
    build_max_heap(arr, size);
    float largest, last;
    for (int i=size-1; i>0; i--) {
        largest = arr[0];
        last = arr[i];

        arr[0] = last;
        arr[i] = largest;

        max_heapify(arr, i, 0);
    }
}


// Recursive utility for IntroSort
void introSortUtil(float* arr, int low, int high, int depth_limit) {
    int size = high - low + 1;

    if (size <= INSERTION_SORT_THRESHOLD) {
        insertionSort(arr, low, high);
        return;
    }

    if (depth_limit == 0) {
        heapSort(arr + low, size);
        return;
    }

    int pivot = partition(arr, low, high);
    introSortUtil(arr, low, pivot - 1, depth_limit - 1);
    introSortUtil(arr, pivot + 1, high, depth_limit - 1);
}

// Entry point
void introSort(float* arr, int n) {
    int depth_limit = 2 * log2(n);
    introSortUtil(arr, 0, n - 1, depth_limit);
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

    analysisFile << "Intro Sort running time: " << durationSec << " sec (size = " << size << ")\n";
    analysisFile.close();
}

int main(int argc, char* argv[]) {
    string input_type = argv[1];
    const char* input_file;
    const char* output_file;
    const char* analysis_file;

    if (input_type == "sorted_inc") {
        input_file = "input_data/sorted_inc.txt";
        output_file = "sorted_nums/intro_sorted_inc.txt";
        analysis_file = "analysis/intro_sorted_inc.txt";
    } else if (input_type == "sorted_dec") {
        input_file = "input_data/sorted_dec.txt";
        output_file = "sorted_nums/intro_sorted_dec.txt";
        analysis_file = "analysis/intro_sorted_dec.txt";
    } else if (input_type == "random") {
        input_file = "input_data/random.txt";
        output_file = "sorted_nums/intro_random.txt";
        analysis_file = "analysis/intro_random.txt";
    } else if (input_type == "partial") {
        input_file = "input_data/partial.txt";
        output_file = "sorted_nums/intro_partial.txt";
        analysis_file = "analysis/intro_partial.txt";
    } else if (input_type == "test") {
        input_file = "input_data/test.txt";
        output_file = "sorted_nums/intro_test.txt";
        analysis_file = "analysis/intro_test.txt";
    } else if (input_type == "stability") {
        input_file = "input_data/stability.txt";
        output_file = "sorted_nums/intro_stability.txt";
        analysis_file = "analysis/intro_stability.txt";
    }

    int size = 0;
    float* numbers = readInput(input_file, size);
    std::cout << "Input size: " << size << std::endl;

    clock_t start = clock();
    introSort(numbers, size);
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