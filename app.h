#pragma once

#define HSE_VALUE 8000000

#include "stdint.h"

void init(void);

void setSysClk_HSE_8MHz(void);

void setSysClk_84MHz(void);

void mainLoop(void);

//Main loop for processing user input
void commandProcessor(void);

void printInstructions(void);
