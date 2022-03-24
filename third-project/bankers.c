//Victor Cox Bankers/safety algorithim
// Banker's Algorithm

#include <stdio.h>


void get_input() {
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
	printf("avail[i]:%d\n",avail[1]);

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



}

int main()
{
	// get input
	get_input();
	int n, m, i, j, k;
	n = 5; // Number of processes
	m = 3; // Number of resources
	int alloc[5][3] = { { 0, 1, 0 }, // P0 // Allocation Matrix
						{ 2, 0, 0 }, // P1
						{ 3, 0, 2 }, // P2
						{ 2, 1, 1 }, // P3
						{ 0, 0, 2 } }; // P4

	int max[5][3] = { { 7, 5, 3 }, // P0 // MAX Matrix
					{ 3, 2, 2 }, // P1
					{ 9, 0, 2 }, // P2
					{ 2, 2, 2 }, // P3
					{ 4, 3, 3 } }; // P4

	int avail[3] = { 3, 3, 2 }; // Available Resources

	int f[n], ans[n], ind = 0;
	for (k = 0; k < n; k++) {
		f[k] = 0;
	}
	int need[n][m];
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++)
			need[i][j] = max[i][j] - alloc[i][j];
	}
	int y = 0;
	for (k = 0; k < 5; k++) {
		for (i = 0; i < n; i++) {
			if (f[i] == 0) {

				int flag = 0;
				for (j = 0; j < m; j++) {
					if (need[i][j] > avail[j]){
						flag = 1;
						break;
					}
				}

				if (flag == 0) {
					ans[ind++] = i;
					for (y = 0; y < m; y++)
						avail[y] += alloc[i][y];
					f[i] = 1;
				}
			}
		}
	}

	int flag = 1;
	
	for(int i=0;i<n;i++) {
		if(f[i]==0) {
			flag=0;
			printf("The following system is not safe");
			break;
		}	
	}
	
	if(flag==1) {
		printf("Following is the SAFE Sequence\n");
		for (i = 0; i < n - 1; i++) {
			printf(" P%d ->", ans[i]);
		}
		printf(" P%d", ans[n - 1]);
	}
	printf("\n");
	

	return (0);

	// This code is contributed by Deep Baldha (CandyZack)
}

