/**********************************************************************************/
/* This is the first version of graph generation. We compute the total number of  */
/* graphs possible (say n) with a given number of vertices and compute all binary */ 
/* numbers from 0 to n. We use this numbers to generate the adjacency matrix of	  */
/* the graph. We check the connectedness of each graph and the connected ones are */
/* written to a file names Graph_i.csv, where "i" is the number of vertices.	  */
/**********************************************************************************/
#include<stdio.h>
#define MAX 8 
/*MAX = Number of vertices */
void generate_Graph(int n);
int Adj[MAX][MAX], reach[MAX];
FILE *fp;
void dfs(int v)
{
	int i;
	reach[v]=1;
	for(i=0;i<MAX;i++)
		if(Adj[v][i] && !reach[i]) {
			dfs(i);
		}
}


void main()
{

	long int n;
	/* The csv file for storing all graphs with "i" vertices is Graph_i.csv */
	if((fp=fopen("Graph_8.csv", "a+"))==NULL) {
		printf("Cannot open file.\n");
	}
	/* "n" is the maximum number of graphs possible with "i" vertices, i.e. 2^(i c 2)*/
	for(n=268435456; n>0; n--) {
		generate_Graph(n);
	}
	fclose(fp);

}

void generate_Graph(int n)	/* Function to convert decimal to binary.*/
{
	int i=0, j=1, row=0, col=0, count=0;
	int binary[100];
	// can use memset - very fast
	for(row=0; row<MAX; row++) {
		for(col=0; col<MAX; col++) {
			Adj[row][col]=0;
			reach[row]=0;
		}
	}

	// convert n to binary number and store it in binary
	while (n!=0) {
		binary[j++]=n%2;
		n/=2;
	}

	// filling up the lower triangular matrix
	for(row=0; row<MAX; row++) {
		for(col=0; col<MAX; col++) {
			if(row<col) {
				Adj[row][col]=binary[j-1];
				Adj[col][row]=binary[j-1];
				if(j>0)
					j=j-1;
			}
		}
	}
	
	for(row=0; row<MAX; row++) {
		printf("\n");
		for(col=0; col<MAX; col++)
			printf("  %d\t",Adj[row][col]);
	}

	dfs(0); /* Checking connectedness of graph*/
	for(i=0;i<MAX;i++)
		if(reach[i])
			count++;
	if(count==MAX) {
		for(row=0; row<MAX; row++)
			for(col=0; col<MAX; col++)
				fprintf(fp,"%d ", Adj[row][col]);
		printf("\nGraph is connected.");
		fprintf(fp,"\n");

	}
	else
		printf("\nGraph is disconnected.");
}
