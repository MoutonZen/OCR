/**
  	BUILD_TXT
 	Description: file containing all that has to do with the build text process
 **/
#include <err.h>
#include <stdlib.h>
#include <stdio.h>

void Build_txt(*int spaces_tab, *int char_tab)
{
	//*************************************************//
	//* This function write the text from OCR in file *//
	//*************************************************//

	// Open file
	file = fopen("OCR.txt", 'w');

	// Write in file
	for (int i = 0; i < sizeof(spaces_tab) ; i++)
	{
		// Write latters
		if (i < sizeof(char_tab))
		{
			fprintf(file, "%c", char_tab[i]);
		}
		// Write Space
		if (spaces_tab[i] == '\n' || spaces_tab[i] == ' ')
		{
			fprintf(file, "%c", spaces_tab[i]);			
		}
	}

	// Close file
	fclose(file);
}