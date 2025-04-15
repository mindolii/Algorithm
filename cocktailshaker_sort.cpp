#include <iostream>
#include <fstream>

using namespace std;

void cocktailSort(float* arr, int size) {
    bool swapped = true;

    while (swapped) {
        swapped = false;
        int start = 0;
        int end = size-1;

        for (int i=start; i<end; i++) {
            if ((int)arr[i] > (int)arr[i+1]) {
                float a = arr[i];
                float b = arr[i+1];

                arr[i] = b;
                arr[i+1] = a;

                swapped = true;
            } else if ((int)arr[i] <= (int)arr[i+1]) {
                continue;
            }
        }

        if (!swapped) {
            break;
        }

        swapped = false;
        end--;

        for (int i=end; i>start; i--) {
            if ((int)arr[i] < (int)arr[i-1]) {
                float a = arr[i];
                float b = arr[i-1];

                arr[i] = b;
                arr[i-1] = a;

                swapped = true;
            } else if ((int)arr[i] >= (int)arr[i-1]) {
                continue;
            }
        }
        
        start++;

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

    analysisFile << "Cocktail Shaker Sort running time: " << durationSec << " sec (size = " << size << ")\n";
    analysisFile.close();
}

int main(int argc, char* argv[]) {
    string input_type = argv[1];
    const char* input_file;
    const char* output_file;
    const char* analysis_file;

    if (input_type == "sorted_inc") {
        input_file = "input_data/sorted_inc.txt";
        output_file = "sorted_nums/cocktailshaker_sorted_inc.txt";
        analysis_file = "analysis/cocktailshaker_sorted_inc.txt";
    } else if (input_type == "sorted_dec") {
        input_file = "input_data/sorted_dec.txt";
        output_file = "sorted_nums/cocktailshaker_sorted_dec.txt";
        analysis_file = "analysis/cocktailshaker_sorted_dec.txt";
    } else if (input_type == "random") {
        input_file = "input_data/random.txt";
        output_file = "sorted_nums/cocktailshaker_random.txt";
        analysis_file = "analysis/cocktailshaker_random.txt";
    } else if (input_type == "partial") {
        input_file = "input_data/partial.txt";
        output_file = "sorted_nums/cocktailshaker_partial.txt";
        analysis_file = "analysis/cocktailshaker_partial.txt";
    } else if (input_type == "test") {
        input_file = "input_data/test.txt";
        output_file = "sorted_nums/cocktailshaker_test.txt";
        analysis_file = "analysis/cocktailshaker_test.txt";
    } else if (input_type == "stability") {
        input_file = "input_data/stability.txt";
        output_file = "sorted_nums/cocktailshaker_stability.txt";
        analysis_file = "analysis/cocktailshaer_stability.txt";
    }

    int size = 0;
    float* numbers = readInput(input_file, size);
    std::cout << "Input size: " << size << std::endl;

    clock_t start = clock();
    cocktailSort(numbers, size);
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