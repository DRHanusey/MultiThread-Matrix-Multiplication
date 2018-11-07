#include<stdlib.h>
#include<math.h>
#include<stdio.h>
#include<time.h>
int main(int argc, char* argv[])
{
if(argc>3)
{
	srand(time(NULL));
	int numRow=atoi(argv[1]);
	int numCol=atoi(argv[2]);
	FILE * fp;
	fp=fopen(argv[3],"w");
	fprintf(fp,"rows( %d ) cols( %d )\n",numRow,numCol);
	int i,j;
	for(i=0;i<numRow;i++)
	{	
		for(j=0;j<numCol;j++)
		{		
			fprintf(fp,"%f ",(double)rand()/RAND_MAX);
		}
	fprintf(fp,"\n");
	}
	fclose(fp);
}		
return 0;
}

