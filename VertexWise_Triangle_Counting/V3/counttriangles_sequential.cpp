#include <stdio.h>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>


using namespace std;
using namespace std::chrono;

void open_matrix_market(string file, int &n, int &nzero) {

	int M,N;
	std::ifstream reader(file);
	std::string line;
	std::regex pattern(R"(\b(\d+)\s+(\d+)\s+(\d+)\b)");
	while (std::getline(reader,line)) {
		std::smatch matches;
		//if it is a comment, pass
		if (line[0] == '%') {
			//cout << line << endl;
		} else {
			//the regular expression must spot the %d %d %d pattern for number of rows, number of columns
			//and number of not zero values
			if (std::regex_search(line,matches,pattern)) {
				M = std::stoi(matches[1]);
				N = std::stoi(matches[2]);
				nzero = std::stoi(matches[3]);
			}
			break;
		}
	}

	if (M == N) {
		n = N;
	} else {
		cout << "The M and N must be the same size!" << endl;
		exit(0);
	}
	reader.close();


}


void read_matrix_market (string file, int &n, int &nzero, uint32_t *rows, uint32_t *cols, uint32_t *csccols) {
	uint32_t M, N,*indeces;
	int i,length = 1;
	std::ifstream reader(file);
	std::string line;
	std::regex pattern(R"(\b(\d+)\s+(\d+)\s+(\d+)\b)");
	//read and ignore all the comments
	while (std::getline(reader,line)) {
		std::smatch matches;
		//if it is a comment, pass
		if (line[0] == '%') {
//			cout << line << endl;
		} else {
			//the regular expression must spot the %d %d %d pattern for number of rows, number of columns
			//and number of not zero values
			if (std::regex_search(line,matches,pattern)) {
				M =(uint32_t) std::stoi(matches[1]);
				N = (uint32_t) std::stoi(matches[2]);
				nzero = std::stoi(matches[3]);
			}
			break;
		}
	}
	//the matrix must be square

	if (M == N) {
		n = N;
	} else {
		cout << "The M and N must be the same size!" << endl;
		exit(0);
	}



	for (i=1;i <= 2*nzero;i++) {
		reader >> M >> N;
		rows[i-1] = M;
		cols[i-1] = N;
		++i;
		rows[i-1] = N;
		cols[i-1] = M;
	}

	for(int a=0;a<2*nzero;a++) {
		csccols[cols[a] - 1]++;
	}

	int summ = 0;
	for (int k = 0; k < n; k++) {
        	uint32_t t = csccols[k];
        	csccols[k] = summ;
        	summ += t;
    	}
	csccols[n] = 2*nzero;
	reader.close();

}

void sort_cscrows(uint32_t *cols,int nzero, uint32_t *rows,uint32_t *csccols, int n,uint32_t *cscrows) {
	//create a copy of csccols
	uint32_t j;
	uint32_t *csc = new uint32_t[n+1];
	for(int i=0;i<n+1;i++) {
		csc[i] = csccols[i];
	}
	for(int i=0;i<nzero;i++) {
		j = csc[cols[i]-1];
		cscrows[j] = rows[i] - 1;
		++csc[cols[i]-1];
	}
}

int count_triangles(uint32_t *cscrows,uint32_t *csccols,int n, int nzero) {

	int num_triangles = 0;
	uint32_t i,j,k,a,b,c;
	for (i=0;i < n; i++) {
		for(c=csccols[i]; c < csccols[i+1];c++) {
			j = cscrows[c];
			for (b=csccols[j];b < csccols[j+1];b++) {
				k = cscrows[b];
				for (a = csccols[k]; a < csccols[k+1]; a++) {
					if (cscrows[a] == i) {
						num_triangles++;
					}
				}
			}
		}
	}

	return num_triangles/6;
}

int main(int argc, char **argv) {

	if (argc < 2) {
		cout << "Please give the input .mtx file." << endl;
		return 1;
	}
	int a,b,c,n, nzero,triangles;
	string file = argv[1];
	open_matrix_market(file,n,nzero);
	uint32_t i ,j ,k;
	uint32_t *rows = new uint32_t[2*nzero];
	uint32_t *cols = new uint32_t[2*nzero];
	uint32_t *csccols = new uint32_t[n+1];
	uint32_t *cscrows = new uint32_t[2*nzero];
	for(a=0;a<n+1;a++) {
		csccols[a] = 0;
	}
	read_matrix_market(file,n,nzero,rows,cols,csccols);
	sort_cscrows(cols,2*nzero,rows,csccols,n,cscrows);
	auto start = high_resolution_clock::now();
	triangles = count_triangles(cscrows,csccols,n,2*nzero);
	auto end = high_resolution_clock::now();
	cout << triangles << endl;
	double execution_time = duration_cast<nanoseconds>(end - start).count();
	execution_time *= 1e-9;
	cout << "Time taken for execution in seconds: " << execution_time;
	cout << " secs" << endl;


	return 0;
}
