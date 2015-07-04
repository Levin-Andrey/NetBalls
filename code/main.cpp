#include <stdio.h>
#include <Windows.h>

void* allocateMemory(int memorySize) {
	return VirtualAlloc(NULL, memorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

struct MemoryBlock {
	void* base;
	int size;
	bool initialized;
};

struct AgentOut {
	bool up;
	bool right;
	bool down;
	bool left;
};

#define UPDATE_FN(name) AgentOut name()
typedef UPDATE_FN(update_fn_t);

struct AgentIn {
	MemoryBlock workMemory;
	update_fn_t* updateFn;
};

UPDATE_FN(test_updateFn) {
	AgentOut result = {};
	result.right = 1;
	return result;
}

enum FloorType {FLOOR, ROAD};
enum ObjectType {FRUIT, AGENT, EMPTY};

struct Tile {
	FloorType typeFloor;
	ObjectType type;
};

struct Point {
	int x;
	int y;
};

enum AgentType { PLAYER, AI };
struct Agent {
	AgentType type;
	Point pos;
};


#define MAP_SIZE 64
#define DEBUG_AGENT_COUNT 8

struct SimulationState {
	Tile map[MAP_SIZE][MAP_SIZE];
	Agent agents[DEBUG_AGENT_COUNT];
};

void simulateStateDump(SimulationState* simState) {
	for (int i = 0; i < MAP_SIZE; i++) {
		for (int j = 0; j < MAP_SIZE; j++) {
			if (simState->map[i][j].type == EMPTY) printf("_");
			if (simState->map[i][j].type == FRUIT) printf("f");
			if (simState->map[i][j].type == AGENT) printf("a");
		}
		printf("\n");
	}
}

Point getNewPos(Tile (*map)[MAP_SIZE], Point oldPos, AgentOut* action) {
	Point result = oldPos;
	int x, y;
	if (action->right && oldPos.x < MAP_SIZE) {
		result.x++;
	}
	if (action->left && oldPos.x > 0) {
		result.x--;
	}
	if (action->down && oldPos.y < MAP_SIZE) {
		result.y++;
	}
	if (action->up && oldPos.y > 0) {
		result.y--;
	}
	if (map[result.x][result.y].type == AGENT) {
		result = oldPos;
	}
	return result;
}

void simulate(MemoryBlock simMemory, AgentIn* agents) {
	int a = 1;
	SimulationState* simState = (SimulationState*)simMemory.base;
	// This is initialisation
	if (!simMemory.initialized) {
		int agentNum = 0;
		for (int i = 0; i < MAP_SIZE; i++) {
			for (int j = 0; j < MAP_SIZE; j++) {
				simState->map[i][j].typeFloor = FLOOR;
				if (agentNum < DEBUG_AGENT_COUNT && i == j) {
					simState->agents[agentNum].pos.x = j;
					simState->agents[agentNum].pos.y = i;
					simState->map[i][j].type = AGENT;
					agentNum++;
				} else if (i == 2 * j && i != 0) {
					simState->map[i][j].type = FRUIT;
				} else {
					simState->map[i][j].type = EMPTY;
				}
			}
		}
		simMemory.initialized = true;
	}
	simulateStateDump(simState);
	// Update agents & apply actions
	AgentOut action;
	Agent* agent;
	for (int i = 0; i < DEBUG_AGENT_COUNT; i++) {
		action = agents[i].updateFn();
		agent = simState->agents + i;
		Point pos = agent->pos;
		Point newPos = getNewPos(simState->map, pos, &action);
		agent->pos = newPos;
		simState->map[newPos.x][newPos.y].type = AGENT;
		simState->map[pos.x][pos.y].type = EMPTY;
	}
	simulateStateDump(simState);
}

#define SIM_MEMORY_LIMIT 100
#define KILOBYTES(Value) ((Value)*1024LL)
#define MEGABYTES(Value) (KILOBYTES(Value)*1024LL)
#define GIGABYTES(Value) (MEGABYTES(Value)*1024LL)
#define TERABYTES(Value) (GIGABYTES(Value)*1024LL)

int main() {
	MemoryBlock simMemory = {};
	simMemory.size = MEGABYTES(SIM_MEMORY_LIMIT);
	simMemory.base = allocateMemory(simMemory.size);
	AgentIn agents[DEBUG_AGENT_COUNT];
	for (int i = 0; i < DEBUG_AGENT_COUNT; i++) {
		agents[i].workMemory.size = MEGABYTES(1);
		agents[i].workMemory.base = allocateMemory(agents[i].workMemory.size);
		agents[i].updateFn = test_updateFn;
	}
	simulate(simMemory, agents);

	char fileName[MAX_PATH];
	HMODULE currentModule = GetModuleHandle(NULL);
	GetModuleFileName(currentModule, fileName, MAX_PATH);
	
	printf("Hello world!");
	return 42;
}