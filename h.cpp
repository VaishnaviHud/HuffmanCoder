#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

struct MinHeapNode {
    char d;
    unsigned frequency;
    MinHeapNode *lChild, *rChild;

    MinHeapNode(char d, unsigned frequency) {
        lChild = rChild = NULL;
        this->d = d;
        this->frequency = frequency;
    }
};

// Function to compare nodes
struct compare {
    bool operator()(MinHeapNode *l, MinHeapNode *r) {
        return (l->frequency > r->frequency);
    }
};

// Helper function to print Huffman codes
void printCodes(struct MinHeapNode *root, string str, unordered_map<char, string> &huffmanCodes) {
    if (!root)
        return;

    if (root->d != '$') {
        huffmanCodes[root->d] = str;
    }

    printCodes(root->lChild, str + "0", huffmanCodes);
    printCodes(root->rChild, str + "1", huffmanCodes);
}

// Function to generate Huffman Codes
MinHeapNode* HuffmanCodes(char d[], int frequency[], int size, unordered_map<char, string> &huffmanCodes) {
    struct MinHeapNode *lChild, *rChild, *top;
    priority_queue<MinHeapNode *, vector<MinHeapNode *>, compare> minHeap;

    for (int i = 0; i < size; ++i)
        minHeap.push(new MinHeapNode(d[i], frequency[i]));

    while (minHeap.size() != 1) {
        lChild = minHeap.top();
        minHeap.pop();
        rChild = minHeap.top();
        minHeap.pop();
        top = new MinHeapNode('$', lChild->frequency + rChild->frequency);
        top->lChild = lChild;
        top->rChild = rChild;
        minHeap.push(top);
    }

    printCodes(minHeap.top(), "", huffmanCodes);
    return minHeap.top();
}

// Function to read file and calculate frequency of characters
void calculateFrequency(const string &filename, unordered_map<char, int> &frequencyMap) {
    ifstream file(filename);
    char ch;
    while (file.get(ch)) {
        frequencyMap[ch]++;
    }
    file.close();
}

// Function to encode the input file using Huffman codes
void encodeFile(const string &inputFile, const string &outputFile, const unordered_map<char, string> &huffmanCodes) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    char ch;

    while (inFile.get(ch)) {
        string code = huffmanCodes.at(ch);
        outFile << code;
    }

    inFile.close();
    outFile.close();
}

int main() {
    string inputFile = "input.txt";  // The file to be encoded
    string outputFile = "encoded.txt"; // The file to write the encoded output

    // Calculate character frequencies from the input file
    unordered_map<char, int> frequencyMap;
    calculateFrequency(inputFile, frequencyMap);

    // Prepare data for Huffman Coding
    vector<char> characters;
    vector<int> frequencies;

    for (const auto &pair : frequencyMap) {
        characters.push_back(pair.first);
        frequencies.push_back(pair.second);
    }

    unordered_map<char, string> huffmanCodes;
    MinHeapNode* root = HuffmanCodes(characters.data(), frequencies.data(), characters.size(), huffmanCodes);

    // Encode the input file
    encodeFile(inputFile, outputFile, huffmanCodes);

    // Print the Huffman codes
    cout << "Huffman Codes:\n";
    for (const auto &pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << "\n";
    }

    return 0;
}