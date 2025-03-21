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
        lChild = rChild = nullptr;
        this->d = d;
        this->frequency = frequency;
    }
};


struct compare {
    bool operator()(MinHeapNode *l, MinHeapNode *r) {
        return (l->frequency > r->frequency);
    }
};


void generateHuffmanCodes(MinHeapNode *root, string str, unordered_map<char, string> &huffmanCodes) {
    if (!root) return;

    if (root->d != '$') {
        huffmanCodes[root->d] = str;
    }

    generateHuffmanCodes(root->lChild, str + "0", huffmanCodes);
    generateHuffmanCodes(root->rChild, str + "1", huffmanCodes);
}


MinHeapNode* buildHuffmanTree(char d[], int frequency[], int size, unordered_map<char, string> &huffmanCodes) {
    priority_queue<MinHeapNode *, vector<MinHeapNode *>, compare> minHeap;

    for (int i = 0; i < size; ++i) {
        minHeap.push(new MinHeapNode(d[i], frequency[i]));
    }

    while (minHeap.size() != 1) {
        MinHeapNode *lChild = minHeap.top();
        minHeap.pop();
        MinHeapNode *rChild = minHeap.top();
        minHeap.pop();

        MinHeapNode *top = new MinHeapNode('$', lChild->frequency + rChild->frequency);
        top->lChild = lChild;
        top->rChild = rChild;
        minHeap.push(top);
    }

    MinHeapNode* root = minHeap.top();
    generateHuffmanCodes(root, "", huffmanCodes);
    return root;
}

void calculateFrequency(const string &filename, unordered_map<char, int> &frequencyMap) {
    ifstream file(filename);
    char ch;
    while (file.get(ch)) {
        frequencyMap[ch]++;
    }
    file.close();
}


void encodeFile(const string &inputFile, const string &encodedFile, const unordered_map<char, string> &huffmanCodes) {
    ifstream inFile(inputFile);
    ofstream outFile(encodedFile);
    char ch;

    while (inFile.get(ch)) {
        outFile << huffmanCodes.at(ch);
    }

    inFile.close();
    outFile.close();
}


void decodeFile(const string &encodedFile, const string &decodedFile, MinHeapNode* root) {
    ifstream inFile(encodedFile);
    ofstream outFile(decodedFile);
    char bit;
    MinHeapNode* curr = root;

    while (inFile.get(bit)) {
        curr = (bit == '0') ? curr->lChild : curr->rChild;

        if (curr->lChild == nullptr && curr->rChild == nullptr) { // Leaf node found
            outFile << curr->d;
            curr = root;
        }
    }

    inFile.close();
    outFile.close();
}

int main() {
    string inputFile = "input.txt"; 
    string encodedFile = "encoded.txt"; 
    string decodedFile = "decoded.txt"; 
  
    unordered_map<char, int> frequencyMap;
    calculateFrequency(inputFile, frequencyMap);

    
    vector<char> characters;
    vector<int> frequencies;

    for (const auto &pair : frequencyMap) {
        characters.push_back(pair.first);
        frequencies.push_back(pair.second);
    }

 
    unordered_map<char, string> huffmanCodes;
    MinHeapNode* root = buildHuffmanTree(characters.data(), frequencies.data(), characters.size(), huffmanCodes);

 
    encodeFile(inputFile, encodedFile, huffmanCodes);


    decodeFile(encodedFile, decodedFile, root);

    
    cout << "Huffman Codes:\n";
    for (const auto &pair : huffmanCodes) {
        cout << pair.first << ": " << pair.second << "\n";
    }

    cout << "\nEncoding completed. Encoded data is stored in '" << encodedFile << "'.";
    cout << "\nDecoding completed. Decoded output is stored in '" << decodedFile << "'.\n";

    return 0;
}
