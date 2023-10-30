#ifndef GLOBAL_H   /* Include guard */
#define GLOBAL_H


/* ---------------- GLOBAL & CONSTANTS ---------------- */
// 100 is the max size for a filename

extern const int FIRST_TABLE_LEN ;
extern const int FIRST_MOVES_LEN ;
extern const int FIRST_MENU_LEN;

// DEFAULT SETTINGS (RECOMMENDED)

extern const int DEFAULT_POWER ;
extern const int DEFAULT_WIDTH ;
extern const int DEFAULT_HEIGHT ;
extern const char DEFAULT_P1_TOKEN[100];
extern const char DEFAULT_P2_TOKEN[100];
extern const char DEFAULT_VOID_TOKEN_STR[100];
extern const int DEFAULT_VOID_TOKEN_SIZE;
extern const char DEFAULT_VOID_TOKEN_STR_LAST_LINE_UNIT[100];

// USED
extern int POWER;
extern int WIDTH ;
extern int HEIGHT;
extern char P1_TOKEN[100];
extern char P2_TOKEN[100];
extern char VOID_TOKEN_STR[100];
extern int VOID_TOKEN_SIZE;
extern char VOID_TOKEN_STR_LAST_LINE_UNIT[100];


#endif // GLOBAL_H