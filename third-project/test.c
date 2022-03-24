//Victor Cox Bankers/safety algorithim
// Banker's Algorithm

#include <stdio.h>

int main() {
	int r;
	printf("Enter the number of resources: ");
	scanf("%d", &r);

	int mx[r];
	printf("\nEnter the max instances of each resource:\n");
	for (int i = 0; i< r; i++) {
		printf("R%d = ",i+1);
		scanf("%d",&mx[i]);
	}
	int p;
	printf("Enter the number of processes: ");
	scanf("%d", &p);
	printf("\n");

	int alloc[p][r];
	printf("Enter the allocation matrix:\n\t");
	for (int i = 0; i<r;i++) {
		printf("R%d ",i+1);
	}
	printf("\n");
	for (int i= 0; i< p; i++) {
		printf("P[%d]\t",i);
		for (int j = 0; j < r; j++) {
			scanf("%d", &alloc[i][j]);
		}
	}

	int max_m[p][r];
	printf("\nEnter the Max matrix:\n\t");
	for (int i =0; i< r; i++) {
		printf("R%d ", i+1);
	}
	printf("\n");
	for (int i = 0; i < p;i++) {
		printf("P[%d]\t", i);
		for (int j = 0; j< r; j++) {
			scanf("%d",&max_m[i][j]);
		}
	}
	int avail[r];
	for (int i =0; i < r;i++) {
		int used = 0;
		for (int j=0;j<p;j++) {
			used += alloc[j][i];
		}
		avail[i] = mx[i]-used;
	}

	// decalre array of functions answer
	int f[p], ans[p], ind = 0;
	for (int k = 0; k < p; k++) {
		f[k] = 0;
	}

	//create need matrix
	int need[p][r];
	for (int i = 0; i < p; i++) {
		for (int j = 0; j < r; j++)
			need[i][j] = max_m[i][j] - alloc[i][j];

	}

	// algorithim
	for (int k = 0; k < 5; k++) {
		for (int i = 0; i < p; i++) {
			if (f[i] == 0) {

				int flag = 0;
				for (int j = 0; j < r; j++) {
					if (need[i][j] > avail[j]){
						flag = 1;
						break;
					}
				}

				if (flag == 0) {
					ans[ind++] = i;
					for (int y = 0; y < r; y++)
						avail[y] += alloc[i][y];
					f[i] = 1;
				}
			}
		}
	}
	
	int flag = 1;
	
	for(int i=0;i<p;i++) {
		if(f[i]==0) {
			flag=0;
			printf("The following system is not safe");
			break;
		}	
	}
	
	if(flag==1) {
		printf("Following is the SAFE Sequence\n");
		for (int i = 0; i < p - 1; i++) {
			printf(" P%d ->", ans[i]);
		}
		printf(" P%d", ans[p - 1]);
	}
	printf("\n");

	return 0;

}


