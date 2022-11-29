#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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



int main(int argc, char *argv[]){

	char P[3]; /* P format of the file */

	int numberOfRows; /* number of rows in the matrix */
	int numberOfColumns; /* number of columns in the matrix */
	int maxValue; /* max value */

	char G0[5] = {'0', '0', '0', '0', '\0'};
	char G1[5] = {'0', '0', '0', '1', '\0'};
	char G2[5] = {'0', '0', '1', '0', '\0'};
	char G3[5] = {'0', '0', '1', '1', '\0'};
	char G4[5] = {'0', '1', '0', '0', '\0'};
	char G5[5] = {'0', '1', '0', '1', '\0'};
	char G6[5] = {'0', '1', '1', '0', '\0'};
	char G7[5] = {'0', '1', '1', '1', '\0'};
	char G8[5] = {'1', '0', '0', '0', '\0'};
	char G9[5] = {'1', '0', '0', '1', '\0'};
	char G10[5] = {'1', '0', '1', '0', '\0'};
	char G11[5] = {'1', '0', '1', '1', '\0'};
	char G12[5] = {'1', '1', '0', '0', '\0'};
	char G13[5] = {'1', '1', '0', '1', '\0'};
	char G14[5] = {'1', '1', '1', '0', '\0'};
	char G15[5] = {'1', '1', '1', '1', '\0'};

	int counters[16] = {0};
	int counterAll = 0;
	float share = 0;

	FILE *inputFile; /* input file given with the first argument */

	int result;

	int pictureSize;

	int i, j, k;
	int value;
	char binary[9];
	char helpbinary[9], helpbinary2[9];
	char tmp[10];
	int *pixelValues;
	int length, helplength;
	int bit;

	char inputhelp[10] = "lenna.pgm";

	inputFile = fopen(inputhelp,"rb"); /* opening input file */

	/* checking that the file exist */
	if(inputFile == NULL) {
		perror("Unable read file open file!");
		exit(1);
	}

	/* reading the P format of the file */
	fscanf(inputFile, "%2s", P);
	P[2] = '\0';



	numberOfRows = getNumberOfRows(inputFile); /* getting the number of the rows in the matrix */
	numberOfColumns = getNumberOfColumns(inputFile); /* getting the number of the columns in the matrix */
	maxValue = getMxaxValue(inputFile);  /* getting the max value in the matrix */

	/*printf("Number of rows: %d\n", numberOfRows); */ /* printing the number of rows */
	/*printf("Number of columns: %d\n", numberOfColumns); */ /* printing the number of columns */
	/*printf("Max number: %d\n", maxValue);*/ /* printing the max value */

	pictureSize = numberOfRows * numberOfColumns;



	pixelValues = (int *)malloc(sizeof(int) * pictureSize);

	for(i = 0; i < pictureSize; i++){
		fread(&pixelValues[i], 1, 1, inputFile);
	}


	/* printIntArray(pixelValues, pictureSize); */


	for(i = 0; i < pictureSize; ++i){

		/*itoa(pixelValues[i],binary,2);*/

		value = pixelValues[i];
		j = 0;
		while(1){
			bit = value % 2;
			if(bit == 0){
				helpbinary[j] = '0';
			}else{
				helpbinary[j] = '1';
			}
			++j;
			value /= 2;
			if(value == 0){
				helpbinary[j] = '\0';
				break;
			}
		}

		helplength = strlen(helpbinary);
		for(k = 0; k < helplength; ++k){
			helpbinary2[k] = helpbinary[helplength - k -1];
		}
		helpbinary2[k] = '\0';
		/* help print
		printf("%s\n", binary);
		printf("%s\n", helpbinary);
		printf("%s\n", helpbinary2);
		printf("\n");
		*/

		/*
		if(i == 1000){
			break;
		}
		*/

		length = strlen(helpbinary2);

		for(k = 0; k < length; ++k){
			binary[k] = helpbinary2[k];
		}

		binary[k] = '\0';

		if(length == 8){
			snprintf(tmp, sizeof tmp, "%s", binary);
		}

		if(length == 7){
			snprintf(tmp, sizeof tmp, "0%s", binary);
		}
			
		if(length == 6){
			snprintf(tmp, sizeof tmp, "00%s", binary);
		}
			
		if(length == 5){
			snprintf(tmp, sizeof tmp, "000%s", binary);
		}
			
		if(length == 4){
			snprintf(tmp, sizeof tmp, "0000%s", binary);
		}
			
		if(length == 3){
			snprintf(tmp, sizeof tmp, "00000%s", binary);
		}
			
		if(length == 2){
			snprintf(tmp, sizeof tmp, "000000%s", binary);
		}
			
		if(length == 1){
			snprintf(tmp, sizeof tmp, "0000000%s", binary);
		}
			

		result = strncmp(tmp, G0, 4);
		if(result == 0){
			counters[0]++;
			counterAll++;
		}

		result = strncmp(tmp, G1, 4);
		if(result == 0){
			counters[1]++;
			counterAll++;
		}
			
		result = strncmp(tmp, G2, 4);
		if(result == 0){
			counters[2]++;
			counterAll++;
		}

		result = strncmp(tmp, G3, 4);
		if(result == 0){
			counters[3]++;
			counterAll++;
		}

		result = strncmp(tmp, G4, 4);
		if(result == 0){
			counters[4]++;
			counterAll++;
		}

		result = strncmp(tmp, G5, 4);
		if(result == 0){
			counters[5]++;
			counterAll++;
		}
		result = strncmp(tmp, G6, 4);
		if(result == 0){
			counters[6]++;
			counterAll++;
		}
		result = strncmp(tmp, G7, 4);
		if(result == 0){
			counters[7]++;
			counterAll++;
		}
			
		result = strncmp(tmp, G8, 4);
		if(result == 0){
			counters[8]++;
			counterAll++;
		}
			
		result = strncmp(tmp, G9, 4);
		if(result == 0){
			counters[9]++;
			counterAll++;
		}
			
		result = strncmp(tmp, G10, 4);
		if(result == 0){
			counters[10]++;
			counterAll++;
		}
			
		result = strncmp(tmp, G11, 4);
		if(result == 0){
			counters[11]++;
			counterAll++;
		}
			
		result = strncmp(tmp, G12, 4);
		if(result == 0){
			counters[12]++;
			counterAll++;
		}
			
		result = strncmp(tmp, G13, 4);
		if(result == 0){
			counters[13]++;
			counterAll++;
		}
			
		result = strncmp(tmp, G14, 4);
		if(result == 0){
			counters[14]++;
			counterAll++;
		}
			
		result = strncmp(tmp, G15, 4);
		if(result == 0){
			counters[15]++;
			counterAll++;
		}


	}

	for( i = 0; i < 16; ++i){
		share = (float)counters[i]/counterAll;
		printf("%d: %f\n", i, share);
	}

	fclose(inputFile);

	return 0;
}
