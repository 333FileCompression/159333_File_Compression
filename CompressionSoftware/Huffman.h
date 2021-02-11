/*
159.333 Programming Project
Supervisor: Dr. Andre Barczak
Author:		Yiqin Wang
ID:			17280498
*/

#include"Scanner.h"
#include<queue>
#include<vector>
typedef string filename;
class HuffmanCoding {
public:
	Scanner scanner;	// read the source file and organize its characters
	Node* BinaryTree;	// root node
	ofstream out;
	ifstream in;
	filename source;	// the name of source file
	filename objecthuf; // the name of compressed file
	filename objectbin; // the name of decompressed file
	int CharLength; 
	int HuffLength;		// compressed character length
	int zero;			// the bits num of the code that less than 8 bits 
	vector<Node*> position;  // the tree after LDR, used for return value which is how many Space character should be print
	vector<Node*> newline;   //store the node which determines whether start a new line
	
	// Constructor sets the HuffLength(compressed character length) to 0 and call the main menu which is Function().
	HuffmanCoding() :HuffLength(0) {
		Function(); // menu
	}
	
	// Run the program and it will show the main menu. It calls two functions which allow user to choose compression or decompression. 
	void Function() {
		int Select;
		cout << endl << endl <<  "Welcom to use the Compression software" << endl;
		cout << "Choose the option: " << endl;
		cout << "1. open a file and compress it" << endl;
		cout << "2. choose a compressed file and decompress it" << endl << endl;
		cin >> Select;
		switch (Select) {
			case 1:
				CompressFile();
				break;
			case 2:
				DecompressFile();
				break;
			default:
				return;
		}
	}
	
	// Compression process 
	void CompressFile() {
		Preparation();
		cout << "   Compressing..." << endl;
		cout << endl << endl;
		
		// two binary file to store the Huffman codes 
		filename binary = "binary.txt";
		filename binary2 = "binary2.txt";
		char temp;
		
		// open file
		in.open(source, ios::binary);
		out.open(binary, ios::binary);

		// read every characters in source file then convert to binary 
		while (!in.eof()) {
			string s;
			in.read(&temp, 1);
			if (in.eof()) {
				break;
			}

			// find every character's corresponding Huffman code
			CharToHufCode(temp, BinaryTree, s);
			out << s;
		}
		
		// close file
		out.close();
		in.close();
		// finish source file convert to binary file above
		
		// convert binary file to compressed file below
		in.open(binary, ios::binary);
		out.open(objecthuf, ios::binary);
		int num = 0;
		string stemp;
		while (!in.eof()) {

			in.read(&temp, 1);
			if (in.eof()) {
				break;
			}

			num++;
			stemp = stemp + temp;
			// convert 8 bits binary to char 
			if (num % 8 == 0) {
				HuffLength++;
				char c;
				int m;
				BinaryToChar(stemp, c);
				m = (int)c;
				out << c;
				stemp = "";
			}
		}
		HuffLength++;
		
		// if remaining code less than 8 bits, make it become 8 bits
		zero = 8 - stemp.size();
		char c;
		int m;
		for (int i = 0; i < zero; i++) {
			stemp.push_back('0');
		}
			
		BinaryToChar(stemp, c);
		m = (int)c;
		out << c;
		// close file 
		out.close();
		in.close();
		cout << "-------------------------------------------------------------------------" << endl;
		cout << "                            File Compress Successfully" << endl;
		cout << "-------------------------------------------------------------------------" << endl;
		getchar();
		// finishe the convert from binary to char above
		ShowSelection();
	}

	// Decompression process
	void DecompressFile() {
		filename binary2 = "binary2.txt";
		cout << "Please type the file name which need to decompress: ";
		cin >> objecthuf;
		cout << endl;
		cout << "Please type the decompressed file name: ";
		cin >> objectbin;
		cout << "-------------------------------------------------------------------------" << endl;
		cout << "                              Decompressing..." << endl;
		cout << "-------------------------------------------------------------------------" << endl;
		
		// open file
		in.open(objecthuf, ios::binary);
		out.open(binary2, ios::binary);
		char temp;
		int num1 = 0;
		while (!in.eof()) {
			string s;
			in.read(&temp, 1);
			if (in.eof()) {
				break;
			}
			
			num1++;
			// process the last character in compressed file 
			if (num1 == HuffLength) {
				string ss;
				CharToBinary(temp, ss);
				for (int i = 0; i < 8 - zero; i++) {
					s.push_back(ss[i]);
				}
				out << s;
				break;
			}
			CharToBinary(temp, s);
			out << s;
		}
		
		// close file
		out.close();
		in.close();
		// convert binary file to source file and open it
		in.open("binary.txt", ios::binary); //ios::binary  make sure read file from begin to end
		out.open(objectbin);
		string s;
		char c;			// store current binary code's corresponding character
		int flag = 0;	// determine current binary code can find corresponding character
		while (!in.eof()) {
			in.read(&temp, 1);
			if (in.eof()) {
				break;
			}

			s = s + temp;
			// determine current read binary code(Huffman Code) find corresponding character or not
			FindCharByBinary(c, BinaryTree, s, flag);
			if (flag == 1) {
				s = "";
				out << c;
			}
		}
		
		// close file
		in.close();
		out.close();
		cout << "-------------------------------------------------------------------------" << endl;
		cout << "                              Decompress succssfully. " << endl;
		cout << "                              You can see it in the direction." << endl;
		cout << "-------------------------------------------------------------------------" << endl;
		Function();
	}

	// scan the file and read the characters and calculate its appearance ratio
	void Initialization() {
		scanner.Run();
		Copy();			// copy the linked list from file and create a new linked list
		source = scanner.filename;
		CharLength = scanner.num;
	}

	// Before Compression   
	// 1.imitialization
	// 2.call the create function of Huffman Tree
	// 3.get every character's Huffman Code
	void Preparation() {
		Initialization();
		BinaryTree = CreateHuffmanTree();
		BinaryTree->coding = "";
		Node* leaf;
		leaf = BinaryTree;
		GetCode(leaf);
		Node* temp = BinaryTree;
		SavePositionInTree(temp);
		NewLine(temp);
		cout << endl;
		cout << "-------------------------------------------------------------------------" << endl;
		cout << endl << "Type the compressed file name: ";
		cin >> objecthuf;
		cout << "-------------------------------------------------------------------------" << endl;

	}

	// create Huffman Tree
	Node* CreateHuffmanTree() {
		Node* p = nullptr;
		// while root node not null
		while (BinaryTree != nullptr) {
			// when the set of nodes only has last two nodes
			if (BinaryTree->Next->Next == nullptr) {
				p = new Node();
				p->right = BinaryTree;
				BinaryTree->coding = '1';
				BinaryTree->parent = p;
				BinaryTree = BinaryTree->Next;
				p->left = BinaryTree;
				BinaryTree->coding = '0';
				BinaryTree->parent = p;
				p->Rate = p->left->Rate + p->right->Rate;
				p->parent = nullptr;
				p->Code = (char)3;
				BinaryTree = p;
				break;
			}
			
			// everytime choose two least nodes within the linked list sort by appearance, then build up to new node. 
			p = new Node();
			p->right = BinaryTree;
			BinaryTree->coding = '1';
			BinaryTree->parent = p;
			BinaryTree = BinaryTree->Next;
			p->left = BinaryTree;
			BinaryTree->coding = '0';
			BinaryTree->parent = p;
			BinaryTree = BinaryTree->Next;
			p->Rate = p->left->Rate + p->right->Rate;
			p->Code = (char)3;
			
			// insert the new node into appropriate position in linked list 
			InsertNewNode(p);
		}
		return p;
	}

	// copy the linked list from file and create a new linked list
	void Copy() {
		Node* p1;
		Node* p2;
		Node* temp;
		BinaryTree = new Node();
		BinaryTree->Code = scanner.Character->Code;
		BinaryTree->Rate = scanner.Character->Rate;
		BinaryTree->left = BinaryTree->right = BinaryTree->parent = nullptr;
		p1 = p2 = BinaryTree;
		for (temp = scanner.Character; temp != nullptr; temp = temp->Next) {
			
			if (temp == scanner.Character) {
				BinaryTree = scanner.Character;
			}

			p1 = new Node();
			p1->Code = temp->Code;
			p1->Rate = temp->Rate;
			p1->left = p1->right = p1->parent = nullptr;
			p2->Next = p1;
			p2 = p2->Next;
		}
		p1->Next = nullptr;
	}

	// insert new node to the linked list
	void InsertNewNode(Node* p) {

		Node* temp = BinaryTree;
		Node* front = temp;	// point to pre-pointer of current insert position
		temp = BinaryTree->Next;
		// when a new node insert into head of linked list
		if (p->Rate < front->Rate || p->Rate == front->Rate) { // less than first element
			p->Next = BinaryTree;
			BinaryTree = p;
		}
		else {
			for (; temp != nullptr; temp = temp->Next, front = front->Next) {
				
				if (p->Rate < temp->Rate || p->Rate == temp->Rate) {
					front->Next = p;
					p->Next = temp;
					break;
				}
			}
			if (temp == nullptr) {
				front->Next = p;
				p->Next = nullptr;
			}
		}
	}

	// produce the Huffman Code of node
	void GetCode(Node* leaf) {
		if (leaf != nullptr) {
			if (leaf != BinaryTree) {
				leaf->coding = leaf->parent->coding + leaf->coding;
			}
				
			if (leaf->right == nullptr && leaf->left == nullptr) {
				return;
			}
				
			GetCode(leaf->left);
			GetCode(leaf->right);
		}
	}

	void CharToHufCode(char c, Node* leaf, string& s) {
		if (leaf != nullptr) {
			if (leaf->Code == c) {
				s = leaf->coding;
				return;
			}
			CharToHufCode(c, leaf->left, s);
			CharToHufCode(c, leaf->right, s);
		}
	}

	// determine current read binary code(Huffman Code) find the corresponding character
	void FindCharByBinary(char& c, Node* leaf, string s, int& flag) {
		if (leaf != nullptr) {
			// the condition when find the corresponding character
			if (s.empty()) {
				if ((leaf->left == nullptr) && (leaf->right == nullptr)) {
					c = leaf->Code;
					flag = 1;
				}
				else {
					flag = 0;
				}
			}
			// keep finding as long as string is not null
			else {
				if (s[0] == '0') {
					popfront(s);
					FindCharByBinary(c, leaf->left, s, flag);
				}
				else {
					popfront(s);
					FindCharByBinary(c, leaf->right, s, flag);
				}
			}
		}
		else {
			flag = 0;
		}
	}

	// pop up the first element of a string
	void popfront(string& s) {
		for (int i = 1; i < s.size(); i++) {
			s[i - 1] = s[i];
		}
		s.pop_back();
	}

	// change the 8 bits binary to ASCII character
	void BinaryToChar(string s, char& c) {
		c = 0;
		int m = 1;
		for (int i = 7; i >= 0; i--) {
			c = (char)(c + (s[i] - '0') * m);
			m = m * 2;
		}
	}
	
	// change the ASCII character to binary
	void CharToBinary(char c, string& s) {
		unsigned char k = 0x80;
		s = "";
		for (int i = 0; i < 8; i++, k >>= 1) {
			if (c & k) {
				s = s + '1';
			}
			else {
				s = s + '0';
			}
		}
	}

	// calculate the compression ratio
	void CalculateRate() {
		cout << "---------------------------------------------------------------------------" << endl;
		cout << "               source file size is " << CharLength << " byte" << endl;
		cout << "               compressed file size is " << HuffLength << " byte." << endl;
		cout << "               compression ratio is " << (double)HuffLength / CharLength << endl;
		cout << "---------------------------------------------------------------------------" << endl;
	}

	// the menu after compress
	void ShowSelection() {
		int Select = 0;
		cout << endl;
		cout << "Choose the option: " << endl;
		cout << "1. check the Huffman Code of characters in file" << endl;
		cout << "2. check the source file" << endl;
		cout << "3. check the compression ratio" << endl;
		cout << "4. back to main menu" << endl << endl;
		while (Select < 4) {
			cin >> Select;
			switch (Select) {
				case 1:
					ShowCode(BinaryTree);
					break;
				case 2:
					ShowSource();
					break;
				case 3:
					CalculateRate();
					break;
				case 4:
					Function();
					break;
				default:
					Function();
					break;
			}
			getchar();
			cout << endl;
			cout << "Choose the option: " << endl;
			cout << "1. check the Huffman Code of characters in file" << endl;
			cout << "2. check the source file" << endl;
			cout << "3. check the compression ratio" << endl;
			cout << "4. back to main menu" << endl;
		}
	}
	
	// store the tree in LDR
	void SavePositionInTree(Node* leaf) {
		if (leaf != nullptr) {
			SavePositionInTree(leaf->left);
			position.push_back(leaf);
			SavePositionInTree(leaf->right);
		}
		else {
			return;
		}
	}
	
	// store the node which determines whether start a new line
	void NewLine(Node* leaf) {
		if (leaf != nullptr) {
			newline.push_back(leaf);
			NewLine(leaf->right);
			if (leaf->right == nullptr) {
				if (leaf->parent->left->right != nullptr) {
					newline.push_back(leaf->parent->left->right);
					NewLine(leaf->parent->left->right);
				}
			}
		}
	}
	
	// determine the current node should be a node that start a new line
	bool IsNewLine(Node* leaf) {
		bool flag = false;
		for (Node* temp : newline) {
			if (temp == leaf) {
				flag = true;
				return flag;
			}
		}
		return flag;
	}
	
	// the return value is how many Space character should be print before print current node
	int ReturnThePosition(Node* leaf) {
		int i = 0;
		for (auto temp : position) {
			i++;
			if (temp == leaf) {
				return i;
			}
		}
	}

	void ShowCode(Node* leaf) {
		if (leaf != nullptr) {
			if (leaf->right == nullptr && leaf->left == nullptr) {
				if (leaf->Code == 10 || leaf->Code == 13) {
					cout << "      " << "Newline" " ---->" << leaf->coding << endl;
				}					
				else if (leaf->Code == 32) {
					cout << "      " << "Space" << " ----> " << leaf->coding << endl;
				}
				else {
					cout << "      " << leaf->Code << " ----> " << leaf->coding << endl;
				}					
				return;
			}
			ShowCode(leaf->left);
			ShowCode(leaf->right);
		}
	}

	void ShowSource() {
		cout << endl << "----------------------------------------------------------------------------------------" << endl;
		in.open(source);
		char temp;
		while (!in.eof()) {
			in.read(&temp, 1);
			if (in.eof()) {
				break;
			}
			cout << temp;
		}
		in.close();
		cout << "\n\nThe file is：" << source << "\t  words count: " << CharLength << endl;
		cout << endl << "----------------------------------------------------------------------------------------" << endl;
	}
};
