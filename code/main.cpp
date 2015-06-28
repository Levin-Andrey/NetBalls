#include <stdio.h>
#include <Windows.h>

void* allocateMemory(int memorySize) {
	return VirtualAlloc(NULL, memorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

int main() {
	HMODULE currentModule = GetModuleHandle(NULL);
	void* memoryStart = allocateMemory(1024 * 1024 * 1024);
	printf("Hello world!");
	return 42;
}