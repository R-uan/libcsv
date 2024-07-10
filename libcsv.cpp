#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern "C"
{
	//	Typdefs
	typedef struct
	{
		int target;
		char *signal;
		char *value;
		int valid;
	} Filter;

	typedef struct
	{
		Filter *array;
		int length;
	} FilterArray;

	typedef struct
	{
		int *array;
		int length;
	} IntArray;

	typedef struct
	{
		char **array;
		int length;
	} CharArray;

	typedef struct
	{
		char ***matrix;
		int rows;
		int cols;
	} Matrix;
	//
	//	Functions definitions.
	void freeMatrix(Matrix *matrix);
	void freeCharArray(CharArray array);

	void removeNewline(char *str);
	void printCharArray(char **array);
	void filter(Matrix matrix, FilterArray filter);
	void removeElement(CharArray *array, int index);
	CharArray split(const char *input, char delimiter);
	void print2DCharArray(char ***array, int rows, int cols);
	CharArray filterHeader(CharArray header, CharArray splitSelected);
	Matrix selectCsvData(IntArray selectedHeaderIndexes, CharArray data);
	Matrix selectCsvFileData(IntArray selectedHeaderIndexes, FILE *file);
	IntArray matchingValuesIndexes(CharArray splitHeader, CharArray splitSelected);
	FilterArray getFilterTarget(CharArray header, const char rowFilterDefinitions[]);

	//
	void processCsv(const char data[], const char selectedColumns[], const char rowFilterDefinitions[])
	{
		printf("Process CSV:\n");
		CharArray lines = split(data, '\n');
		CharArray splitHeader = split(lines.array[0], ',');
		removeElement(&lines, 0);
		lines.array[lines.length] = NULL;

		CharArray splitSelected;
		if (selectedColumns[0] == '\0')
			splitSelected = splitHeader;
		else
			splitSelected = split(selectedColumns, ',');

		CharArray selectedHeader = filterHeader(splitHeader, splitSelected);
		printCharArray(selectedHeader.array);
		printf("\n");

		IntArray selectedHeaderIndexes = matchingValuesIndexes(splitHeader, splitSelected);
		Matrix matrix = selectCsvData(selectedHeaderIndexes, lines);
		FilterArray filterTarget = getFilterTarget(splitSelected, rowFilterDefinitions);
		filter(matrix, filterTarget);
		printf("\n");
	}

	//
	void processCsvFile(const char csv[], const char selectedColumns[], const char rowFilterDefinitions[])
	{
		printf("Process CSV File:\n");
		FILE *file;
		file = fopen(csv, "r");
		if (file == NULL)
		{
			perror("womp womp, couldn't open the file.");
			return exit(EXIT_FAILURE);
		}

		char buffer[2048];
		CharArray splitHeader;
		if (fgets(buffer, sizeof(buffer), file))
		{
			removeNewline(buffer);
			splitHeader = split(buffer, ',');
		}
		CharArray splitSelected;
		if (selectedColumns[0] == '\0')
			splitSelected = splitHeader;
		else
			splitSelected = split(selectedColumns, ',');

		CharArray selectedHeader = filterHeader(splitHeader, splitSelected);
		printCharArray(selectedHeader.array);
		printf("\n");

		IntArray selectedHeaderIndexes = matchingValuesIndexes(splitHeader, splitSelected);
		Matrix matrix = selectCsvFileData(selectedHeaderIndexes, file);
		FilterArray filterTarget = getFilterTarget(splitSelected, rowFilterDefinitions);
		filter(matrix, filterTarget);
		fclose(file);
		printf("\n");
	}
}

CharArray filterHeader(CharArray header, CharArray splitSelected)
{
	int cols = 1;
	int index = 0;
	char **temp = (char **)malloc(cols * sizeof(char *));
	for (int i = 0; i < header.length; i++)
	{
		for (int k = 0; k < splitSelected.length; k++)
		{
			if (strcmp(header.array[i], splitSelected.array[k]) == 0)
			{
				temp[index] = (char *)malloc((strlen(header.array[i]) + 1) * sizeof(char));
				strcpy(temp[index], header.array[i]);
				cols++;
				index++;
				temp = (char **)realloc(temp, cols * sizeof(char *));
			}
		}
	}

	temp[index] = NULL;

	CharArray result;
	result.array = temp;
	result.length = index;

	return result;
}

//
//
//
void removeElement(CharArray *array, int index)
{
	if (index < 0 || index >= array->length)
	{
		printf("Invalid index\n");
		return;
	}

	free(array->array[index]);

	for (int i = index; i < array->length - 1; i++)
	{
		array->array[i] = array->array[i + 1];
	}

	array->array[array->length - 1] = NULL;
	array->length--;
}

void removeNewline(char *str)
{
	size_t len = strlen(str);
	if (len > 0 && str[len - 1] == '\n')
	{
		str[len - 1] = '\0';
	}
}

//
//	Summary:
//		Filters the CSV file's data based on the selected columns indexes.
//	Returns:
//		Matrix:
//			A bidimensional array containing the rows and columns of the selected indexes.
//			Lenght of the rows and columns of the selected columns.
Matrix selectCsvFileData(IntArray selectedHeaderIndexes, FILE *file)
{
	char buffer[256];
	int rows = 0;
	int cols = selectedHeaderIndexes.length;
	char ***selectedMatrix = (char ***)malloc(sizeof(char **));
	if (selectedMatrix == NULL)
	{
		perror("selectCsvFileData(***selectedMatrix): Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	while (fgets(buffer, sizeof(buffer), file))
	{
		char ***tempMatrix = (char ***)realloc(selectedMatrix, (rows + 1) * sizeof(char **));
		if (tempMatrix == NULL)
		{
			perror("selectCsvFileData - ***selectedMatrix: Memory reallocation failed");
			exit(EXIT_FAILURE);
		}

		selectedMatrix = tempMatrix;
		selectedMatrix[rows] = (char **)malloc(cols * sizeof(char *));
		if (selectedMatrix[rows] == NULL)
		{
			perror("selectCsvFileData(**selectedMatrix[rows]): Memory allocation failed");
			exit(EXIT_FAILURE);
		}

		CharArray splitLine = split(buffer, ',');
		for (int j = 0; j < cols; j++)
		{
			char *targetValue = splitLine.array[selectedHeaderIndexes.array[j]];
			removeNewline(targetValue);
			selectedMatrix[rows][j] = (char *)malloc((strlen(targetValue) + 1) * sizeof(char));
			if (selectedMatrix[rows][j] == NULL)
			{
				perror("selectCsvFileData(*selectedMatrix[rows][j]1): Memory allocation failed");
				exit(EXIT_FAILURE);
			}
			strcpy(selectedMatrix[rows][j], targetValue);
		}
		rows++;
	}

	Matrix matrix;
	matrix.cols = cols;
	matrix.rows = rows;
	matrix.matrix = selectedMatrix;
	return matrix;
}

//
//	Summary:
//		Filters the CSV data based on the selected columns indexes.
//	Returns:
//		Matrix:
//			A bidimensional array containing the rows and columns of the selected indexes.
//			Lenght of the rows and columns of the selected columns.
Matrix selectCsvData(IntArray selectedHeaderIndexes, CharArray data)
{
	int rows = 0;
	int cols = selectedHeaderIndexes.length;
	char ***selectedMatrix = NULL;

	for (int i = 0; i < data.length; i++)
	{
		char *line = data.array[i];
		removeNewline(line);
		char ***tempMatrix = (char ***)realloc(selectedMatrix, (rows + 1) * sizeof(char **));
		if (tempMatrix == NULL)
		{
			perror("selectCsvData(***selectedMatrix): Memory reallocation failed");
			exit(EXIT_FAILURE);
		}

		selectedMatrix = tempMatrix;
		selectedMatrix[rows] = (char **)malloc(cols * sizeof(char *));
		if (selectedMatrix[rows] == NULL)
		{
			perror("selectCsvData(**selectedMatrix[rows]): Memory allocation failed");
			exit(EXIT_FAILURE);
		}

		CharArray splitLine = split(line, ',');
		for (int j = 0; j < cols; j++)
		{
			char *targetValue = splitLine.array[selectedHeaderIndexes.array[j]];
			selectedMatrix[rows][j] = (char *)malloc((strlen(targetValue) + 1) * sizeof(char));
			if (selectedMatrix[rows][j] == NULL)
			{
				perror("selectCsvData(*selectedMatrix[rows][j]2): Memory allocation failed");
				exit(EXIT_FAILURE);
			}

			strcpy(selectedMatrix[rows][j], targetValue);
		}
		rows++;
	}

	Matrix matrix;
	matrix.cols = cols;
	matrix.rows = rows;
	matrix.matrix = selectedMatrix;
	return matrix;
}

//
//	Summary:
//		Moves the rows up and decrease the row size.
void removeRow(char ***matrix, int *rows, int *cols, int rowIndex)
{
	if (matrix == NULL || *matrix == NULL || rowIndex >= *rows)
		return;

	for (int j = 0; j < *cols; j++)
	{
		free(matrix[rowIndex][j]);
	}
	free(matrix[rowIndex]);

	for (int i = rowIndex; i < *rows - 1; i++)
	{
		matrix[i] = matrix[i + 1];
	}

	(*rows)--;
}

//
//	Summary:
//		Get the target column by the index.
//	Returns:
//		FilterArray:
//			Filter array.
//			Length of the Filter array.
FilterArray getFilterTarget(CharArray splitHeader, const char rowFilterDefinitions[])
{
	int size = 0;
	int indice = 0;
	CharArray splitFilter = split(rowFilterDefinitions, '\n');
	Filter *identifiedFilters = (Filter *)malloc(sizeof(Filter));
	if (identifiedFilters == NULL)
	{
		perror("getFilterTarget(identifiedFilters): Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	for (int k = 0; k < splitHeader.length; k++)
	{
		for (int i = 0; i < splitFilter.length; i++)
		{
			if (strstr(splitFilter.array[i], splitHeader.array[k]) != NULL)
			{
				char signal = '\0';
				if (strchr(splitFilter.array[i], '<') != NULL)
					signal = '<';
				else if (strchr(splitFilter.array[i], '>'))
					signal = '>';
				else if (strchr(splitFilter.array[i], '='))
					signal = '=';

				if (signal != '\0')
				{
					CharArray temp = split(splitFilter.array[i], signal);
					identifiedFilters[indice].signal = (char *)malloc(2 * sizeof(char));
					identifiedFilters[indice].signal[0] = signal;
					identifiedFilters[indice].signal[1] = '\0';
					identifiedFilters[indice].target = k;
					identifiedFilters[indice].value = strdup(temp.array[1]);
					identifiedFilters[indice].valid = 1;

					size++;
					indice++;
					Filter *tempFilters = (Filter *)realloc(identifiedFilters, (size + 1) * sizeof(Filter));
					if (tempFilters == NULL)
					{
						perror("getFilterTarget(tempFilters): Memory reallocation failed");
						exit(EXIT_FAILURE);
					}
					identifiedFilters = tempFilters;

					freeCharArray(temp);
				}
			}
		}
	}

	freeCharArray(splitFilter);

	FilterArray result;
	result.array = identifiedFilters;
	result.length = size;

	return result;
}

//
//	Summary:
//		Applies the filter to the matrix and prints the final result.
void filter(Matrix matrix, FilterArray filter)
{
	for (int k = 0; k < filter.length; k++)
	{
		int target = filter.array[k].target;
		for (int i = matrix.rows - 1; i >= 0; i--)
		{
			int comparison = strcmp(matrix.matrix[i][target], filter.array[k].value);
			if (*filter.array[k].signal == '=')
			{
				if (comparison != 0)
				{
					removeRow(matrix.matrix, &matrix.rows, &matrix.cols, i);
				}
			}
			else if (*filter.array[k].signal == '>')
			{
				if (comparison <= 0)
				{
					removeRow(matrix.matrix, &matrix.rows, &matrix.cols, i);
				}
			}
			else if (*filter.array[k].signal == '<')
			{
				if (comparison >= 0)
				{
					removeRow(matrix.matrix, &matrix.rows, &matrix.cols, i);
				}
			}
		}
	}

	print2DCharArray(matrix.matrix, matrix.rows, matrix.cols);
}

//
//	Summary:
//		Finds the indexes of the selected columns.
//	Returns:
//		IntArray:
//			Columns indexes array.
//			Length of the array.
IntArray matchingValuesIndexes(CharArray splitHeader, CharArray splitSelected)
{
	int size = 0;
	int *match = NULL;
	for (int i = 0; i < splitHeader.length; i++)
	{
		for (int k = 0; k < splitSelected.length; k++)
		{
			if (strcmp(splitHeader.array[i], splitSelected.array[k]) == 0)
			{
				int *temp = (int *)realloc(match, (size + 1) * sizeof(int));
				if (temp == NULL)
				{
					perror("matchingvValuesIndex(temp1): Memory allocation failed");
					exit(EXIT_FAILURE);
				}
				match = temp;
				match[size] = i;
				size++;
				break;
			}
		}
	}

	int *temp = (int *)realloc(match, (size + 1) * sizeof(int));
	if (temp == NULL)
	{
		perror("matchingvValuesIndex(temp2): Memory allocation failed");
		exit(EXIT_FAILURE);
	}
	match = temp;
	match[size] = -1;

	IntArray result;
	result.array = match;
	result.length = size;
	return result;
}

//
//	Summary:
//		Counts the amount of strings to split.
//	Returns:
//		int amount.
int count(const char *input, char delimiter)
{
	int count = 0;
	while (*input)
	{
		if (*input == delimiter)
		{
			count++;
		}
		input++;
	}
	return count;
}

//
//	Summary:
//		Split a string into an array based on the given delimiter.
//	Returns:
//		CharArray:
//			Array of strings
//			Length of the array
CharArray split(const char *input, char delimiter)
{
	int index = 0;
	const char *start = input;
	int size = count(input, delimiter) + 1;
	char **array = (char **)malloc((size + 1) * sizeof(char *));
	if (array == NULL)
	{
		perror("split(array)1: Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	while (*input)
	{
		if (*input == delimiter)
		{
			int length = input - start;
			array[index] = (char *)malloc((length + 1) * sizeof(char));
			if (array[index] == NULL)
			{
				perror("split(array[index]1): Memory allocation failed");
				exit(EXIT_FAILURE);
			}

			strncpy(array[index], start, length);
			array[index][length] = '\0';
			index++;
			start = input + 1;
		}
		input++;
	}

	int length = input - start;
	array[index] = (char *)malloc((length + 1) * sizeof(char));
	if (array[index] == NULL)
	{
		perror("split(array[index]2): Memory allocation failed");
		exit(EXIT_FAILURE);
	}

	strncpy(array[index], start, length);
	array[index][length] = '\0';
	index++;
	array[index] = NULL;

	CharArray result;
	result.array = array;
	result.length = size;
	return result;
}

//
//	Summary:
//		Prints 2D Array.
void print2DCharArray(char ***array, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			printf("%s ", array[i][j]);
		}
		printf("\n");
	}
}

//
//	Summary:
//		Prints array.
void printCharArray(char **array)
{
	int i = 0;
	while (array[i] != NULL)
	{
		printf("%s ", array[i]);
		i++;
	}
}

//
void freeMatrix(Matrix *matrix)
{
	if (matrix == NULL || matrix->matrix == NULL)
		return;

	for (int i = 0; i < matrix->rows; i++)
	{
		for (int j = 0; j < matrix->cols; j++)
		{
			free(matrix->matrix[i][j]);
		}
		free(matrix->matrix[i]);
	}
	free(matrix->matrix);
	matrix->matrix = NULL;
	matrix->rows = 0;
	matrix->cols = 0;
}

//
void freeCharArray(CharArray array)
{
	for (int i = 0; i < array.length; i++)
	{
		free(array.array[i]);
	}
	free(array.array);
}
