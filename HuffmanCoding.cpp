#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

constexpr auto EMPTY_STRING = "";

//Tree node.
struct Node {
    char character;
    int weight;
    Node* left, * right;
};

// load a new tree node.
Node* loadNode(char character, int weight, Node* left, Node* right) {
    Node* node = new Node();

    node->character = character;
    node->weight = weight;
    node->left = left;
    node->right = right;

    return node;
}

// order the heap by compare objects. 
struct compare {
    bool operator()(Node* left, Node* right) {
        // the highest priority item has the lowest weight.
        return left->weight > right->weight;
    }
};

// check if the Huffman Tree is a single node.
bool isLeaf(Node* root) {
    return root->left == nullptr && root->right == nullptr;
}

// traverse the Huffman Tree and encoding. then store in a map.
void encode(Node* root, string str, unordered_map<char, string>& huffmanCode) {
    if (root == nullptr) {
        return;
    }

    // found a leaf node
    if (isLeaf(root)) {
        huffmanCode[root->character] = (str != EMPTY_STRING) ? str : "1";
    }

    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

// traverse the Huffman Tree and decoding.
void decode(Node* root, int& index, string str) {
    if (root == nullptr) {
        return;
    }

    // found a leaf node
    if (isLeaf(root)) {
        cout << root->character;
        return;
    }

    index++;

    if (str[index] == '0') {
        decode(root->left, index, str);
    }
    else {
        decode(root->right, index, str);
    }
}

// construct Huffman Tree and excute encoding and decoding.
void constructHuffmanTree(string text) {
    // if no string input.
    if (text == EMPTY_STRING) {
        return;
    }

    // count the weight of each character appear in input and store it in a map.
    unordered_map<char, int> weight;
    for (char character : text) {
        weight[character]++;
    }

    // a priority queue to store Huffman Tree nodes.
    priority_queue<Node*, vector<Node*>, compare> priorityQueue;

    // create a leaf node for each character and add in to the priority queue.
    for (auto pair : weight) {
        priorityQueue.push(loadNode(pair.first, pair.second, nullptr, nullptr));
    }

    // the node in the queue greater than 1.
    while (priorityQueue.size() != 1) {
        // remove the two nodes of the highest priority(the lowest weight) from the queue.

        Node* left = priorityQueue.top();
        priorityQueue.pop();
        Node* right = priorityQueue.top();
        priorityQueue.pop();

        // create a new node, the new node's weight is the sum of the previous two nodes.
        // the previous two nodes become the children of the new node.
        // add the new node to the priority queue.

        int sum = left->weight + right->weight;
        priorityQueue.push(loadNode('\0', sum, left, right));
    }

    // store the root of Huffman Tree from  queue.
    Node* root = priorityQueue.top();

    // traverse the Huffman Tree and store Huffman Code in a map. then print.
    unordered_map<char, string> huffmanCode;
    encode(root, EMPTY_STRING, huffmanCode);

    cout << "Huffman Codes are:" << endl;
    for (auto pair : huffmanCode) {
        cout << pair.first << " " << pair.second << '\n';
    }

    cout << "\n\nOriginal string is:\n" << text << endl;

    // print encoded string.
    string str;
    for (char character : text) {
        str += huffmanCode[character];
    }
    cout << "\n\nEncoded string is:\n" << str << endl;


    // print decoded string.
    cout << "\n\nDecoded string is:" << endl;
    if (isLeaf(root)) {
        // in case the input like a, aa, aaa, etc.
        while (root->weight--) {
            cout << root->character;
        }
    }
    else {
        // traverse the Huffman Tree again and decoding.
        int index = -1;
        while (index < (int)str.size() - 1) {
            decode(root, index, str);
        }
        cout << endl;
    }
}


int main() {
    // input string.
    string text = "I am finished the Huffman Coding algorithm. wooooooooow!";

    // construct Huffman Tree and excute encoding and decoding.
    constructHuffmanTree(text);
    return 0;
}