#define DEBUG 0

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <climits>
#include <ctime>

#include "Headers/inputOutput.h"

using namespace std;

void usage() {
	cout << "Usage:" << endl;
	cout << "	arg0 -> prog" << endl;
	cout << "	arg1 -> path document 1" << endl;
	cout << "	arg2 -> path document 2" << endl;
	cout << "	arg3 -> t hash functions" << endl;
	exit(1);
}

bool isPrime(int num) {
	int i, count = 0;
    if (num == 0)
    {
		return true;
    }
    else  {
		for(i=2; i < num; i++)
			if (num % i == 0)
				count++;
    }
    if (count > 1)
 	    return false;
    else
        return true;
}

unsigned int findNextPrime(unsigned int num) {
	for (unsigned int i=num+1; i<2*num; i++)
		if (isPrime(i))
			return i;
}

unordered_set<string> findUnion(vector<string> documentA, vector<string> documentB) {
	unordered_set<string> auxiliar;
	for (string s : documentA)
		auxiliar.insert(s);
	for (string s : documentB)
		auxiliar.insert(s);
	return auxiliar;
}

unordered_set<string> setUnion(unordered_set<string> set_A, unordered_set<string> set_B) {
	unordered_set<string> auxiliar;
	for (string s: set_A)
		auxiliar.insert(s);
	for (string s: set_B)
		auxiliar.insert(s);
	return auxiliar;
}

void init_coeffs(vector<unsigned int> &coeffs, int rows) {
	iota(coeffs.begin(), coeffs.end(), 1);
	random_shuffle(coeffs.begin(), coeffs.end());
	#if DEBUG
	for (auto i = coeffs.begin(); i != coeffs.end(); ++i)
		std::cout << *i << ' ';
	cout << endl;
	#endif
}

auto computeCharacteristicMatrix(vector<string> document_A, vector<string> document_B) {
	/* Computes the characteristic matrix of two documents.
	 * Notice the matrix is going to be sparse, thus a
	 * representation by the positions in which the 1's
	 * appears is more efficient in space. */
	unordered_set<string> document_A_words(document_A.begin(), document_A.end());
	unordered_set<string> document_B_words(document_B.begin(), document_B.end());
	unordered_set<string> common_words = setUnion(document_A_words, document_B_words);
	unsigned int n_common_words = common_words.size();
	auto result = new int[n_common_words][2];

	int word_n = 0;
	for (string s: common_words) {
		bool A_has_s = document_A_words.find(s) != document_A_words.end();
		bool B_has_s = document_B_words.find(s) != document_B_words.end();
		result[word_n][0] = (int) A_has_s;
		result[word_n][1] = (int) B_has_s;
		word_n++;	
	}

	#if DEBUG
	printf("Characteristic matrix\n");
	for (unsigned int i=0; i<n_common_words; i++) {
		printf("%d\t%d\n", result[i][0], result[i][1]);
	}
	#endif

	return result;
}

double JaccardMinhashApproximation(vector<string> document_A, vector<string> document_B, unsigned int t) {
	vector<unsigned int> coeffs_A(t);
	vector<unsigned int> coeffs_B(t);

	unordered_set<string> doc_union = findUnion(document_A, document_B);
	unsigned int i_doc_union = doc_union.size(); // AKA n_rows, n_words

	init_coeffs(coeffs_A, i_doc_union);
	init_coeffs(coeffs_B, i_doc_union);
	#if DEBUG
	for (auto i = coeffs_A.begin(); i != coeffs_A.end(); ++i)
		std::cout << *i << ' ';
	cout << endl;
	#endif

	unsigned int c = findNextPrime(i_doc_union);

	#if DEBUG
	printf("-----HASH FUNCTIONS-----\n");
	for (unsigned int i=0; i<t; i++) {
		printf("(%dx+%d)%%%d\n", coeffs_A[i], coeffs_B[i], c);
	}
	printf("------------------------\n");
	#endif

	auto characteristic_matrix =  computeCharacteristicMatrix(document_A, document_B);

	auto signature_matrix = new int[t][2];

	// initialize signature_matrix with MAX_INT
	for (unsigned int i=0; i<t; i++){
		signature_matrix[i][0] = INT_MAX;
		signature_matrix[i][1] = INT_MAX;
	}

	#if DEBUG
	printf("-----SIGNATURE MATRIX-----\n");
	for (unsigned int i=0; i<t; i++) {
		printf("%d\t%d\n", signature_matrix[i][0], signature_matrix[i][1]);
	}
	printf("--------------------------\n");
	#endif


	for (unsigned int i=0; i<i_doc_union; i++) {
		unsigned int hash_values[t];
		for (unsigned int j=0; j<t; j++) {
			hash_values[j] = (coeffs_A[j]*i + coeffs_B[j]) % c;
		}
		#if DEBUG
		// cout << "hash_values, iteration " << i << endl;
		// for (unsigned int z=0; z<t; z++) {
		// 	printf("%d ", hash_values[z]);
		// }
		// cout << endl;
		#endif

		for (unsigned int c=0; c<2; c++) {
			if (characteristic_matrix[i][c] == 1) {
				for (unsigned int j=0; j<t; j++) {
					if (signature_matrix[j][c] > hash_values[j])
						signature_matrix[j][c] = hash_values[j];
				}
			} 
		}
	}

	#if DEBUG
	printf("-----SIGNATURE MATRIX-----\n");
	for (unsigned int i=0; i<t; i++) {
		printf("%d\t%d\n", signature_matrix[i][0], signature_matrix[i][1]);
	}
	printf("--------------------------\n");
	#endif

	double similarity = 0.0;
	for (unsigned int i=0; i<t; i++) {
		if (signature_matrix[i][0] == signature_matrix[i][1]) {
			similarity += 1.0/t;
		}
	}

	return similarity;
}

int main(int argc, char* argv[]) {
	srand(time(0));
	if (argc != 4) usage();

	vector<string> starterDocumentA, starterDocumentB;
	read(argv[1], argv[2], starterDocumentA, starterDocumentB);
	unsigned int t = atoi(argv[3]);


	clock_t start = clock();
	double similarity = JaccardMinhashApproximation(starterDocumentA, starterDocumentB, t);
	clock_t end = clock();
	
	calculateTime(start, end);
	printf("Similarity approximation: %f\n", similarity);
	
	return 0;
}
