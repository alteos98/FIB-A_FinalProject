#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <fstream>

using namespace std;

void usageJaccardSimilarityKShingles() {
	cout << "Usage:" << endl;
	cout << "	arg0 -> prog" << endl;
	cout << "	arg1 -> path document 1" << endl;
	cout << "	arg2 -> path document 2" << endl;
	cout << "	arg3 -> k" << endl;
	exit(1);
}

void error(string message) {
	cout << message << endl;
	exit(1);
}

void read(char* path1, char* path2, vector<string>& starterDocumentA, vector<string>& starterDocumentB) {
	ifstream fileA, fileB;

	// Obrir primer fitxer
    fileA.open(path1);
    if (not fileA.is_open()) error("El primer fitxer no existeix");

	// Obrir segon fitxer
    fileB.open(path2);
    if (not fileB.is_open()) error("El segon fitxer no existeix");
    
    // Llegir
    string x;
    while (fileA >> x) {
    	starterDocumentA.push_back(x);
	}
    while (fileB >> x) {
    	starterDocumentB.push_back(x);
	}
    
    // Tancar
    fileA.close();
    fileB.close();
}

void printVector(vector<string>& v) {
	cout << v[0];
	for (unsigned int i=1; i < v.size(); ++i) {
		cout << " " << v[i];
	}
	cout << endl;
}

void printUnorderedSet(unordered_set<string>& set) {
	for (string s : set) {
		cout << s << endl;
	}
	cout << endl;
}
