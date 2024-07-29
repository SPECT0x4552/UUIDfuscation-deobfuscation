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
		return EXIT_FAILURE;
	}

	// Get the real size of the shellcode which is the number of UUID strings * 16
	// That is because every UUID string consists of 16 raw bytes

	sBuffSize = NmbrOfElements * 16;

	// Allocate memory to a buffer that will hold the deobfuscated shellcode
	pBuffer = (PBYTE)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sBuffSize);
	if (pBuffer == NULL) {
		printf("%s HeapAlloc Failed With Error : %ld\n", e, GetLastError());
		return EXIT_FAILURE;
	}

	// Set TmpBuffer equal to pBuffer 
	TmpBuffer = pBuffer;

	// Loop through all the UUID strings in the UuidArray[]
	for (int i = 0; i < NmbrOfElements; i++) {

		// Deobfuscate one UUID string at a time
	}


}


int main(void) {

	return 0;
}