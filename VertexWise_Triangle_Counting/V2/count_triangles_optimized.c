#include <stdio.h>
#include <time.h>
//Optimized version of counting triangles in undirected, unweighted graphs
//This code enumerating and checking only i<j<k triplets
int main() {


	int A[5][5] = {  {0,0,1,1,1},{0,0,0,0,1},
			{1,0,0,0,1}, {1,0,0,0,0},
			{1,1,1,0,0}
	};
	int B[5][5] = { {0,1,1,1,1}, {1,0,1,0,1},
			{1,1,0,1,1},{1,0,1,0,0},
			{1,1,1,0,0}
	};
	int i,j,k;
	int c3[5] = {0,0,0,0,0};
	clock_t start, end;
	double execution_time;
	long int length = sizeof(B[0])/sizeof(B[0][0]);
	start = clock();
	//for every i<j<k 
	for (i=0;i<length-2;i++) {
		for (j=i+1;j<length-1;j++) {
			for (k=j+1;k<length;k++) {
				if (B[i][j] == B[i][k] && B[i][j] == B[j][k] && B[i][j] == 1) {
					c3[i] = c3[i] + 1;
					c3[j] = c3[j] + 1;
					c3[k] = c3[k] + 1;
				}

			}
		}
	}

	for (i=0;i<=4;i++) {
		printf("%d, ",c3[i]);

	}
	printf("\n");
	end = clock();
	execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
	printf("Time taken for execution in seconds: %f\n",execution_time);

	return 0;

}

