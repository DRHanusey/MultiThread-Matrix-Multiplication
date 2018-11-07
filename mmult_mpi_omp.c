#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>
#define min(x, y) ((x)<(y)?(x):(y))

double* gen_matrix(int n, int m);
int mmult(double *c, double *a, int aRows, int aCols, double *b, int bRows, int bCols);
void compare_matrix(double *a, double *b, int nRows, int nCols);

// My function declerations
double** importMatrix(FILE *fp, int numRow,int numCol);
void printMatrix(double** matrix,int numCol,int numRow);

/** 
    Program to multiply a matrix times a matrix using both
    mpi to distribute the computation among nodes and omp
    to distribute the computation among threads.
*/

int main(int argc, char* argv[])
{
  int nrows, ncols;
  double *aa;	/* the A matrix */
  double *bb;	/* the B matrix */
  double *cc1;	/* A x B computed using the omp-mpi code you write */
  double *cc2;	/* A x B computed using the conventional algorithm */
  int myid, numprocs, sent;
  double starttime, endtime;
  MPI_Status status;
  /* insert other global variables here */
     char ch;
    FILE *fp1, *fp2, *fpOut;
    int aRow, aCol, bRow, bCol;
    char str1[10], str2[10];
    double matrixEntry;
    int canMult = 0;


  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  if (argc > 1) {
    nrows = atoi(argv[1]);
    ncols = nrows;
    if (myid == 0) {
      // Master Code goes here
      // aa = gen_matrix(nrows, ncols);
      // bb = gen_matrix(ncols, nrows);
      // cc1 = malloc(sizeof(double) * nrows * nrows);

    //Import matrix a dimensions
    if ((fp1 = fopen(argv[ 1 ], "r")) == NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }
    fscanf(fp1, "%s", str1);
    fscanf(fp1, "%s", str1);
    aRow = atoi(str1);
    //printf("str1 = %s, str2 = %s\n", str1, str2);
    printf("aRow = %d\n", aRow);

    fscanf(fp1, "%s", str2);
    fscanf(fp1, "%s", str2);
    fscanf(fp1, "%s", str2);
    //printf("str1 = %s, str2 = %s\n", str1, str2);
    aCol = atoi(str2);
    printf("aCol = %d\n", aCol);
    // gets rid of ")"
    fscanf(fp1, "%s", str2);
      
    //Import matrix b dimensions
    if ((fp2 = fopen(argv[ 2 ], "r")) == NULL) {
        printf("Cannot open file.\n");
        exit(1);
    }
    fscanf(fp2, "%s", str1);
    fscanf(fp2, "%s", str1);
    //printf("str1 = %s, str2 = %s\n", str1, str2);
    bRow = atoi(str1);
    printf("bRow = %d\n", bRow);

    fscanf(fp2, "%s", str2);
    fscanf(fp2, "%s", str2);
    fscanf(fp2, "%s", str2);
    //printf("str1 = %s, str2 = %s\n", str1, str2);
    bCol = atoi(str2);
    printf("bCol = %d\n", bCol);
    // gets rid of ")"
    fscanf(fp2, "%s", str2); 
   
    //Check is matrix a and b can be multiplied
    if (aCol == bRow) {
        canMult = 1;
        printf("These matrices can be multiplied");
    } else {
        printf("These matrices can NOT be multiplied");
        fclose(fp1);
        fclose(fp2);
        return 0;
    }
 
    aa = importMatrix(fp1,aCol,aRow);
    printf("\nMatrix aa: ");
    printMatrix(aa,aCol,aRow);

    bb = importMatrix(fp2,bCol,bRow);
    printf("\nMatrix bb: ");
    printMatrix(bb,bCol,bRow);



      starttime = MPI_Wtime();
      sent = 0;
      
      /* Insert your master code here to store the product into cc1 */
      endtime = MPI_Wtime();
      printf("\n endtime-starteim = %f\n",(endtime - starttime));
      cc2  = malloc(sizeof(double) * nrows * nrows);
      mmult(cc2, aa, nrows, ncols, bb, ncols, nrows);
      compare_matrices(cc2, cc1, nrows, nrows);
    } else {
      // Slave Code goes here
    }
  } else {
    fprintf(stderr, "Usage matrix_times_vector <size>\n");
  }
  MPI_Finalize();
  return 0;
}


double** importMatrix(FILE *fp, int numCol,int numRow) {

        int i, j;
        double** matrix = malloc(numCol * sizeof (double*));
        for (i = 0; i < numCol; ++i)
            matrix[i] = malloc(numRow * sizeof (double));

        for (i = 0; i < numCol; i++) {
            for (j = 0; j < numRow; j++) {
                if (!fscanf(fp, "%lf", &matrix[i][j]))
                    break;
            }
        }
        close(fp);
        return matrix;
}

    
void printMatrix(double** matrix,int numCol,int numRow){
        int i,j;
                for (i = 0; i < numCol; i++) {
            printf("\n");
            for (j = 0; j < numRow; j++) {
                printf("mat[%d][%d] = %lf ", i, j, matrix[i][j]); 
            }
        } 
}







