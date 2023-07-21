#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct vptree {

  //vantage point
  double *vp;
  double md;
  int idx;
  struct vptree *inner;
  struct vptree *outer;
} vptree;

//QUICKSELECT FROM WEBSITE geeksforgeeks
void swap(double *a, double *b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}

int partition(double *arr, int low, int high) {
  double pivot = arr[high];
  int i = low - 1;
  for (int j = low; j < high; j++) {
    if (arr[j] <= pivot) {
      i++;
      swap(&arr[i], &arr[j]);
    }
  }
  swap(&arr[i + 1], &arr[high]);
  return i + 1;
}

double quickselect(double *arr, int low, int high, int k) {
  if (low == high) {
    return arr[low];
  }

  int pivot = partition(arr, low, high);
  if (pivot == k) {
    return arr[pivot];
  } else if (pivot < k) {
    return quickselect(arr, pivot + 1, high, k);
  } else {
    return quickselect(arr, low, pivot - 1, k);
  }
}

//computes the euclidean distance
void euclideanDistance(double **X, int n, int m, double *eucldistance) {

    for (int i = 0; i < n - 1; i++) {
        double sumOfSquares = 0.0;
        for (int j = 0; j < m; j++) {
            double diff = X[i][j] - X[n - 1][j];
            sumOfSquares += diff * diff;
        }
       eucldistance[i] += sqrt(sumOfSquares);
//       printf("To X[i] einai: %lf %lf kai i apostasi einai: %lf\n",X[i][0],X[i][1],eucldistance[i]);
    }
}
vptree * getInner(vptree * T){
  return (T->inner);
}


vptree * getOuter(vptree * T){
  return (T->outer);
}


double getMD(vptree * T){
  return (T->md);
}


double * getVP(vptree * T){
  return (T->vp);

}


int getIDX(vptree * T){
  return (T->idx);

}
vptree * vantagePointTree(double **X,int n, int d,int *indexes) {

	int idx1=0,idx2=0,*indexesIn,*indexesOut;
	vptree *T = NULL;
	if (n == 0) {          //if n=0 then the leaf of tree is empty
		return T;
	}
	T = (vptree*) malloc(sizeof(vptree));
	T->vp = (double*)malloc(d * sizeof(double));
	for (int i=0;i < d;i++) {                    //vantage point of the tree is the last
		T->vp[i] = X[n-1][i];
	}
	T->idx = indexes[n - 1];
//	printf("The vantage point is: %lf %lf\n",T->vp[0],T->vp[1]);
//	printf("The index of vantage point tree is: %d\n",T->idx);
	if (n == 1) {
		T->inner = NULL;
		T->outer = NULL;
		return T;
	}
	double *eucldistance = (double *)malloc((n-1)*sizeof(double));
	double *eucldistance2 = (double *)malloc((n-1)*sizeof(double));
	for(int i=0;i<n-1;i++) {
		eucldistance[i] = 0;
		eucldistance2[i] = 0;
	}
	euclideanDistance(X,n,d,eucldistance); //computes the euclidean distance
//	printf("----------------------Euclidean Distance------------------\n");
	for (int i=0;i<n-1;i++) {
		eucldistance2[i] = eucldistance[i];
	}
	T->md = quickselect(eucldistance2,0,n-2,(n/2)-1);
	double **in, **out;
	int nin = n/2;
	int nout = n - 1 - (n/2);
	//allocates memory for inner and outer
	in = (double **) malloc(nin*sizeof(double*));
	out = (double **) malloc(nout*sizeof(double*));
	indexesIn = (int *) malloc(nin*sizeof(int));
	indexesOut = (int *) malloc(nout*sizeof(int));
	for(int i = 0;i < nin; i++) {
		in[i] = (double*) malloc(d * sizeof(double));
		if((i == nin-1) && (n % 2 == 0)) {
			continue;
		} else {
			out[i] = (double*) malloc(d*sizeof(double));
		}
	}
//	printf("The median point of vptree is: %lf\n",T->md);
	//seperates inner and outer
	for (int i = 0;i < n-1; i++) {
		if (eucldistance[i] <= (T->md)) {
//			printf("H eukl apostasi einai: %lf kai to median einai: %lf\n",eucldistance[i],T->md);
			for (int j=0;j < d;j++) {
				in[idx1][j] = X[i][j];
				indexesIn[idx1] = i;
//				printf("Sto inner einai: %lf\n",in[idx1][j]);
			}
			idx1++;
		} else {
			for (int j=0;j < d;j++) {
				out[idx2][j] = X[i][j];
				indexesOut[idx2] = i;
//				printf("Sto outer einai: %lf\n",out[idx2][j]);
			}
			idx2++;
		}
	}
	free(eucldistance);
	free(eucldistance2);
	T->inner = vantagePointTree(in,nin,d,indexesIn);
	T->outer = vantagePointTree(out,nout,d,indexesOut);
	for (int i=0;i<nin;i++) {
		free(in[i]);
	}
	for (int i=0;i<nout;i++) {
		free(out[i]);
	}
	free(in);
	free(out);
	free(indexesIn);
	free(indexesOut);
	return T;
}

vptree * buildvp(double **X, int n, int d) {
	vptree *T = NULL;
	if (n == 0) {          //if n=0 then the leaf of tree is empty
		return T;
	}
	int *indexes;
	indexes = (int*) malloc(n*sizeof(int));
	for (int i=0;i < n;i++) {
		indexes[i] = i;
	}
	T = vantagePointTree(X,n,d,indexes);
	free(indexes);
	return T;
}

int main() {

	double **X;
	int n = 100000,i,j;
	int d = 2;
	clock_t start, end;
	X = (double**)malloc(n *sizeof(double*));
	for (i=0;i < n;i++) {
		X[i] = (double*)malloc(d * sizeof(double));
	}
	srand((unsigned int) time(NULL));
	for(i = 0;i < n;i++) {
		for(j = 0;j < d;j++) {
			X[i][j] = ((double) rand() / (double)(RAND_MAX));
//			printf("%lf\n",X[i][j]);
		}
	}
	vptree * T = (vptree*)malloc(sizeof(vptree));
	start = clock();
	T = buildvp(X,n,d);
	end = clock();
	double *vantagePoint = (double *) malloc(d*sizeof(double));
	vantagePoint = getVP(T);
	printf("The vantage point is: %lf %lf\n",vantagePoint[0],vantagePoint[1]);
	printf("The index of vantage point is: %d\n",getIDX(T));
	printf("The median of the vantage point tree is: %lf\n",getMD(T));
	vptree *inner = (vptree*)malloc(sizeof(vptree));
	inner = getInner(T);
	vantagePoint = getVP(inner);
	printf("The vantage point of inner tree is: %lf %lf and its index is: %d\n",vantagePoint[0],vantagePoint[1],getIDX(inner));
 	printf("Time taken to execute in seconds : %lf\n", ((double)(end - start))/CLOCKS_PER_SEC);
	free(X);
	free(T);
	free(vantagePoint);
	free(inner);
	return 0;

}
