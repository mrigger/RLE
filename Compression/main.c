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
	
	uint32_t current = 0;
	uint8_t previous = 0;
	uint8_t exit = 1;
	uint8_t i = 0;
	
	FILE * inFile;
	FILE * outFile;
	
	uint8_t filename[512] = {0};
	
	printf("Run-Length Encoding (RLE) compression tool - Version %.2d.%.2d\n", VERSION_MAJOR, VERSION_MINOR);
	printf("Copyright (C) Remi DEBORD. All rights reserved (2016).\n\n"); 
	
	// Exit if no argument is passed
	if(argc == 1)
	{
		return 0;
	}
	
	// Input file
	strcpy(&filename[0], argv[1]);
	
	printf("Opening %s... ", filename);
	
	inFile = fopen(filename, "rb");
	
	// Output file
	strcat(&filename[0], ".rle\0");
	
	outFile = fopen(filename, "wb");
	
	// Files opened ?
	if((inFile != NULL) && (outFile != NULL))
	{		
		// Uncompressed size
		fseek(inFile, 0, SEEK_END);   // non-portable
		uncompressedSize=ftell(inFile);
		rewind(inFile);
    
		printf("OK (%d Bytes)\n\n", uncompressedSize);
		
		// One percent of the uncompressed size
		onePercent = uncompressedSize / 100;
		
		// Prevent zero value
		onePercent = (onePercent == 0) ? 1 : onePercent;
		
		while(exit != 0)
		{
			current = fgetc(inFile);
			
			// End of file reached ?
			if(current == EOF)
			{
				exit = 0;
			}
			
			// First character ?
			if(bytes == 0)
			{
				previous = current;
			}
			
			// Character read != previous character, max length reached, or EOF ?
			if((previous != (uint8_t)current) || (i == UINT8_MAX) || (exit == 0))
			{
				//printf("(%d:%c)", i, previous);
					
				// Length byte
				fputc(i, outFile);
				
				// Data byte
				fputc(previous, outFile);
				
				previous = current;
				i = 1;
				
				compressedSize += 2; // Length byte and data byte
			}
			else
			{
				i++;
			}
			
			// Refresh every 1%
			if((bytes % onePercent) == 0)
			{
				printf("\rCompressing... %.0f%% (%d/%d)", (((float)bytes / uncompressedSize) * 100), bytes, uncompressedSize);
			}
			
			if(exit != 0)
			{
				bytes++;
			}
		}
		
		printf("\rCompressing... %.0f%% (%d/%d)\n\n", (((float)bytes / uncompressedSize) * 100), bytes, uncompressedSize);

		// Compute compression ratio
		compressionRatio = (float)uncompressedSize / compressedSize;
		
		printf("Uncompressed size: %d Bytes\n", uncompressedSize);
		printf("  Compressed size: %d Bytes\n", compressedSize);
		printf("Compression ratio: %.1f (%.0f%%)\n", compressionRatio, (((float)1 / compressionRatio) * 100));
		
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

