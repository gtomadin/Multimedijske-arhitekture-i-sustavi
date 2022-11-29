#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BLOCKSIZE 8
#define SHIFT 128.0

// function that returns the number of rows
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
// function that returns the number of columns
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

//function that returns the maximum value
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

// function that print an integer array
void printIntArray(int arr[], int size){
	for(int i = 0; i < size; ++i){
		printf("%d: %d\n", i, arr[i]);
	}
}

// function that print an float array
void printFloatArray(float arr[], int size){
	for(int i = 0; i < size; ++i){
		printf("%d: %f\n", i, arr[i]);
	}
}

// function that returns the number of blocks
int getNumberOfBlocks(int size){
	return size/BLOCKSIZE;
}

// function that print an integer matrix
void printIntMatrix(int mat[], int size){

	for(int i = 0; i < size; ++i){
		for(int j = 0; j < size; ++j){
			printf("%d ", mat[i * size + j]);
		}
		printf("\n");
	}
}

// function that write an integer matrix in the selected file
void writeIntMatrix(int mat[], int size, FILE *file){

	for(int i = 0; i < size; ++i){
		for(int j = 0; j < size; ++j){
			fprintf(file, "%d ", mat[i * size + j]);
		}
		fprintf(file, "\n");
	}
}

// function that print an float matrix
void printFloatMatrix(float mat[], int size){

	for(int i = 0; i < size; ++i){
		for(int j = 0; j < size; ++j){
			printf("%f ", mat[i * size + j]);
		}
		printf("\n");
	}
}


int main(int argc, char *argv[]){

	char P[3]; // P format of the file

	int numberOfRows; // number of rows in the matrix
	int numberOfColumns; // number of columns in the matrix
	int maxValue; // max value

	int numberOfBlocks; // number of blocks

	int selectedBlockN; // selected block - N given with the second argument

	int rowOfBlock, columnOfBlock; // row and column of the block

	int indexOfRow, indexOfColumn; // index of the first element of the block in the matrix


	float Cu, Cv; // C(u) and C(v) variables
	float sumY; // sum of Y
	float sumCb; // sum of Y
	float sumCr; // sum of Y
	float cos1; // first cosine of the formula
	float cos2; // second cosine of the formula

	// matrixes that contains the Y, Cb and Cb values of the given block
	float YofBlock[BLOCKSIZE][BLOCKSIZE] = {0};
	float CBofBlock[BLOCKSIZE][BLOCKSIZE] = {0};
	float CRofBlock[BLOCKSIZE][BLOCKSIZE] = {0};

	// matrixes that contains the Y, Cb and Cb values of the given block after the DCT
	float DCT_Y[BLOCKSIZE][BLOCKSIZE] = {0};
	float DCT_CB[BLOCKSIZE][BLOCKSIZE] = {0};
	float DCT_CR[BLOCKSIZE][BLOCKSIZE] = {0};

	// quantization table K1
	int K1[BLOCKSIZE][BLOCKSIZE] = { { 16, 11, 10, 16, 24, 40, 51, 61 },
		{ 12, 12, 14, 19, 26, 58, 60, 55 },
		{ 14, 13, 16, 24, 40, 57, 69, 56 },
		{ 14, 17, 22, 29, 51, 87, 80, 62 },
		{ 18, 22, 37, 56, 68, 109, 103, 77 },
		{ 24, 36, 55, 64, 81, 104, 113, 92 },
		{ 49, 64, 78, 87, 103, 121, 120, 101 },
		{ 72, 92, 95, 98, 112, 100, 103, 99 } };

	// quantization table K2
	int K2[BLOCKSIZE][BLOCKSIZE] = { { 17, 18, 24, 47, 99, 99, 99, 99 },
		{ 18, 21, 26, 66, 99, 99, 99, 99 },
		{ 24, 26, 56, 99, 99, 99, 99, 99 },
		{ 47, 66, 99, 99, 99, 99, 99, 99 },
		{ 99, 99, 99, 99, 99, 99, 99, 99 },
		{ 99, 99, 99, 99, 99, 99, 99, 99 },
		{ 99, 99, 99, 99, 99, 99, 99, 99 },
		{ 99, 99, 99, 99, 99, 99, 99, 99 } };


	// matrixes that contains the Y, Cb and Cb values of the given block after the quantization
	int K_Y[BLOCKSIZE][BLOCKSIZE] = {0};
	int K_CB[BLOCKSIZE][BLOCKSIZE] = {0};
	int K_CR[BLOCKSIZE][BLOCKSIZE] = {0};

	FILE *inputFile; // input file given with the first argument
	FILE *outputFile; // output file given with the first argument


	///////////////////////////////////
	//
	// Reading values from the file
	//
	///////////////////////////////////


	inputFile = fopen(argv[1],"rb"); // opening input file

	// checking that the file exist
	if(inputFile == NULL) {
		perror("Unable read file open file!");
		exit(1);
	}

	// reading the P format of the file
	fscanf(inputFile, "%2s", P);
	P[2] = '\0';

	// checking the P format
//	printf("%s\n", P);
	if( strcmp(P, "P6")){
		printf("Wrong P");
	}


	numberOfRows = getNumberOfRows(inputFile); // getting the number of the rows in the matrix
	numberOfColumns = getNumberOfColumns(inputFile); // getting the number of the columns in the matrix
	maxValue = getMxaxValue(inputFile); // // getting the max value in the matrix

//	printf("Number of rows: %d\n", numberOfRows); // printing the number of rows
//	printf("Number of columns: %d\n", numberOfColumns); // printing the number of columns
//	printf("Max number: %d\n", maxValue); // printing the max value

	// defining the dynamic integer arrays that contains the R, G and B values of the matrix
	int *R;
	R = (int *)malloc(sizeof(int) * numberOfRows * numberOfColumns);

	int *G;
	G = (int *)malloc(sizeof(int) * numberOfRows * numberOfColumns);

	int *B;
	B = (int *)malloc(sizeof(int) * numberOfRows * numberOfColumns);

	// reading the values from the file
	for(int i = 0; i < numberOfRows; i++){
		for(int j = 0; j < numberOfColumns; j++){
			fread(&R[i*numberOfColumns + j], 1, 1, inputFile);
			fread(&G[i*numberOfColumns + j], 1, 1, inputFile);
			fread(&B[i*numberOfColumns + j], 1, 1, inputFile);
		}
	}

	// setting the max value in the arrays
	for(int i = 0; i<numberOfRows * numberOfColumns; ++i){
		if(R[i] > maxValue){
			R[i] = maxValue;
		}
		if(G[i] > maxValue){
			G[i] = maxValue;
		}
		if(B[i] > maxValue){
			B[i] = maxValue;
		}
	}

	// printing the RGB values
//	printf("R: \n");
//	printIntArray(R, numberOfRows*numberOfColumns);
//	printf("G: \n");
//	printIntArray(G, numberOfRows*numberOfColumns);
//	printf("B: \n");
//	printIntArray(B, numberOfRows*numberOfColumns);

	///////////////////////////////////
	//
	// conversion from RGB to YCbCr
	//
	///////////////////////////////////

	// defining the dynamic integer arrays that contains the Y, Cb and Cr values of the matrix
	float *Y;
	Y = (float *)malloc(sizeof(float) * numberOfRows * numberOfColumns);

	float *Cb;
	Cb = (float *)malloc(sizeof(float) * numberOfRows * numberOfColumns);

	float *Cr;
	Cr = (float *)malloc(sizeof(float) * numberOfRows * numberOfColumns);

	// converting the values from RGB to YCbCr with the given formula
	for(int i=0; i<numberOfRows; i++){
		for(int j=0; j<numberOfColumns; j++){
			int r = R[i*numberOfColumns + j];
			int g = G[i*numberOfColumns + j];
			int b = B[i*numberOfColumns + j];

			Y[i*numberOfColumns + j] = (0.299*r + 0.587*g + 0.114*b);
			Cb[i*numberOfColumns + j] = (-0.1687*r - 0.3313*g + 0.5*b + 128);
			Cr[i*numberOfColumns + j] = (0.5*r - 0.4187*g - 0.0813*b + 128);
		}
	}

	// printing the YCbCr values
//	printf("Y: \n");
//	printFloatArray(Y, numberOfRows*numberOfColumns);
//	printf("Cb: \n");
//	printFloatArray(Cb, numberOfRows*numberOfColumns);
//	printf("Cr: \n");
//	printFloatArray(Cr, numberOfRows*numberOfColumns);


	///////////////////////////////////
	//
	// discrete cosine transform
	//
	///////////////////////////////////

	// Operating a shift
	for(int i = 0; i < numberOfRows*numberOfColumns; ++i){
			Y[i] -= SHIFT;
			Cb[i] -= SHIFT;
			Cr[i] -= SHIFT;
	}


	selectedBlockN = atoi(argv[2]); // getting the N of the selected block

//	printf("N of Block: %d\n", selectedBlockN); // printing the selected block

	numberOfBlocks = getNumberOfBlocks(numberOfRows); // getting the number of blocks

	rowOfBlock = selectedBlockN / numberOfBlocks; // getting the row of the block
	columnOfBlock = selectedBlockN % numberOfBlocks; // getting the column of the block

//	printf("Row of the block = %d\n", rowOfBlock); // printing the row of the block
//	printf("Column of the block = %d\n", columnOfBlock); // printing the column of the block

	indexOfRow = rowOfBlock * BLOCKSIZE; // getting the row index of the begging of the block
	indexOfColumn = columnOfBlock * BLOCKSIZE; // getting the column index of the begging of the block

//	printf("index of row = %d\n", indexOfRow); // printing the row index of the begging of the block
//	printf("index Of column = %d\n", indexOfColumn); // printing the column index of the begging of the block

	// getting the values of the selected values
	for(int i = 0; i < BLOCKSIZE; ++i){
		for(int j = 0; j < BLOCKSIZE; ++j){
			YofBlock[i][j] = Y[(indexOfRow + i)*numberOfColumns + (indexOfColumn + j)];
			CBofBlock[i][j] = Cb[(indexOfRow + i)*numberOfColumns + (indexOfColumn + j)];
			CRofBlock[i][j] = Cr[(indexOfRow + i)*numberOfColumns + (indexOfColumn + j)];
		}
	}


	// making the DCT transformation with the given formula
	for(int u = 0; u < BLOCKSIZE; ++u){
		for(int v = 0; v < BLOCKSIZE; ++v){

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

			for(int i = 0; i < BLOCKSIZE; ++i){
				for(int j = 0; j < BLOCKSIZE; ++j){
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

	///////////////////////////////////
	//
	// quantization of values
	//
	///////////////////////////////////


	for(int i = 0; i < BLOCKSIZE; ++i){
		for(int j = 0; j < BLOCKSIZE; ++j){
			K_Y[i][j] = (round)(DCT_Y[i][j] / K1[i][j]);
			K_CB[i][j] = (round)(DCT_CB[i][j] / K2[i][j]);
			K_CR[i][j] = (round)(DCT_CR[i][j] / K2[i][j]);
		}
	}

//	printf("\n");

	///////////////////////////////////
	//
	// Writing the values in the file
	//
	///////////////////////////////////

	outputFile = fopen(argv[3], "w+"); //opening output file


	// printing the result values
//	printIntMatrix(KY, BLOCKSIZE);
//	printf("\n");
//	printIntMatrix(KCB, BLOCKSIZE);
//	printf("\n");
//	printIntMatrix(KCR, BLOCKSIZE);
//	printf("\n");

	// writing the result values in the given file
	writeIntMatrix(*K_Y, BLOCKSIZE, outputFile);
	fprintf(outputFile, "\n");
	writeIntMatrix(*K_CB, BLOCKSIZE, outputFile);
	fprintf(outputFile, "\n");
	writeIntMatrix(*K_CR, BLOCKSIZE, outputFile);
	fprintf(outputFile, "\n");


	fclose(inputFile); // closing the input file
	fclose(outputFile); // closing the output file
	return 0;
}

