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
			cout << line << endl;
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


uint32_t *read_matrix_market (string file, int &n, int &nzero, uint32_t *rows, uint32_t *cols) {
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
			cout << line << endl;
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
	cout << M << " " << N << " " << nzero << endl;
	//the matrix must be square

	if (M == N) {
		n = N;
	} else {
		cout << "The M and N must be the same size!" << endl;
		exit(0);
	}

	cout << "The number x is: " << M << endl;
	cout << "the number y is: " << N << endl;
	cout << "The not zero values are: " << nzero << endl;
	//reads the matrix market in CSC and COO format
	//CSC format: rows, cols, indeces
	indeces = (uint32_t *)malloc(1*sizeof(uint32_t));
//	rows = new uint32_t[nzero];
//	cols = new uint32_t[nzero];

	for (i=1;i <= nzero;i++) {
		reader >> M >> N;
		rows[i-1] = M;
		cols[i-1] = N;
		if (i == 1) {
			indeces[0] = N;
			continue;
		}
		if (cols[i-2] != N) {
			length = length + 1;
			indeces = (uint32_t *)realloc(indeces,length*sizeof(uint32_t));
			indeces[length-1] = i;
		}
	}

	for (int h=0;h<nzero;h++) {
		cout << rows[h] <<" " << cols[h] << endl;


	}
	reader.close();
	return indeces;
}

int main(int argc, char **argv) {

	if (argc < 2) {
		cout << "Please give the input .mtx file." << endl;
		return 1;
	}
	int a,b,c,n, nzero;
	string file = argv[1];
	open_matrix_market(file,n,nzero);
	uint32_t *indeces=NULL ,i ,j ,k;
	uint32_t *rows = new uint32_t[nzero];
	uint32_t *cols = new uint32_t[nzero];
	indeces = read_matrix_market(file,n,nzero,rows,cols);
	int c3[n];
	for (a = 0;a < n ; a++) {
		c3[a] = 0;
	}
	cout << nzero << endl;
	auto start = high_resolution_clock::now();
	for (a=0;a < nzero;a++) {
		i = rows[a];
		j = cols[a];
		for (b=a+1; b<nzero ;b++) {
			if (i == cols[b]) {
				k = rows[b];
				for(c= 0;c < nzero;c++) {
					if (k == rows[c] && j == cols[c]) {
						c3[i-1] = c3[i-1] + 1;
						c3[j-1] = c3[j-1] + 1;
						c3[k-1] = c3[k-1] + 1;
						continue;
					}
					if (k == cols[c] && j == rows[c]) {
						c3[i-1] = c3[i-1] + 1;
						c3[j-1] = c3[j-1] + 1;
						c3[k-1] = c3[k-1] + 1;
					}
				}
			}
		}
	}
	auto end = high_resolution_clock::now();
	for (a=0;a<n;a++) {
		cout << "The " << a+1 << " node has: " << c3[a] << " triangles." << endl;
	}

	double execution_time = duration_cast<nanoseconds>(end - start).count();
	execution_time *= 1e-9;
	cout << "Time taken for execution in seconds: " << execution_time;
	cout << " secs" << endl;


	return 0;
}


