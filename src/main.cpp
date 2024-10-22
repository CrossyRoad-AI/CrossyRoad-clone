#include <iostream>
#include <time.h>
#include <stdlib.h>

// Used by shared memory
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <string.h>

#include "game/game.hpp"

HANDLE mapFile;

char memoryMapName[] = "Global\\crossyroad-memorymap";
char* memoryBuffer;

void initSharedMemory() {
    std::cout << "Init file mapping shared memory" << std::endl;

	mapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 5000, memoryMapName);
	if(mapFile == NULL) {
		printf("[!]Could not create file mapping object1 (%d)\n", GetLastError());
		exit(1);
	} else printf("Connected to file mapping\n");

    memoryBuffer = (char *) MapViewOfFile(mapFile, FILE_MAP_ALL_ACCESS, 0, 0, 5000);
	if(memoryBuffer == NULL) {
		printf("[!]Could not map view of file1 (%d)\n", GetLastError());

		CloseHandle(mapFile);
		exit(1);
	} else printf("Got file map view\n");

    // memoryBuffer[0] = 1; // Set ready state
    // strcpy(memoryBuffer, "1"); // Write into shared memory buffer
}

int main() {
    srand(time(NULL));

    // Init shared memory communication
    initSharedMemory();
    
    Game* game = Game::getInstance();
    game->initInputs();

    game->loadModels();

    game->initRun();
    game->loop();

    game->quit();

    return 0;
}