/*
159.333 Programming Project
Supervisor: Dr. Andre Barczak
Author:		Yiqin Wang
ID:			17280498
*/

#include<iostream>
#include<string>
#include<fstream>
using namespace std;

struct Node {
	char Code;		// the character
	double Rate;	// the character appearance ratio
	string coding;	// Huffman Code
	Node* Next;
	Node* parent;	// parent pointer
	Node* left;		// left pointer
	Node* right;	// right pointer
};

class Scanner {
public:
	string text;
	ifstream in;
	string filename;
	Node* Character;
	int num;
	Scanner() :num(0) {}

	void Run() {
		cout << "Please type the file name: ";
		cin >> filename;
		in.open(filename);

		while (!in) {
			cout << "Failed to read file. Please re-type the file name(file direction matters): " << endl;
			getline(cin, filename, '\n');
			in.open(filename);
		}

		Character = new Node();
		Node* p1; Node* p2;
		char temp;
		p1 = p2 = Character;
		num = 0;

		// read file
		while (!in.eof()) {
			in.read(&temp, 1);

			if (in.eof()) {
				break;
			}

			num++;
			if (num == 1) {
				Character->Code = temp;
				Character->Rate = 0;
				Character->Rate++;
				Character->right = Character->left = Character->parent = nullptr;
				Character->Next = nullptr;
			}
			else {
				if (IsANew(temp)) {
					p2 = new Node();
					p2->Code = temp;
					p2->Rate = 0;
					p2->Rate++;
					p2->left = p2->right = p2->parent = nullptr;
					p1->Next = p2;
					p1 = p1->Next;
					p1->Next = nullptr;
				}
			}
		}
		in.close();
		cout << endl;

		GetRate(num);
		Sort();
		
		cout << endl << "              read file success                   " << endl;
		
		// show characters and their appearance ratio
		cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
		for (Node* temp = Character; temp != nullptr; temp = temp->Next) {
			if (temp->Code == 10 || temp->Code == 13) {
				cout << "¡ó " << "Newline" << "  ";
				printf("%.4f\t\t\t", temp->Rate);
			}
			else if (temp->Code == 32) {
				cout << "¡ó " << "Space" << "  ";
				printf("%.4f\t\t\t", temp->Rate);
			}
			else {
				cout << "¡ó " << temp->Code << "  ";
				printf("%.4f\t\t\t", temp->Rate);
			}

		}
	}
	
	// determine the character read in or not
	bool IsANew(char temp1) {
		Node* temp;

		for (temp = Character; temp != nullptr; temp = temp->Next) {
			if ((*temp).Code == temp1) {
				temp->Rate++;
				return false;
			}
		}
		return true;
	}

	// calculate the appearance ratio of the character
	void GetRate(int num) {
		for (Node* temp = Character; temp != nullptr; temp = temp->Next) {
			temp->Rate = (double)(temp->Rate / num);
		}
	}

	
	void Sort() { 
		Node* temp;
		Node* temp1;
		double Rate;
		char code;
		for (temp = Character; temp->Next != nullptr; temp = temp->Next) {
			temp1 = temp;	// temp points to the element which needs to sort
			int i = 1;
			Node* temp2;	
			temp2 = temp1->Next;
			while (temp2 != nullptr) {
				if (temp2->Rate < temp1->Rate) {
					temp1 = temp2;	// temp1 points to the least node in current loop
				}
				temp2 = temp2->Next;
			}
			// exchange the least node and the element which needs to sort in current loop
			code = temp1->Code;
			Rate = temp1->Rate;
			temp1->Code = temp->Code;
			temp1->Rate = temp->Rate;
			temp->Code = code;
			temp->Rate = Rate;
		}
	}
};