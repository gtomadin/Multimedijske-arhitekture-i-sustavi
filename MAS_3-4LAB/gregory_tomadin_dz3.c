#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES // for C
#include <math.h>
#include <time.h>
#define SIZE 512
#define BLOCKSIZE 8
#define SHIFT 128.0

int R[SIZE][SIZE] = {0};
int G[SIZE][SIZE] = {0};
int B[SIZE][SIZE] = {0};

float Y[SIZE][SIZE] = {0};
float Cb[SIZE][SIZE] = {0};
float Cr[SIZE][SIZE] = {0};

/* matrixes that contains the Y, Cb and Cb values of the given block*/
float YofBlock[BLOCKSIZE][BLOCKSIZE] = {0};
float CBofBlock[BLOCKSIZE][BLOCKSIZE] = {0};
float CRofBlock[BLOCKSIZE][BLOCKSIZE] = {0};

/* matrixes that contains the Y, Cb and Cb values of the given block after the DCT*/
float DCT_Y[BLOCKSIZE][BLOCKSIZE] = {0};
float DCT_CB[BLOCKSIZE][BLOCKSIZE] = {0};
float DCT_CR[BLOCKSIZE][BLOCKSIZE] = {0};

/* quantization table K1*/
int K1[BLOCKSIZE][BLOCKSIZE] = { { 16, 11, 10, 16, 24, 40, 51, 61 },
			{ 12, 12, 14, 19, 26, 58, 60, 55 },
			{ 14, 13, 16, 24, 40, 57, 69, 56 },
			{ 14, 17, 22, 29, 51, 87, 80, 62 },
			{ 18, 22, 37, 56, 68, 109, 103, 77 },
			{ 24, 36, 55, 64, 81, 104, 113, 92 },
			{ 49, 64, 78, 87, 103, 121, 120, 101 },
			{ 72, 92, 95, 98, 112, 100, 103, 99 } };

/* quantization table K2*/
int K2[BLOCKSIZE][BLOCKSIZE] = { { 17, 18, 24, 47, 99, 99, 99, 99 },
			{ 18, 21, 26, 66, 99, 99, 99, 99 },
			{ 24, 26, 56, 99, 99, 99, 99, 99 },
			{ 47, 66, 99, 99, 99, 99, 99, 99 },
			{ 99, 99, 99, 99, 99, 99, 99, 99 },
			{ 99, 99, 99, 99, 99, 99, 99, 99 },
			{ 99, 99, 99, 99, 99, 99, 99, 99 },
			{ 99, 99, 99, 99, 99, 99, 99, 99 } };


/* matrixes that contains the Y, Cb and Cb values of the given block after the quantization*/
float K_Y[BLOCKSIZE][BLOCKSIZE] = {0};
float K_CB[BLOCKSIZE][BLOCKSIZE] = {0};
float K_CR[BLOCKSIZE][BLOCKSIZE] = {0};

int IK_Y[BLOCKSIZE][BLOCKSIZE] = {0};
int IK_CB[BLOCKSIZE][BLOCKSIZE] = {0};
int IK_CR[BLOCKSIZE][BLOCKSIZE] = {0};
/* function that returns the number of rows*/
int getNumberOfRows(FILE *file){
	int c;
	while(1){
		c = getc(file);
		if( c >=  '0' && c <= '9'){
			int x = 0;
			while( c >=  '0' && c <= '9'){
				x = 10 * x + c - '0';
				c = getc(file);
			}
			return x;
		}
	}
}
/* function that returns the number of columns*/
int getNumberOfColumns(FILE *file){
	int c;
	while(1){
		c = getc(file);
		if( c >=  '0' && c <= '9'){
			int x = 0;
			while( c >=  '0' && c <= '9'){
				x = 10 * x + c - '0';
				c = getc(file);
			}
			return x;
		}
	}
}

/* function that returns the maximum value*/
int getMxaxValue(FILE *file){
	int c;
	while(1){
		c = getc(file);
		if( c >=  '0' && c <= '9'){
			int x = 0;
			while( c >=  '0' && c <= '9'){
				x = 10 * x + c - '0';
				c = getc(file);
			}
			return x;
		}
	}
}

/* function that print an integer array*/
void printIntArray(int arr[], int size){
	int i;
	for(i = 0; i < size; ++i){
		printf("%d: %d\n", i, arr[i]);
	}
}

/* function that print an float array*/
void printFloatArray(float arr[], int size){
	int i;
	for(i = 0; i < size; ++i){
		printf("%d: %f\n", i, arr[i]);
	}
}


/* function that returns the number of blocks*/
int getNumberOfBlocks(int size){
	return size/BLOCKSIZE;
}

/* function that print an integer matrix*/
void printIntMatrix(int mat[], int size){
	int i, j;
	for(i = 0; i < size; ++i){
		for(j = 0; j < size; ++j){
			printf("%d ", mat[i * size + j]);
		}
		printf("\n");
	}
}



/* function that write an integer matrix in the selected file*/
void writeIntMatrix(int mat[], int size, FILE *file){
	int i, j;
	for(i = 0; i < size; ++i){
		for(j = 0; j < size; ++j){
			fprintf(file, "%d ", mat[i * size + j]);
		}
		fprintf(file, "\n");
	}
}

/* function that print an float matrix*/
void printFloatMatrix(float mat[], int size){
	int i, j;
	for(i = 0; i < size; ++i){
		for(j = 0; j < size; ++j){
			printf("%f ", mat[i * size + j]);
		}
		printf("\n");
	}
}

/* setting the max value in the arrays*/
void setMaxValue(int maxValue){
	int i,j;
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			if(R[i][j] > maxValue){
				R[i][j] = maxValue;
			}
			if(G[i][j] > maxValue){
				G[i][j] = maxValue;
			}
			if(B[i][j] > maxValue){
				B[i][j] = maxValue;
			}
		}
	}
}

/* conversion from RGB to YCbCr*/
void converRGBtoYCbCr(){
	int i, j;
	for(i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			int r = R[i][j];
			int g = G[i][j];
			int b = B[i][j];

			Y[i][j] = (0.299*r + 0.587*g + 0.114*b);
			Cb[i][j] = (-0.1687*r - 0.3313*g + 0.5*b + 128);
			Cr[i][j] = (0.5*r - 0.4187*g - 0.0813*b + 128);
		}
	}
}

/* shift*/
void shift(){
	int i, j;
	for(i = 0; i < SIZE; i++){
		for(j = 0; j < SIZE; j++){
			Y[i][j] -= SHIFT;
			Cb[i][j] -= SHIFT;
			Cr[i][j] -= SHIFT;
		}
	}
}

/* discrete cosine transform*/
void discreteCosineTransform(int selectedBlockN){

	int numberOfBlocks;  /* number of blocks*/

	int rowOfBlock, columnOfBlock; /* row and column of the block*/

	int indexOfRow, indexOfColumn;  /* index of the first element of the block in the matrix*/

	float Cu, Cv; /* C(u) and C(v) variables*/
	float sumY; /* sum of Y*/
	float sumCb; /* sum of Y*/
	float sumCr; /* sum of Y*/
	float cos1; /* first cosine of the formula*/
	float cos2; /* second cosine of the formula*/

	int i, j, u, v;

	numberOfBlocks = getNumberOfBlocks(SIZE); /* getting the number of blocks*/

	rowOfBlock = selectedBlockN / numberOfBlocks; /* getting the row of the block*/
	columnOfBlock = selectedBlockN % numberOfBlocks; /* getting the column of the block*/

	/*	printf("Row of the block = %d\n", rowOfBlock);  printing the row of the block*/
	/*	printf("Column of the block = %d\n", columnOfBlock);  printing the column of the block*/

	indexOfRow = rowOfBlock * BLOCKSIZE; /* getting the row index of the begging of the block*/
	indexOfColumn = columnOfBlock * BLOCKSIZE; /* getting the column index of the begging of the block*/

	/*printf("index of row = %d\n", indexOfRow);  printing the row index of the begging of the block*/
	/*printf("index Of column = %d\n", indexOfColumn);  printing the column index of the begging of the block*/

	/* getting the values of the selected block*/
	for(i = 0; i < BLOCKSIZE; ++i){
		for(j = 0; j < BLOCKSIZE; ++j){
			YofBlock[i][j] = Y[indexOfRow + i][indexOfColumn + j];
			CBofBlock[i][j] = Cb[indexOfRow + i][indexOfColumn + j];
			CRofBlock[i][j] = Cr[indexOfRow + i][indexOfColumn + j];
		}
	}
	
	/*printFloatMatrix(*YofBlock, BLOCKSIZE);
	printf("\n");
	printFloatMatrix(*CBofBlock, BLOCKSIZE);
	printf("\n");
	printFloatMatrix(*CRofBlock, BLOCKSIZE);
	printf("\n");*/

	/* making the DCT transformation with the given formula*/
	for(u = 0; u < BLOCKSIZE; ++u){
		for(v = 0; v < BLOCKSIZE; ++v){

			if(u == 0){
				Cu = 1/sqrt(2);
			}else{
				Cu = 1.;
			}

			if(v == 0){
				Cv = 1/sqrt(2);
			}else{
				Cv = 1.;
			}

			sumY = 0;
			sumCb = 0;
			sumCr = 0;

			for( i = 0; i < BLOCKSIZE; ++i){
				for(j = 0; j < BLOCKSIZE; ++j){
					cos1 = cos(((2 * i + 1) * u * M_PI) / 16);
					cos2 = cos(((2 * j + 1) * v * M_PI) / 16);
					sumY += YofBlock[i][j] * cos1 * cos2;
					sumCb += CBofBlock[i][j] * cos1 * cos2;
					sumCr += CRofBlock[i][j] * cos1 * cos2;
				}
			}

			DCT_Y[u][v] = Cu * Cv * sumY / 4.;
			DCT_CB[u][v] = Cu * Cv * sumCb / 4.;
			DCT_CR[u][v] = Cu * Cv * sumCr / 4.;

		}
	}

}


/* quantization of values*/
void quantizationOfValues(){
	int i, j;

	for(i = 0; i < BLOCKSIZE; ++i){
			for(j = 0; j < BLOCKSIZE; ++j){
				K_Y[i][j] = (DCT_Y[i][j] / K1[i][j]);
				K_CB[i][j] =(DCT_CB[i][j] / K2[i][j]);
				K_CR[i][j] = (DCT_CR[i][j] / K2[i][j]);
			}
		}
}

void myRound(){
	int i, j;

	for(i = 0; i < BLOCKSIZE; ++i){
		for(j = 0; j < BLOCKSIZE; ++j){
			IK_Y[i][j] = (int)floor(K_Y[i][j] + 0.5);
			IK_CB[i][j] = (int)floor(K_CB[i][j] + 0.5);
			IK_CR[i][j] = (int)floor(K_CR[i][j] + 0.5);
		}
	}
	
}


int main(int argc, char *argv[]){

	int i, j;

	char P[3]; /* P format of the file*/

	int numberOfRows; /* number of rows in the matrix*/
	int numberOfColumns; /* number of columns in the matrix*/
	int maxValue; /* max value*/

	int selectedBlockN;  /* selected block - N given with the second argument*/

	FILE *inputFile; /* input file given with the first argument*/
	FILE *outputFile; /* output file given with the first argument*/
	
	clock_t start, end;
	double cpu_time_used;
	
	/*********************************/
	/**/
	/* Reading values from the file*/
	/**/
	/********************************/
	start = clock();
	inputFile = fopen(argv[1],"rb"); /* opening input file*/

	/* checking that the file exist*/
	if(inputFile == NULL) {
		perror("Unable read file open file!");
		exit(1);
	}

	/* reading the P format of the file*/
	fscanf(inputFile, "%2s", P);
	P[2] = '\0';

	/* checking the P format*/
/*	printf("%s\n", P);*/
	if( strcmp(P, "P6")){
		printf("Wrong P");
	}


	numberOfRows = getNumberOfRows(inputFile); /* getting the number of the rows in the matrix*/
	numberOfColumns = getNumberOfColumns(inputFile); /* getting the number of the columns in the matrix*/
	maxValue = getMxaxValue(inputFile); /* getting the max value in the matrix*/

	/*printf("Number of rows: %d\n", numberOfRows);  printing the number of rows*/
	/*printf("Number of columns: %d\n", numberOfColumns);  printing the number of columns*/
	/*printf("Max number: %d\n", maxValue);  printing the max value*/

	if(numberOfRows != SIZE){
		perror("Wrong number of rows! The number of rows must be 512");
		exit(1);
	}

	if(numberOfColumns != SIZE){
		perror("Wrong number of columns! The number of rows must be 512");
		exit(1);
	}


	/*reading the values from the file*/
	for(i = 0; i < numberOfRows; i++){
		for(j = 0; j < numberOfColumns; j++){
			fread(&R[i][j], 1, 1, inputFile);
			fread(&G[i][j], 1, 1, inputFile);
			fread(&B[i][j], 1, 1, inputFile);
		}
	}



	
	setMaxValue(maxValue);
	converRGBtoYCbCr();
	shift();
	
	
	selectedBlockN = atoi(argv[2]); /* getting the N of the selected block*/
	
	for(selectedBlockN = 0; selectedBlockN< 4096; ++selectedBlockN){
		
	/*printf("%d\n", selectedBlockN);*/
	
	discreteCosineTransform(selectedBlockN);
	
	
	/*printFloatMatrix(*DCT_Y, BLOCKSIZE);
	printf("\n");
	printFloatMatrix(*DCT_CB, BLOCKSIZE);
	printf("\n");
	printFloatMatrix(*DCT_CR, BLOCKSIZE);
	printf("\n");*/
	
	
	quantizationOfValues();
	
	myRound();
	
	/*printIntMatrix(*IK_Y, BLOCKSIZE);
	printf("\n");
	printIntMatrix(*IK_CB, BLOCKSIZE);
	printf("\n");
	printIntMatrix(*IK_CR, BLOCKSIZE);
	printf("\n");*/
	
	outputFile = fopen(argv[3], "w+"); /*opening output file*/
	
	/* writing the result values in the given file*/
	writeIntMatrix(*IK_Y, BLOCKSIZE, outputFile);
	fprintf(outputFile, "\n");
	writeIntMatrix(*IK_CB, BLOCKSIZE, outputFile);
	fprintf(outputFile, "\n");
	writeIntMatrix(*IK_CR, BLOCKSIZE, outputFile);
	fprintf(outputFile, "\n");
	}
	printf("kraj\n");
	fclose(inputFile); /* closing the input file*/
	fclose(outputFile); /* closing the output file*/
	
	end = clock();
	cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Time: %f", cpu_time_used);
	return 0;
}

