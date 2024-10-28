#include <iostream>
#include <time.h>
#include <stdlib.h>

// Used by shared memory
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string.h>

#include "game/constants/constants.h"

#include "game/game.hpp"

HANDLE mapFile;
char* memoryBuffer;

void initSharedMemory() {
    mapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE | SEC_NOCACHE | SEC_COMMIT, 0, 5000, MEMORY_MAP_NAME);
	if(mapFile == NULL) {
		printf("[!]Could not create file mapping object1 (%d), remember to run this program as administrator \n", GetLastError());
		exit(1);
	}

    memoryBuffer = (char *) MapViewOfFile(mapFile, FILE_MAP_ALL_ACCESS, 0, 0, 5000);
	if(memoryBuffer == NULL) {
		printf("[!]Could not map view of file1 (%d)\n", GetLastError());

		CloseHandle(mapFile);
		exit(1);
	}
}

int main() {
    srand(time(NULL));

    initSharedMemory();
    
    Game* game = Game::getInstance();
    game->initInputs();

    game->loadModels();
    game->initRun();

    game->loop();
    game->quit();

    return 0;
}