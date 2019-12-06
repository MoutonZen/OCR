/**
  	BUILD_TXT
 	Description: file containing all that has to do with the build text process
 **/
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

void Build_txt(int *spaces_tab, size_t length)
{
	//*************************************************//
	//* This function write the text from OCR in file *//
	//*************************************************//

	// Open file
	FILE *file;
	file = fopen("OCR.txt", "w");

	// Write in file
	for (size_t i = 1; i < length ; i++)
	{
		
		fprintf(file, "%c", spaces_tab[i]);
	}

	// Close file
	fclose(file);
}