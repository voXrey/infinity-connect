#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "structs.h"
#include "colors.h"
#include "utils.h"
#include "enums.h"
#include "global.h"
#include "functions.h"


// Load a settings file
bool loadSettings(char *filename)
{
    FILE *buffer = fopen(filename, "r");
    if (buffer == NULL)
        return false;

    fscanf(buffer, "POWER==%d\n", &POWER);
    fscanf(buffer, "WIDTH==%d\n", &WIDTH);
    fscanf(buffer, "HEIGHT==%d\n", &HEIGHT);
    fscanf(buffer, "P1_TOKEN==%100[^\n]\n", P1_TOKEN);
    fscanf(buffer, "P2_TOKEN==%100[^\n]\n", P2_TOKEN);
    fscanf(buffer, "VOID_TOKEN_STR==%100[^\n]\n", VOID_TOKEN_STR);
    fscanf(buffer, "VOID_TOKEN_SIZE==%d\n", &VOID_TOKEN_SIZE);
    fscanf(buffer, "VOID_TOKEN_STR_LAST_LINE_UNIT==%100[^\n]", VOID_TOKEN_STR_LAST_LINE_UNIT);

    fclose(buffer);
    return true;
}

// Reset settings in a file and load it
bool resetSettings(char *filename)
{
    FILE *buffer = fopen(filename, "w");
    if (buffer == NULL)
        return false;

    fprintf(buffer, "POWER==%d\n", DEFAULT_POWER);
    fprintf(buffer, "WIDTH==%d\n", DEFAULT_WIDTH);
    fprintf(buffer, "HEIGHT==%d\n", DEFAULT_HEIGHT);
    fprintf(buffer, "P1_TOKEN==%s\n", DEFAULT_P1_TOKEN);
    fprintf(buffer, "P2_TOKEN==%s\n", DEFAULT_P2_TOKEN);
    fprintf(buffer, "VOID_TOKEN_STR==%s\n", DEFAULT_VOID_TOKEN_STR);
    fprintf(buffer, "VOID_TOKEN_SIZE==%d\n", DEFAULT_VOID_TOKEN_SIZE);
    fprintf(buffer, "VOID_TOKEN_STR_LAST_LINE_UNIT==%s\n", DEFAULT_VOID_TOKEN_STR_LAST_LINE_UNIT);

    fclose(buffer);
    return true;
}

// Display current settings
void displaySettings()
{
    printf("POWER --------------------------- \"%d\"\n", POWER);
    printf("WIDTH --------------------------- \"%d\"\n", WIDTH);
    printf("HEIGHT -------------------------- \"%d\"\n", HEIGHT);
    printf("P1_TOKEN ------------------------ \"%s\"\n", P1_TOKEN);
    printf("P2_TOKEN ------------------------ \"%s\"\n", P2_TOKEN);
    printf("VOID_TOKEN_STR ------------------ \"%s\"\n", VOID_TOKEN_STR);
    printf("VOID_TOKEN_SIZE ----------------- \"%d\"\n", VOID_TOKEN_SIZE);
    printf("VOID_TOKEN_STR_LAST_LINE_UNIT --- \"%s\"\n", VOID_TOKEN_STR_LAST_LINE_UNIT);
    printf("%sEntry to go back...%s ", KYEL, KNRM);
    getchar();
}

// Create a player on the heap
player *createPlayer(int id, const char *token_str)
{
    player *p = malloc(sizeof(player));

    p->id = id;
    p->pseudo = malloc(sizeof(char) * 50);
    p->token_str = token_str;

    return p;
}

// Delete a player from the heap
void destroyPlayer(player *p)
{
    free(p->pseudo);
    free(p);
}

// Create a token on the heap
token *createToken(player *player)
{
    token *p = malloc(sizeof(token));

    p->player = player;
    p->under_token = NULL;
    p->left_token = NULL;
    p->right_token = NULL;
    p->upper_token = NULL;

    return p;
}

// Delete a token from the heap
void destroyToken(token *t)
{
    free(t);
}

// Create a column on the heap
column *createColumn(big_int id)
{
    column *col = malloc(sizeof(column));

    col->id = id;
    col->first_token = NULL;
    col->last_token = NULL;

    return col;
}

// Delete a column from the heap
void destroyColumn(column *col) 
{
    // Free tokens
    token *current_token = col->last_token;
    token *tmp_token;
    do
    {
        tmp_token = current_token->under_token;
        destroyToken(current_token);
        current_token = tmp_token;
    } while (current_token != NULL);

    // Then free the column
    free(col);
}


// Create a table on the heap
table *createTable(int len)
{
    table *tab = malloc(sizeof(table));

    // Create an list of columns
    tab->columns = malloc(sizeof(column *) * len);
    tab->len = 0;
    tab->len_max = len;

    return tab;
}

// Delete a table from the heap
void destroyTable(table *tab)
{
    // Destroy columns
    for (big_int i = 0; i < tab->len; i++)
        if (tab->columns[i] != NULL)
            destroyColumn(tab->columns[i]);
    
    // Free list of columns
    free(tab->columns);
    // Then free table
    free(tab);
}

// Double size of a table's list of columns
void doubleTableSize(table *tab)
{
    // Update max len
    tab->len_max *= 2;

    // Create new list with new len
    column **new_columns = malloc(sizeof(column *) * tab->len_max);
    // Copy old columns
    for (big_int i = 0; i < tab->len; i++)
        new_columns[i] = tab->columns[i];
    
    // Free old list
    free(tab->columns);
    // Change pointer
    tab->columns = new_columns;
}

// Add a column to a table
void addColumn(column *col, table *tab)
{
    // If table is full, double its size
    if (tab->len == tab->len_max)
        doubleTableSize(tab);
    // Add the column and update table's len
    tab->columns[tab->len] = col;
    tab->len++;
}


// Create an array to store moves on the heap
array *createMovesArray()
{
    // Alloc for the struct
    array *moves_array = malloc(sizeof(array));

    // Alloc for the list
    moves_array->a = malloc(sizeof(big_int) * FIRST_MOVES_LEN);
    moves_array->len = 0;
    moves_array->len_max = FIRST_MOVES_LEN;

    return moves_array;
}

// Delete an array of moves from the heap
void destroyMovesArray(array *moves)
{
    // Free the list, then the array
    free(moves->a);
    free(moves);
}

// Double moves array's list size
void doubleMovesSize(array *moves_array)
{
    moves_array->len_max *= 2; // new size

    big_int *new_a = malloc(sizeof(big_int) * moves_array->len_max);
    for (big_int i = 0; i < moves_array->len; i++)
        new_a[i] = moves_array->a[i]; // copy

    free(moves_array->a);
    moves_array->a = new_a;
}

// Add a move to a moves array
void addMove(big_int move, array *moves)
{
    // Double size of necessary
    if (moves->len == moves->len_max)
        doubleMovesSize(moves);
    // Add move trivialy
    moves->a[moves->len] = move;
    moves->len++;
}


// Create a game on the heap
game *createGame()
{
    game *g = malloc(sizeof(game));

    g->table = createTable(FIRST_TABLE_LEN); // game table
    g->player1 = createPlayer(1, P1_TOKEN); // first player
    g->player2 = createPlayer(2, P2_TOKEN); // second player
    g->winner = NULL; // winner is NULL for the moment
    g->moves_list = createMovesArray(); // array to store moves
    g->rounds = 0; // 0 rounds for the moment

    return g;
}

// Delete a fame from the heap
void destroyGame(game *g)
{
    destroyTable(g->table); // delete game table

    // delete players
    destroyPlayer(g->player1); 
    destroyPlayer(g->player2);

    destroyMovesArray(g->moves_list); // delete moves array

    // then, free game
    free(g);
}


// Count the number of ally tokens in a "row" placed next to it to deduce a victory or not
bool isAligned(token *t)
{
    // Iterations vars
    token *current_token = NULL;
    token *left_token = NULL;
    token *right_token = NULL;
    int i; // counter

    // South - North (gravity)
    i = 0;
    current_token = t;
    while (current_token->under_token != NULL)
    {
        if (current_token->under_token->player->id != t->player->id)
            break;
        current_token = current_token->under_token;
        i++;
    }
    if (i + 1 >= POWER)
        return true;

    // West - East
    i = 0;
    current_token = t;
    while (current_token->left_token != NULL)
    {
        if (current_token->left_token->player->id != t->player->id)
            break;
        current_token = current_token->left_token;
        i++;
    }
    current_token = t;
    while (current_token->right_token != NULL)
    {
        if (current_token->right_token->player->id != t->player->id)
            break;
        current_token = current_token->right_token;
        i++;
    }
    if (i + 1 >= POWER)
        return true;

    // South-West - North-East
    i = 0;
    current_token = t;
    while (current_token->under_token != NULL && current_token->under_token->left_token != NULL)
    {
        if (current_token->under_token->left_token->player->id != t->player->id)
            break;
        current_token = current_token->under_token->left_token;
        i++;
    }
    current_token = t;
    while (current_token->upper_token != NULL && current_token->upper_token->right_token != NULL)
    {
        if (current_token->upper_token->right_token->player->id != t->player->id)
            break;
        current_token = current_token->upper_token->right_token;
        i++;
    }
    if (i + 1 >= POWER)
        return true;

    // North-West - South-East
    i = 0;
    current_token = t;
    while (current_token->upper_token != NULL && current_token->upper_token->left_token != NULL)
    {
        if (current_token->upper_token->left_token->player->id != t->player->id)
            break;
        current_token = current_token->upper_token->left_token;
        i++;
    }

    current_token = t;
    while (current_token->under_token != NULL && current_token->under_token->right_token != NULL)
    {
        if (current_token->under_token->right_token->player->id != t->player->id)
            break;
        current_token = current_token->under_token->right_token;
        i++;
    }

    if (i + 1 >= POWER)
        return true;

    return false;
}


// Get a column by its id. Return a boolean for success and column is stored (if true) in *column
bool getColumn(table *tab, big_int column_id, column **column)
{
    // Itrerate table's columns
    for (big_int i = 0; i < tab->len; i++)
    {   
        // If id is the searched id
        if (tab->columns[i]->id == column_id)
        {
            // Store column
            *column = tab->columns[i];
            return true; // success
        }
    }
    return false; // without success
}


// Add a token by column struct
void addToken_(token *t, column *col, table *tab)
{
    if (col->last_token != NULL) // not the first token
    {
        // update under token
        col->last_token->upper_token = t;

        // update new token and sides tokens
        t->under_token = col->last_token;
        if (col->last_token->left_token != NULL && col->last_token->left_token->upper_token != NULL)
        {
            t->left_token = col->last_token->left_token->upper_token;
            t->left_token->right_token = t;
        }
        if (col->last_token->right_token != NULL && col->last_token->right_token->upper_token != NULL)
        {
            t->right_token = col->last_token->right_token->upper_token;
            t->right_token->left_token = t;
        }
        col->last_token = t;
    }
    else // first token
    {
        col->first_token = t;
        col->last_token = t;

        // update sides (if necessary)
        column *side_col;
        if (getColumn(tab, col->id - 1, &side_col))
        {
            t->left_token = side_col->first_token;
            side_col->first_token->right_token = t;
        }
        if (getColumn(tab, col->id + 1, &side_col))
        {
            t->right_token = side_col->first_token;
            side_col->first_token->left_token = t;
        }
    }
}

// Add a token by column's id
void addToken(token *t, table *tab, big_int column_id)
{
    column *col;
    // create column if necessary
    if (!getColumn(tab, column_id, &col))
    {
        col = createColumn(column_id);
        addColumn(col, tab);
    };
    addToken_(t, col, tab);
}


// Get columns to display
void getColumnsToDisplay(table *tab, big_int columns_id[WIDTH], column *col[WIDTH])
{
    // Init
    for (int i = 0; i < WIDTH; i++)
        col[i] = NULL;

    for (big_int i = 0; i < tab->len; i++)
    {
        // Is in list ?
        for (int j = 0; j < WIDTH; j++)
        {
            // Put to final list
            if (tab->columns[i]->id == columns_id[j])
            {
                col[j] = tab->columns[i];
                break;
            }
        }
    }
}

// Update the display table
void updateDisplayTable(table *game_table, table *display_table, big_int mid_column_id)
{
    // To store ids
    big_int columns_id[WIDTH];

    // Define limit
    big_int first_id = (WIDTH % 2 == 1) ? (mid_column_id - WIDTH / 2) : (mid_column_id - WIDTH / 2 + 1);

    for (big_int i = 0; i < WIDTH; i++)
        columns_id[i] = i + first_id;

    getColumnsToDisplay(game_table, columns_id, display_table->columns);
}

// Create an array 2D to store tokens' pointers to display, on the heap
token ***createDisplayTokensArray()
{
    // Trivial array 2D
    token ***a = malloc(sizeof(token **) * HEIGHT);
    for (int i = 0; i < HEIGHT; i++)
    {
        a[i] = malloc(sizeof(token *) * WIDTH);
        for (int j = 0; j < WIDTH; j++)
        {
            a[i][j] = NULL; // init to NULL
        }
    }
    return a;
}

// Delete display-tokens-array2D from the heap
void destroyDisplayTokensArray(token ***a)
{
    // trivial
    for (int i = 0; i < HEIGHT; i++)
    {
        free(a[i]);
    }
    free(a);
}

// Update tokens to display in the display-array2D
void updateDisplayArray2D(table *display_table, token ***tokens_array, big_int bottom_row)
{
    token *current_token;

    // columns
    for (int i = 0; i < WIDTH; i++)
    {
        // token* current_token = (display_table->columns[i] != NULL) ? (display_table->columns[i]->first_token) : NULL;
        if (display_table->columns[i] == NULL)
            current_token = NULL;
        else
        {
            current_token = display_table->columns[i]->first_token;
            for (big_int j = 0; j < bottom_row; j++)
                if (current_token != NULL)
                    current_token = current_token->upper_token;
        }
        // rows
        for (int j = 0; j < HEIGHT; j++)
        {
            if (current_token == NULL)
                tokens_array[j][i] = NULL;
            else
            {
                // adapt format to display easier
                tokens_array[j][i] = current_token;
                current_token = current_token->upper_token;
            }
        }
    }
}

// Print an unit for last line
void printLastLineUnit()
{
    for (int i = 0; i < VOID_TOKEN_SIZE; i++)
        printf("%s", VOID_TOKEN_STR_LAST_LINE_UNIT);
    printf(" ");
}

// Display final grid
void displayGrid(token ***tokens_array, big_int bottom_row)
{
    // rows
    for (int i = HEIGHT - 1; i >= 0; i--)
    {
        // columns
        for (int j = 0; j < WIDTH; j++)
        {
            // no token = default void token
            if (tokens_array[i][j] == NULL)
            {
                if (i != 0)
                    printf("%s ", VOID_TOKEN_STR);
                else
                    printLastLineUnit();
            }
            else
                printf("%s ", tokens_array[i][j]->player->token_str);
        }
        // display some rows' ids
        if (i % 5 == 0)
            printf("|←-Row %lld\n", bottom_row + i);
        else
            printf("\n");
    }
}

// Display an arrow to highlight the middle column
void displayMidArrow(big_int mid_column_id)
{
    big_int lim = (WIDTH % 2 == 1) ? (WIDTH / 2) : (WIDTH / 2 - 1); // adaptation to width parity (mid on the left)
    for (int i = 0; i != lim; i++) // spaces
        {
            for (int j = 0; j < VOID_TOKEN_SIZE; j++) printf(" ");
            printf(" ");
        }
    printf("↑\n"); // arrow
    for (int i = 0; i != lim; i++) // spaces
        {
            for (int j = 0; j < VOID_TOKEN_SIZE; j++) printf(" ");
            printf(" ");
        }
    printf("Col %lld\n", mid_column_id); // id
}

// Display all
void display(table *game_table, table *display_table, big_int mid_column_id, big_int bottom_row_id, token ***tokens_array)
{
    // Avoid bugs
    if (bottom_row_id < 0)
        bottom_row_id = 0;

    // Display preparation
    updateDisplayTable(game_table, display_table, mid_column_id);
    updateDisplayArray2D(display_table, tokens_array, bottom_row_id);

    // Display
    system("clear");
    displayGrid(tokens_array, bottom_row_id);
    displayMidArrow(mid_column_id);
}


// Display player's victory
void printVictory(player *p)
{
    printf("%s----------------- CONGRATULATIONS %s%s%s! -----------------\n%s\\(^-^)/%s You won the game! %s<(^_^)>%s\n", KYEL, KCYN, p->pseudo, KYEL, KGRN, KNRM, KGRN, KNRM);
}

// Ask pseudos to players
void askPseudos(game *g)
{
    printf("Player 1, what's your pseudo? ");
    scanf("%50s", g->player1->pseudo);
    flushStdin();
    printf("Player 2, what's your pseudo? ");
    scanf("%50s", g->player2->pseudo);
    flushStdin();
}

// Ask for saving
response askForSaving()
{
    char response;
    int stop;

    printf("Do you want to save the game? [%sY%s/%sn%s] ", KGRN, KNRM, KRED, KNRM);
    while (true)
    {
        stop = scanf("%c", &response);
        if (stop < 1)
        {
            printf("Use 'Y' for yes or 'n' for no: ");
            continue;
        }
        switch (response)
        {
        case 'Y':
        case 'y':
            return YES;
        case 'N':
        case 'n':
            return NO;
        default:
        {
            printf("Use 'Y' for yes or 'n' for no: ");
            continue;
        }
        }
    }
}

// Ask for filename
void askFileName(char filename[100])
{
    printf("What's the name of the file? ");
    scanf("%100s", filename);
    flushStdin();
}

// Ask to player what he wants to do
action askActionToPlayer(player *p, big_int *column_id, big_int *row_id)
{
    printf("It's your turn %s%s%s,\nWhat do you want to do?\n> ", KCYN, p->pseudo, KNRM);

    int stop = 0;
    char action;

    while (true)
    {
        action = getchar(); // action type is determined by a char

        if (action == 'p') // play
        {
            while (true)
            {
                stop = scanf("%lld", column_id);
                flushStdin();
                if (stop == 0)
                    printf("%s", "Enter the column's id\n> ");
                else
                    break;
            }
            return PLAY;
        }
        else if (action == 'f') // focus on a grid part
        {
            while (true)
            {
                // f0 = f0:0
                stop = scanf("%lld", column_id);
                if (stop == 0)
                {
                    printf("%s", "Enter the mid column's id\n> ");
                    flushStdin();
                    continue;
                }
                stop = scanf(":%lld", row_id);
                if (stop == 0)
                {
                    *row_id = 0;
                }
                flushStdin();
                break;
            }
            return FOCUS;
        }
        else if (action == 's') // save
        {
            flushStdin();
            return SAVE;
        }
        else if (action == 'q') // quit
        {
            flushStdin();
            return QUIT;
        }
        else
        {
            flushStdin();
            printf("Please use the format in the user guide!\n> ");
        }
    }
}


// Determine which player has to play using parity of rounds
player *whichPlayer(game *g)
{
    return (g->rounds % 2 == 0) ? g->player1 : g->player2;
}


// Save the game
bool saveGame(game *g, char *filename)
{
    FILE *buffer = fopen(filename, "w");
    if (NULL == buffer)
        return false;

    fprintf(buffer, "%s\n%s\n", g->player1->pseudo, g->player2->pseudo); // Pseudos
    for (big_int i = 0; i < g->moves_list->len; i++)
        fprintf(buffer, "%lld\n", g->moves_list->a[i]); // Moves

    fclose(buffer);
    return true;
}

// Play a game
void playGame(game *g)
{
    // Display vars
    table *display_table = createTable(WIDTH);
    token ***tokens_array = createDisplayTokensArray();

    big_int column_id = 0;
    big_int row_id = 0;

    // Save vars
    char filename[100];
    bool success = false;
    response player_response;

    // Iterations vars
    player *current_player = NULL;
    token *new_token = NULL;
    action player_action;

    while (g->winner == NULL)
    {
        current_player = whichPlayer(g);
        do
        {
            display(g->table, display_table, column_id, row_id, tokens_array);
            player_action = askActionToPlayer(current_player, &column_id, &row_id);

            if (player_action == SAVE)
            {
                askFileName(filename);
                success = saveGame(g, filename);
                printf("%s", (success) ? "Game saved!\n" : "Game couldn't be saved...\n");
                sleep(2);
            }
            else if (player_action == QUIT)
            {
                return;
            }

        } while (player_action != PLAY); // Until placing a token

        new_token = createToken(current_player);
        addToken(new_token, g->table, column_id);
        addMove(column_id, g->moves_list);

        g->rounds++;

        if (isAligned(new_token))
        {
            g->winner = current_player;
            break;
        }
    }

    // Current player won
    display(g->table, display_table, column_id, row_id, tokens_array);
    printVictory(g->winner);

    // Free display utilities
    destroyDisplayTokensArray(tokens_array);
    destroyTable(display_table);

    // Save ?
    player_response = askForSaving();
    if (player_response == YES)
    {
        do
        {
            askFileName(filename);
            success = saveGame(g, filename);
            if (!success)
                printf("An error has occured during saving, let's retry:\n");
        } while (!success);
        printf("Game saved!\n");
    }
}

// Load game with filename and game
bool loadGame_(char *filename, game *g)
{
    big_int move;
    token *new_token;
    player *current_player;

    // file
    FILE *buffer = fopen(filename, "r");
    if (NULL == buffer)
        return false;

    // get pseudos
    fscanf(buffer, "%50s", g->player1->pseudo);
    fscanf(buffer, "%50s", g->player2->pseudo);

    // iterate game's moves
    int stop = fscanf(buffer, "%lld", &move);
    while (stop == 1)
    {

        current_player = whichPlayer(g);
        new_token = createToken(current_player);
        addToken(new_token, g->table, move);
        addMove(move, g->moves_list);

        g->rounds++;
        stop = fscanf(buffer, "%lld", &move);
    }
    fclose(buffer);

    // victory for last token placed only
    if (isAligned(new_token))
        g->winner = new_token->player;

    return true;
}

// Load game from scratch
void loadGame()
{
    bool success = false;
    char filename[100];

    // Create game
    game *g = createGame();

    // Load game
    askFileName(filename);
    success = loadGame_(filename, g);

    if (success)
        playGame(g); // play if game loaded correctly
    else
    {
        printf("File can't be oppened, returning to main menu...\n");
        sleep(2);
    }

    // Delete game
    destroyGame(g);
}

// Play a new game
void play()
{
    game *g = createGame();
    askPseudos(g);
    playGame(g);
    destroyGame(g);
}
