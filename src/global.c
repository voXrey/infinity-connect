#include "global.h"

/* ---------------- GLOBAL & CONSTANTS ---------------- */
// 100 is the max size for a filename

const int FIRST_TABLE_LEN = 2;
const int FIRST_MOVES_LEN = 8;
const int FIRST_MENU_LEN = 4;

// DEFAULT SETTINGS (RECOMMENDED)

const int DEFAULT_POWER = 5;
const int DEFAULT_WIDTH = 15;
const int DEFAULT_HEIGHT = 20;
const char DEFAULT_P1_TOKEN[100] = "🔵";
const char DEFAULT_P2_TOKEN[100] = "🔴";
const char DEFAULT_VOID_TOKEN_STR[100] = "  ";
const int DEFAULT_VOID_TOKEN_SIZE = 2;
const char DEFAULT_VOID_TOKEN_STR_LAST_LINE_UNIT[100] = "_";

// USED
int POWER = 5;
int WIDTH = 15;
int HEIGHT = 20;
char P1_TOKEN[100] = "🔵";
char P2_TOKEN[100] = "🔴";
char VOID_TOKEN_STR[100] = "  ";
int VOID_TOKEN_SIZE = 2;
char VOID_TOKEN_STR_LAST_LINE_UNIT[100] = "_";