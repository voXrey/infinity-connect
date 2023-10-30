#ifndef FUNCTIONS_H   /* Include guard */
#define FUNCTIONS_H

#include <stdbool.h>

void loadGame();
void askFileName(char filename[100]);
bool loadSettings(char *filename);
bool resetSettings(char *filename);
void displaySettings();
void play();

#endif // FUNCTIONS_H

