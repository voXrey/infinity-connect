#ifndef STRUCTS_H   /* Include guard */
#define STRUCTS_H

typedef long long big_int;

struct array
{
    big_int *a;
    big_int len;
    big_int len_max;
};
typedef struct array array;

struct player
{
    int id;
    char *pseudo;
    const char *token_str;
};
typedef struct player player;

struct token
{
    struct player *player;
    struct token *upper_token;
    struct token *under_token;
    struct token *left_token;
    struct token *right_token;
};
typedef struct token token;

struct column
{
    big_int id;
    struct token *first_token;
    struct token *last_token;
};
typedef struct column column;

struct table
{
    struct column **columns;
    big_int len;
    big_int len_max;
};
typedef struct table table;

struct game
{
    struct table *table;
    struct player *player1;
    struct player *player2;
    struct player *winner;
    array *moves_list;
    big_int rounds;
};
typedef struct game game;

typedef struct menu menu;
typedef struct choice choice;

struct choice
{
    char *phrase;
    void (*f)();
    menu *parent_menu;
    menu *sub_menu;
    menu *redirection;
};

struct menu
{
    char *path; // Beautiful feature thank to Alexandre (github: AJBS-alexo)
    choice **choices;
    int len_choices;
    int len_max;
};
typedef struct menu menu;

#endif // STRUCTS_H