#include <stdio.h>
#include <time.h>

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

	start = clock();
	for (i=0;i<=4;i++) {
		for (j=0;j<=4;j++) {
			for (k=0;k<=4;k++) {
				if (B[i][j] == B[i][k] && B[i][j] == B[j][k] && B[i][j] == 1) {
					c3[i] = c3[i] + 1;
					c3[j] = c3[j] + 1;
					c3[k] = c3[k] + 1;
				}

			}
		}
	}
	for (i=0;i<=4;i++) {
		c3[i] = c3[i]/6;
		printf("%d, ",c3[i]);

	}
	printf("\n");
	end = clock();
	execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
	printf("Time taken for execution in seconds: %f\n",execution_time);
	return 0;

}
