#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <fstream>

#include "inputOutput.h"

using namespace std;

class JaccardSimilarityKShingles {
	private:
		unordered_set<string> documentA;
		unordered_set<string> documentB;
		double jaccardSimilarity;
	
	public:
		JaccardSimilarityKShingles() {
			jaccardSimilarity = 0.0;
		}
		
		JaccardSimilarityKShingles(unordered_set<string> documentA, unordered_set<string> documentB) {
			this->documentA = documentA;
			this->documentB = documentB;
			jaccardSimilarity = 0.0;
		}
		
		double getJaccardSimilarity() {
			return this->jaccardSimilarity;
		}
		
		void algorithm() {
			double intersectionAB = findIntersection();
			double unionAB = findUnion();
			this->jaccardSimilarity = intersectionAB / unionAB;
		}
	
	private:
		double findIntersection() {
			double res = 0.0;
			for (string s : documentA) {
				if (documentB.count(s) > 0) ++res;
			}
			return res;
		}
		
		double findUnion() {
			unordered_set<string> auxiliar;
			for (string s : documentA)
				auxiliar.insert(s);
			for (string s : documentB)
				auxiliar.insert(s);
			return auxiliar.size();
		}
	
};

void kShingles(vector<string>& starterDocument, unordered_set<string>& document, int k) {
	if (k > starterDocument.size()) error("La k no pot ser major que el nombre de paraules del document");
    for (int i=0; i < starterDocument.size() - k + 1; ++i) {
    	string s;
    	for (int j=i; j < i + k; ++j) {
    		if (i == j) s = starterDocument[j];
    		else s += " " + starterDocument[j];
		}
		document.insert(s);
	}
}

void write(JaccardSimilarityKShingles p) {
	cout << "El valor de la similitud de Jaccard obtingut: " << p.getJaccardSimilarity() << endl;
}

int main(int argc, char* argv[]) {
	if (argc != 3) usage();
	
	int k; vector<string> starterDocumentA, starterDocumentB;
	cout << "Entra el valor de k:" << endl; cin >> k;
	
	read(argv[1], argv[2], starterDocumentA, starterDocumentB);
	
	unordered_set<string> documentA, documentB;
	kShingles(starterDocumentA, documentA, k); kShingles(starterDocumentB, documentB, k);
	JaccardSimilarityKShingles p(documentA, documentB);
	p.algorithm();
	
	write(p);
	return 0;
}
