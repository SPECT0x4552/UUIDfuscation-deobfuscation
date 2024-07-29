#include <Windows.h>
#include <stdio.h>

#pragma warning(disable:4996)

// Function takes in 16 raw bytes and returns them in a UUID format

char* GenerateUUid(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p) {

	// Each UUID segment is 32 bytes (bits?)
	char Output0[32], Output1[32], Output2[32], Output3[32];


	// UUID itself consisting of the 4 segments that I declared below
	char result[128];

	// Generate Output0 from the first 4 bytes
	sprintf(Output0, "%0.2X%0.2X%0.2X%0.2X", a, b, c, d);

	// Generate Output1 from the second 4 bytes
	sprintf(Output1, "%0.2X%0.2X%0.2X%0.2X", e, f, g, h);
	sprintf(Output2, "%0.2X%0.2X%0.2X%0.2X", i, j, k, l);
	sprintf(Output3, "%0.2X%0.2X%0.2X%0.2X", m, n, o, p);

	// Combine all of the outputs above to generate the UUID
	sprintf(result, "%s-%s-%s%s", Output0, Output1, Output2, Output3);

	return (char*)result;
}


// This the function that generates the UUID output representation of the Shellcode
// Requires a pointer(or base address) to the buffer where the shellcode is stored and the size of that buffer

BOOL GenerateUuidOutput(unsigned char* pShellcode, SIZE_T ShellcodeSize) {

	// If the Shellcode buffer is NULL or the size of the Shellcode buffer is NULL,
	// or if the Shellcode is not a multiple of 16, return FALSE and exit.
	if (pShellcode == NULL || ShellcodeSize == NULL || ShellcodeSize % 16 != 0) {
		return FALSE;
	}

	printf("char* UuidArray[%d] = { \n\t", (int)(ShellcodeSize % 16));

	// Read one shellcode byte at a time, when we have read 16 shellcode bytes, begin generating the UUID
	// 'c' is used to store the number of bytes read
	// We start at 16 bytes by default

	int c = 16, counter = 0; 
	char* UUID = NULL;

	for (int i = 0; i < ShellcodeSize; i++) {
		if (c == 16) {
			counter++;

			UUID = GenerateUUid(pShellcode[i], pShellcode[i + 1], pShellcode[i + 2], pShellcode[i + 3], pShellcode[i + 4], pShellcode[i + 5], pShellcode[i + 6],
				pShellcode[i + 7], pShellcode[i + 8], pShellcode[i + 9], pShellcode[i + 10], pShellcode[i + 11], pShellcode[i + 12],
				pShellcode[i + 13], pShellcode[i + 14], pShellcode[i + 15]);

			if (i == ShellcodeSize - 16) {

				// This prints out the last UUID string since the total bytes we have are i+15
				printf("\"%s\"", UUID);
			}
			else {
				printf("\"%s\"", UUID);
			}
			c = 1;

			// Beautify the output of the console
			if (counter % 3 == 0) {
				printf("\n\t");
			}

			c++;
		}
	}


	printf("\n}\n\t");
	return TRUE;
}


int main(void) {


	return 0;
}