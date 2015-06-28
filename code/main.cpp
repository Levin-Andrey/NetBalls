#include <stdio.h>
#include <Windows.h>

void* allocateMemory(int memorySize) {
	return VirtualAlloc(NULL, memorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

struct MemoryBlock {
	void* base;
	int size;
};

struct AgentIn {
	MemoryBlock workMemory;
	void* updateFn;
};

void simulate(MemoryBlock gameMemory, AgentIn* agents, int agentsCount) {
	int a = 1;
}

#define DEBUG_AGENT_COUNT 8
#define KILOBYTES(Value) ((Value)*1024LL)
#define MEGABYTES(Value) (KILOBYTES(Value)*1024LL)
#define GIGABYTES(Value) (MEGABYTES(Value)*1024LL)
#define TERABYTES(Value) (GIGABYTES(Value)*1024LL)

int main() {
	MemoryBlock gameMemory;
	gameMemory.size = MEGABYTES(100);
	gameMemory.base = allocateMemory(gameMemory.size);
	AgentIn agents[DEBUG_AGENT_COUNT];
	for (int i = 0; i < DEBUG_AGENT_COUNT; i++) {
		agents[i].workMemory.size = MEGABYTES(1);
		agents[i].workMemory.base = allocateMemory(agents[i].workMemory.size);
	}
	simulate(gameMemory, agents, DEBUG_AGENT_COUNT);

	char fileName[MAX_PATH];
	HMODULE currentModule = GetModuleHandle(NULL);
	GetModuleFileName(currentModule, fileName, MAX_PATH);
	
	printf("Hello world!");
	return 42;
}