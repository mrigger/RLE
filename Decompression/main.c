#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> // uint8_t 
#include <string.h> // strcpy

#define VERSION_MAJOR 1
#define VERSION_MINOR 0

int main(int argc, char *argv[])
{
	uint32_t uncompressedSize = 0;
	uint32_t compressedSize = 0;
	uint32_t onePercent = 0;
	uint32_t bytes = 0;
	
	float compressionRatio = 0;
	
	uint32_t length = 0;
	uint32_t data = 0;
	uint8_t exit = 1;
	uint8_t i = 0;
	
	FILE * inFile;
	FILE * outFile;
	
	uint8_t filename[512] = {0};
	
	printf("Run-Length Encoding (RLE) uncompression tool - Version %.2d.%.2d\n", VERSION_MAJOR, VERSION_MINOR);
	printf("Copyright (C) Remi DEBORD. All rights reserved (2016)).\n\n"); 
	
	// Exit if no argument is passed
	if(argv[1] == NULL)
	{
		return 0;
	}
	
	// Input file
	strcpy(&filename[0], argv[1]);
	
	printf("Opening %s... ", filename);
	
	inFile = fopen(filename, "rb");
	
	// Output file
	strcat(&filename[0], ".out\0");
	
	outFile = fopen(filename, "wb");
	
	// Files opened ?
	if((inFile != NULL) && (outFile != NULL))
	{		
		// Compressed size
		fseek(inFile, 0, SEEK_END);   // non-portable
		compressedSize=ftell(inFile);
		rewind(inFile);
    
		printf("OK (%d Bytes)\n\n", compressedSize);
		
		// One percent of the compressed size
		onePercent = compressedSize / 100;
		
		// Prevent zero value
		onePercent = (onePercent == 0) ? 1 : onePercent;
		
		while(exit != 0)
		{
			length = fgetc(inFile);
			
			// End of file reached ?
			if(length == EOF)
			{
				exit = 0;
			}
			else
			{
				data = fgetc(inFile);
				
				if(data != EOF)
				{
					// Store data
					for(i = 0; i < length; i++)
					{
						fputc(data, outFile);
						
						uncompressedSize++;
					}
				}
				else
				{
					exit = 0;
				}
			}
			
			// Refresh every 1%
			if((bytes % onePercent) == 0)
			{
				printf("\rUncompressing... %.0f%% (%d/%d)", (((float)bytes / compressedSize) * 100), bytes, compressedSize);
			}
			
			bytes++;
		}
		
		// Last refresh
		printf("\rUncompressing... %.0f%% (%d/%d)\n\n", (((float)bytes / compressedSize) * 100), bytes, compressedSize);
		
		printf("  Compressed size: %d Bytes\n", compressedSize);
		printf("Uncompressed size: %d Bytes\n", uncompressedSize);
		
		fclose(inFile);
		fclose(outFile);
	}
	else
	{
		printf("NOK\n");
	}
	
	//getchar();
	
	return 0;
}

