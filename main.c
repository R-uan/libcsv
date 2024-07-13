#include "libcsv.h"
#include <stdio.h>
int main() {
	// output
	// header1,header3
	// 4,6
	printf("processCsv[0] output:\n\n");
	const char csv[] = "header1,header2,header3\n1,2,3\n4,5,6\n7,8,9";
	processCsv(csv, "header1,header3", "header1>1\nheader3<8");
	printf("\n");
 
	
	// output
	// header1,header3,header4
	printf("processCsv[1] output:\n\n");
	const char csv2[] = "header1,header2,header3,header4\n1,2,3,4\n5,6,7,8\n9,10,11,12";
	processCsv(csv2, "header1,header3,header4", "header1>1\nheader3<10");
	printf("\n");

	// output:
	// col1,col3,col4,coç7 
	printf("processCsvFile[0] output: \n\n");
	processCsvFile("./data.csv", "col1,col3,col4,col7", "col3>l1c3\ncol1>l1c1");
	printf("\n");

	// output:
	// perror
	printf("processCsvFile[1] output: \n\n");
	processCsvFile("./data.csv", "col1,col3,col4,col7", "col3#l1c3\ncol1>l1c1");
	printf("\n");
	
	// output
	// header1,header3
	// 4,6
	printf("processCsvFile[2] output: \n\n");
	const char csv_file[] = "./data.csv";
	processCsvFile(csv_file, "col1,col2", "header1>1\nheader3=6");
	printf("\n");

	return 0;
}
