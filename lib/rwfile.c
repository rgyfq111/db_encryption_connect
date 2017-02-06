#include "rwfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//#include <openssl/des.h>


//#define ENCRYPT_KEY "uitox123456"


int WriteFile(char* file_name, char* connect)
{
	FILE *fp = fopen(file_name, "w");
	if (fp == NULL)
	{
	    printf("Error opening file!\n");
	    exit(1);
	}

	fprintf(fp, "%s", connect);

	fclose(fp);
	
	return 0;
}

char *ReadFile(char* file_path)
{
	FILE* fp;
	char ch, * buffer;
	long file_length;
	fp = fopen(file_path, "r");
	
	if( fp == NULL)
	{
		exit(EXIT_FAILURE);
	}

	
	fseek(fp, 0, SEEK_END);
	file_length = ftell(fp);
	rewind(fp);	
	buffer = (char*) malloc (sizeof(char) *  file_length);
	if(buffer != NULL)
	{
		fread(buffer, 1, file_length, fp);
	}
	else
	{
		printf("builed mem fail.");
	//	exit(EXIT_FAILURE);
	}
	
	fclose(fp);
	return buffer;
}

