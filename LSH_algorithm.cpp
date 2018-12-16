#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <algorithm>

#include "Headers/inputOutput.h"

using namespace std;


void usage() {
	cout << "Usage:" << endl;
	cout << "	arg0 -> prog" << endl;
	cout << "	arg1 -> k size of shingles" << endl;
	cout << "	arg2 -> t hash functions" << endl;
	cout << "	arg3 -> hash table size n" << endl;
    cout << "	arg4 -> b bands in LSH" << endl;
	cout << "	arg5 -> path document 1" << endl;
	cout << "	arg6 -> path document 2" << endl;
	cout << "	argn+7 -> path document n" << endl;
	exit(1);
}

//Funcio per utilitzar unordered_set<pair<int,int> >
namespace std {
template <> struct hash<std::pair<int, int>> {
    inline size_t operator()(const std::pair<int, int> &v) const {
        std::hash<int> int_hasher;
        return int_hasher(v.first) ^ int_hasher(v.second);
    }
};
}

//Funcio pel sort
bool comp(string i,string j) { return (i<j); }

//Descompon el starterDocument en k-shingles i posa el resultat a document
void kShingles(vector<string>& starterDocument, vector<string>& document, unsigned int k) {
	if (k > starterDocument.size()) error("La k no pot ser major que el nombre de paraules del document");
    for (unsigned int i=0; i < starterDocument.size() - k + 1; ++i) {
    	string s;
    	for (unsigned int j=i; j < i + k; ++j) {
    		if (i == j) s = starterDocument[j];
    		else s += " " + starterDocument[j];
		}
		document.push_back(s);
	}
}

//Troba el total de k-shingles diferents en tots els documents
unordered_set<string> setUnion(vector<unordered_set<string> > documentA) {
	unordered_set<string> auxiliar;
	for(unsigned int i = 0; i < documentA.size(); ++i) {
		for (string s : documentA[i])
			auxiliar.insert(s);
	}
	return auxiliar;
}

//Inicialitzem les funcions de hash
void init_coeffs(vector<unsigned int> &coeffs, int rows) {
	for (unsigned int i = 0; i < coeffs.size(); ++i) {
		int a = rand()%rows;
		coeffs[i] = a;
	}
}

//Trobem la characteristic matrix
//A documents hi ha el conjunt de K-shingles de tots els docs i a common_words hi haura el total de k-shingles diferents entre tots els docs
vector<vector<int> > computeCharacteristicMatrix(vector<vector<string> > documents, unordered_set<string>& common_words) {
	vector<unordered_set<string> > document_A;
	for(unsigned int i = 0; i < documents.size(); ++i) {
		unordered_set<string> document_B(documents[i].begin(), documents[i].end());
		document_A.push_back(document_B);
	}
	common_words = setUnion(document_A);
	unsigned int n_common_words = common_words.size();
	int n_docs = documents.size();
	vector<vector<int> > result(n_common_words,vector<int>(n_docs));

	int word_n = 0;
	for (string s: common_words) {
		for(int i = 0; i < n_docs; ++i) {
			bool A_has_s = document_A[i].find(s) != document_A[i].end();
			result[word_n][i] = (int) A_has_s;
		}
		word_n++;	
	}
	return result;
}

//Construim la Signature Matrix
vector<vector<int> > ConstructSignature(vector<vector<string> > documents, unsigned int t) {
	vector<unsigned int> coeffs_A(t);										//Estructures per guardar les funcions de hash
	vector<unsigned int> coeffs_B(t);
	unordered_set<string> doc_union;										//hi haura el total de k-shingles diferents entre tots els docs

	vector<vector<int> > characteristic_matrix =  computeCharacteristicMatrix(documents,doc_union);
	
	unsigned int i_doc_union = doc_union.size();
	int n_docs = documents.size();
	init_coeffs(coeffs_A, i_doc_union+1);									//Inicialitzem funcions hash
	init_coeffs(coeffs_B, i_doc_union+1);

	vector<vector<int> > signature_matrix(t,vector<int>(n_docs));
	
	for (unsigned int i=0; i<t; i++){										//Inicialitzem la signature matrix amb un valor elevat
		for(int j = 0; j < n_docs; ++j) {
			signature_matrix[i][j] = 10000;
		}
	}
	
	for (unsigned int i=0; i<i_doc_union; i++) {							//Per cada fila de la characteristic matrix
		int hash_values[t];
		for (unsigned int j=0; j<t; j++) {									//li donem un valor de hashing per les t funcions de hash i d aquesta manera simulem les permutacions de files
			hash_values[j] = (coeffs_A[j]*(i+1) + coeffs_B[j])%(i_doc_union+1);
		}

		for (int c=0; c<n_docs; c++) {										//Apliquem el minhash
			if (characteristic_matrix[i][c] > 0) {
				for (unsigned int j=0; j<t; j++) {
					if (signature_matrix[j][c] > hash_values[j]) signature_matrix[j][c] = hash_values[j];
				}
			} 
		}
	}
	return signature_matrix;
}

//A partir de la signature matrix busquem els parells candidats aplicant el LSH i calculem les seves similituds
vector<vector<double> > finding_candidates(vector<vector<int> > signature, unordered_set<pair<int,int> >& candidates, int t, int b) {
	
	int n_docs = signature[0].size();
	vector<vector<double> >  similarity(n_docs, vector<double>(n_docs,0.0));//Inicialitzem una taula de similituds a 0
	double r = t/b;
	for(int i = 0; i < b; ++i) {											//Iterem per bandes
		for(int j = 0; j+1 < n_docs; ++j) {									//Iterem per columnes
			for(int k = j+1; k < n_docs ; ++k) {							//Iterem sobre les columnes de la dreta de la columna j
				bool dif = false;
				int m = signature.size()/b;
				for(int l = 0; l < m and not dif; ++l) {					//Iterem per les files de la banda
					if(signature[l+(r*i)][j] != signature[l+(r*i)][k]) {	//Si alguna fila de la banda es diferent parem 
						dif = true;
					}
				}
				if(not dif) {												//Si hem trobat parell candidat
					pair<int,int> aux = {j,k};
					candidates.insert(aux);									
					similarity[j][k]+= r/t;									//Sumem la similitud a la taula
				}
			}
		}
	}
	return similarity;			
}

int main(int argc, char* argv[]) {
    
	if (argc < 7) usage();
     
    unsigned int k = atoi(argv[1]); 
    unsigned int t = atoi(argv[2]);
    unsigned int b = atoi(argv[3]);
    unsigned int n = atoi(argv[4]);
    if(b > t or (t%b != 0)) {
		cout << "b ha de ser més petita que t i ha de ser divisora de t" << endl;
		exit(1);
	}
    vector<vector<string> > starterDocuments(n);
    for(unsigned int i = 0; i < n; ++i) {
		starterDocuments[i] = read1doc(argv[5+i]);
	}
    vector<vector<string> > Documents(n);
    for(unsigned int i = 0; i < n; ++i) {
		kShingles(starterDocuments[i], Documents[i], k);
		sort(Documents[i].begin(),Documents[i].end(),comp);
	}

	vector<vector<int> > SignatureMatrix = ConstructSignature(Documents, t);
	
	unordered_set<pair<int,int> > candidates;
	vector<vector<double> >  similarity = finding_candidates(SignatureMatrix,candidates,t,b);
	
	if(!candidates.empty()) {
		for (auto it=candidates.begin(); it != candidates.end(); ++it)  {
			int doc1 = (*it).first;
			int doc2 = (*it).second;
			cout << "Simulitud entre el document " << doc1+1 << " i el " << doc2+1 << " es de: " << similarity[doc1][doc2] << endl;
		}
	}
	else cout << "No hi ha cap parell de documents candidats a ser similars" << endl;
}

