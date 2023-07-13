import random
import numpy as np

A = np.zeros((4,4))
e = np.ones(4)

for i in range(0,4):
    for j in range(0,i):
        if i != j:
            A[i][j] = random.randint(0,1)
            A[j][i] = A[i][j]

c3 = np.zeros(4)
for i in range(0,4-2):
    for j in range(i+1,4-1):
        for k in range(j+1,4):
            if A[i][j] == 1 and A[j][k] == 1 and A[k][i] == 1:
                c3[i] = c3[i] + 1
                c3[j] = c3[j] + 1
                c3[k] = c3[k] + 1

c3_opt = np.matmul(np.multiply(A,np.matmul(A,A)),e)/2
