#include "libcsv.h"

int main() {
	// output
	// header1,header3
	// 4,6
	const char csv[] = "header1,header2,header3\n1,2,3\n4,5,6\n7,8,9";
	processCsv(csv, "header1,header3", "header1>1\nheader3<8");

	// output
	// header1,header3
	// 4,6
	const char csv_file[] = "./custom.csv";
	processCsvFile(csv_file, "header3,header1", "header1>1\nheader3<8");
	
	// output
	// header1,header3,header4
	const char csv2[] = "header1,header2,header3,header4\n1,2,3,4\n5,6,7,8\n9,10,11,12";
	processCsv(csv, "header1,header3,header4", "header1>1\nheader3<10");

	// output:
	// header1,header3,header4
	const char csv_file2[] = "./custom.csv";
	processCsvFile(csv_file, "header1,header3", "header1>1\nheader3<10");

	return 0;
}
