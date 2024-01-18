#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>

using namespace std;

// Huffman tree node structure
struct HuffmanNode {
    char data;
    unsigned frequency;
    HuffmanNode *left, *right;

    HuffmanNode(char data, unsigned frequency) : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Comparison function for priority queue
struct Compare {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->frequency > right->frequency;
    }
};

// Traverse the Huffman tree and assign codes
void generateCodes(HuffmanNode* root, string code, unordered_map<char, string>& huffmanCodes) {
    if (root) {
        if (!root->left && !root->right) {
            huffmanCodes[root->data] = code;
        }
        generateCodes(root->left, code + "0", huffmanCodes);
        generateCodes(root->right, code + "1", huffmanCodes);
    }
}

// Build the Huffman tree
HuffmanNode* buildHuffmanTree(const unordered_map<char, unsigned>& frequencyMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    for (auto& pair : frequencyMap) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* internalNode = new HuffmanNode('$', left->frequency + right->frequency);
        internalNode->left = left;
        internalNode->right = right;

        pq.push(internalNode);
    }

    return pq.top();
}

// Encode the text using Huffman codes
string encodeText(const string& text, const unordered_map<char, string>& huffmanCodes) {
    string encodedText = " ";
    for (char ch : text) {
        encodedText += huffmanCodes.at(ch);
    }
    return encodedText;
}

// Write Huffman codes and encoded text to a binary file
void writeCompressedFile(const string& originalText, const unordered_map<char, string>& huffmanCodes, const string& outputFile) {
    ofstream compressedFile(outputFile, ios::binary);

    // Write the Huffman codes to the file
    for (auto& pair : huffmanCodes) {
        compressedFile << pair.first << " " << pair.second << " ";
    }
    compressedFile << "\n";

    // Write the encoded text to the file
    for (char ch : originalText) {
        compressedFile << huffmanCodes.at(ch);
    }

    compressedFile.close();
}

int main() {
    string inputFile = "input.txt";
    string outputFile = "compressed.huff";

    // Read the input text file
    ifstream inFile(inputFile);
    string text((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();

    // Calculate character frequencies
    unordered_map<char, unsigned> frequencyMap;
    for (char ch : text) {
        frequencyMap[ch]++;
    }

    // Build the Huffman tree
    HuffmanNode* root = buildHuffmanTree(frequencyMap);

    // Generate Huffman codes
    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    // Encode the text using Huffman codes
    string encodedText = encodeText(text, huffmanCodes);

    // Write compressed data to a binary file
    writeCompressedFile(text, huffmanCodes, outputFile);

    cout << "Compression completed. Compressed file: " << outputFile << endl;

    return 0;
}
