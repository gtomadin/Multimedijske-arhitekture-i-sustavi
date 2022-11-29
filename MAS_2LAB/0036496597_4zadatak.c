#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BLOCKSIZE 16
#define DIMENSION 512
#define DIMENSION_IN_BLOCKS 32
#define MINVALUE 0
#define MAXVALUE 496

/* function that returns the number of rows */
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
/* function that returns the number of columns */
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

/* function that returns the maximum value */
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

/* function that print an integer array */
void printIntArray(int arr[], int size){
	int i;
	for(i = 0; i < size; ++i){
		if(arr[i] < 100){
			printf("%d: %d\n", i, arr[i]);
		}
	}
}

void printIntMatrix(int arr[], int size){
	int i, j;
	for(i = 0; i < 32; ++i){
		for(j = 0; j < 32; ++j){
			printf("%d ", arr[i*size + j]);
		}
		printf("\n");
	}
}

int main(int argc, char *argv[]){

	char PFile1[3]; /* P format of the file */
	char PFile2[3];  /* P format of the file */

	int numberOfRowsFile1; /* number of rows in the matrix */
	int numberOfColumnsFile1; /* number of columns in the matrix */
	int maxValueFile1; /* max value */

	int numberOfRowsFile2; /* number of rows in the matrix */
	int numberOfColumnsFile2; /* number of columns in the matrix */
	int maxValueFile2; /* max value */

	FILE *inputFile1;
	FILE *inputFile2;

	int pictureSizeFile1;


	int *pixelValuesFile1;

	int pixelValeusFile2[DIMENSION][DIMENSION] = {0};

	int pixelValeusSelectedBlock[BLOCKSIZE][BLOCKSIZE] = {0};

	int selectedBlock;

	int i, j, x, y;
	int selectedBlockRow, selectedBlockColumn;
	int indexOfSelectedBlockRow, indexOfSelectedBlockColumn;

	int indexOfEvaluatingBlockRow, indexOfEvaluatingBlockColumn;

	char stringInputFile1[12] = "lenna1.pgm";
	char stringInputFile2[12] = "lenna.pgm";

	double diff, sum;
	double mean, mad;
	int minRowOfBlock, minColumnOfBlock;
	int rowDiff, columnDiff;

	int minRowIndex, maxRowIndex, minColumnIndex, maxColumnIndex;

	inputFile1 = fopen(stringInputFile1, "rb");


	if(inputFile1 == NULL) {
		perror("Unable read file open file!");
		exit(1);
	}

	fscanf(inputFile1, "%2s", PFile1);
	PFile1[2] = '\0';

	numberOfRowsFile1 = getNumberOfRows(inputFile1);
	numberOfColumnsFile1 = getNumberOfColumns(inputFile1);
	maxValueFile1 = getMxaxValue(inputFile1);

	/* help print
	printf("Number of rows: %d\n", numberOfRowsFile1);
	printf("Number of columns: %d\n", numberOfColumnsFile1);
	printf("Max number: %d\n", maxValueFile1);
	*/


	if(numberOfRowsFile1 * numberOfColumnsFile1 != DIMENSION * DIMENSION){
		perror("Wrong dimension of the picture");
	}

	pictureSizeFile1 = numberOfRowsFile1 * numberOfColumnsFile1;

	pixelValuesFile1 = (int *)malloc(sizeof(int) * pictureSizeFile1);

	for(i = 0; i < pictureSizeFile1; i++){
		fread(&pixelValuesFile1[i], 1, 1, inputFile1);
	}

	/*printIntMatrix(pixelValuesFile1, DIMENSION);*/



	/* druga datoteka */

	inputFile2 = fopen(stringInputFile2, "rb");

	if(inputFile2 == NULL) {
		perror("Unable read file open file!");
		exit(1);
	}

		fscanf(inputFile2, "%2s", PFile2);
		PFile2[2] = '\0';

		numberOfRowsFile2 = getNumberOfRows(inputFile2);
		numberOfColumnsFile2 = getNumberOfColumns(inputFile2);
		maxValueFile2 = getMxaxValue(inputFile2);

		/* help print
		printf("Number of rows: %d\n", numberOfRowsFile2);
		printf("Number of columns: %d\n", numberOfColumnsFile2);
		printf("Max number: %d\n", maxValueFile2);
		*/

		if(numberOfRowsFile2 * numberOfColumnsFile2 != DIMENSION * DIMENSION){
			perror("Wrong dimension of the picture");
		}

		for(i = 0; i < DIMENSION; i++){
				for(j = 0; j < DIMENSION; j++){
					fread(&pixelValeusFile2[i][j], 1, 1, inputFile2);
				}
			}
		/*
		for(i = 0; i < DIMENSION; ++i){
				for(j = 0; j < DIMENSION; ++j){
					printf("%d ", pixelValeusFile2[i][j]);
				}
				printf("\n");
			}
		*/

		selectedBlock = atoi(argv[1]);

		/*printf("%d\n", selectedBlock);*/

		selectedBlockRow = selectedBlock / DIMENSION_IN_BLOCKS;

		selectedBlockColumn = selectedBlock % DIMENSION_IN_BLOCKS;

		/* help print
		printf("Row of the block = %d\n", selectedBlockRow);
		printf("Column of the block = %d\n", selectedBlockColumn);
		*/

		indexOfSelectedBlockRow = selectedBlockRow * BLOCKSIZE;
		indexOfSelectedBlockColumn = selectedBlockColumn * BLOCKSIZE;

		/* help print
		printf("index of row = %d\n", indexOfSelectedBlockRow);
		printf("index Of column = %d\n", indexOfSelectedBlockColumn);
		*/


		for(i = 0; i < BLOCKSIZE; ++i){
			for(j = 0; j < BLOCKSIZE; ++j){
				pixelValeusSelectedBlock[i][j] = pixelValuesFile1[(indexOfSelectedBlockRow + i) * numberOfColumnsFile1 + (indexOfSelectedBlockColumn + j)];
			}
		}

		/*
		for(i = 0; i < BLOCKSIZE; ++i){
			for(j = 0; j < BLOCKSIZE; ++j){
				printf("%d ", pixelValeusSelectedBlock[i][j]);
			}
			printf("\n");
		}
		*/

		/*printf("\n\n");*/

		minRowIndex = indexOfSelectedBlockRow - BLOCKSIZE;
		minColumnIndex = indexOfSelectedBlockColumn - BLOCKSIZE;
		maxRowIndex = indexOfSelectedBlockRow + BLOCKSIZE;
		maxColumnIndex = indexOfSelectedBlockColumn + BLOCKSIZE;

		if(minRowIndex < MINVALUE){
			minRowIndex = MINVALUE;
		}
		if(minColumnIndex < MINVALUE){
			minColumnIndex = MINVALUE;
		}

		if(maxRowIndex > MAXVALUE){
			maxRowIndex = MAXVALUE;
		}

		if(maxColumnIndex > MAXVALUE){
			maxColumnIndex = MAXVALUE;
		}



		mad = 10000000.0;
		for(i = minRowIndex; i <= maxRowIndex; ++i){
			for(j = minColumnIndex; j <= maxColumnIndex; ++j){




				indexOfEvaluatingBlockRow = i;
				indexOfEvaluatingBlockColumn = j;

				/*
				printf("(%d,%d)\n", i - indexOfSelectedBlockRow, j - indexOfSelectedBlockColumn);
				*/

				sum = 0;
				mean = 0;
				diff = 0;
				for(x = 0; x < BLOCKSIZE; ++x){
					for(y = 0; y < BLOCKSIZE; ++y){

						/*printf("%d ", pixelValeusFile2[indexOfEvaluatingBlockRow + x][indexOfEvaluatingBlockColumn + y]);*/

						diff = abs(pixelValeusSelectedBlock[x][y] - pixelValeusFile2[indexOfEvaluatingBlockRow + x][indexOfEvaluatingBlockColumn + y]);


						sum += diff;


					}

					/*printf("\n");*/

				}


				mean = sum /(BLOCKSIZE * BLOCKSIZE);
				/*printf("%f %f\n", sum, mean);*/


				if(mean < mad){
					mad = mean;
					minRowOfBlock = indexOfEvaluatingBlockRow;
					minColumnOfBlock = indexOfEvaluatingBlockColumn;
				}

			}

		}


	/* help print
	printf("min row: %d\n", minRowOfBlock);
	printf("min column: %d\n", minColumnOfBlock);
	*/

	rowDiff = minRowOfBlock - indexOfSelectedBlockRow;
	columnDiff = minColumnOfBlock - indexOfSelectedBlockColumn;


	printf("%d,%d\n", columnDiff, rowDiff);
	/*printf("%f", mad);*/

	fclose(inputFile1);
	fclose(inputFile2);

	free(pixelValuesFile1);

	return 0;
}
