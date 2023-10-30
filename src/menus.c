#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "global.h"
#include "colors.h"
#include "utils.h"
#include "structs.h"
#include "functions.h"
#include "menus.h"


// Create a choice for a menu, on the heap
choice *createChoice(char *phrase, void (*f)(), menu *sub_menu, menu *redirection)
{
    choice *c = malloc(sizeof(choice));
    c->phrase = phrase;
    c->f = f;
    c->sub_menu = sub_menu;
    c->redirection = redirection;
    return c;
}

// Change redirection of a choice
void changeRedirection(choice *c, menu *redirection)
{
    c->redirection = redirection;
}

// Delete a choice from the heap
void destroyChoice(choice *c)
{
    free(c);
}

// Create a menu on the heap
menu *createMenu(char *path)
{
    menu *m = malloc(sizeof(menu));
    m->path = path;
    m->len_choices = 0;
    m->choices = malloc(sizeof(choice *) * FIRST_MENU_LEN);
    m->len_max = FIRST_MENU_LEN;
    return m;
}

// Delete a menu from the heap
void destroyMenu(menu *m)
{
    // Delete sub menus
    for (int i = 0; i < m->len_choices; i++)
    {
        if (m->choices[i]->sub_menu != NULL && m->choices[i]->sub_menu != m) // protect against infinity loop
            destroyMenu(m->choices[i]->sub_menu);
        destroyChoice(m->choices[i]);
    }

    free(m->choices);
    free(m);
}

// Double menu's choices' list size
void doubleSizeMenu(menu *m)
{
    m->len_max *= 2;
    choice **choices = malloc(sizeof(choice *) * m->len_max);
    for (int i = 0; i < m->len_choices; i++)
        choices[i] = m->choices[i];
}

// Add a choice to a menu
void addChoice(choice *c, menu *m)
{
    // Double size of necessary
    if (m->len_choices == m->len_max)
        doubleSizeMenu(m);

    // Add to menu
    m->choices[m->len_choices] = c;
    m->len_choices++;

    // Update choice
    c->parent_menu = m;
}

// Display a menu
void displayMenu(menu *m)
{
    system("clear");
    printf(" -~-~-~-~-~-~- [ %s%s%s ] -~-~-~-~-~-~- \n", KYEL, m->path, KNRM);
    for (int i = 0; i < m->len_choices; i++)
        printf("[%s%d%s] : %s\n", KYEL, i, KNRM, m->choices[i]->phrase);
}

// Ask the user's choice for a menu
int askMenuChoice(int len)
{
    int stop, n;
    int max_choice = len - 1;

    printf("> ");
    while (true)
    {
        stop = scanf("%d", &n);
        flushStdin();
        if ((stop == 0) || (n < 0) || (n > max_choice))
            printf("Please choose correctly\n> ");
        else
            break;
    }
    return n;
}

// Starts the interaction with a menu
menu *interactionMenu(menu *m)
{
    displayMenu(m);
    choice *c = m->choices[askMenuChoice(m->len_choices)];
    c->f(c); // effect
    return c->redirection; // return next destination
}

// Call interactionMenu until there is no more menu 
void navigateInMenu(menu *m)
{
    while (m != NULL)
        m = interactionMenu(m);
}

// Choice functions
// They allow you to correctly call the program functions
void choiceNothing(choice *c) {}
void choiceLeave(choice *c)
{
    printf("Good by! ~\\(>-<)/~\n");
    changeRedirection(c, NULL);
}
void choicePlay(choice *c) { play(); }
void choiceLoadGame(choice *c) { loadGame(); }
void choice418(choice *c)
{
    system("clear");
    printf("I'm a teapot\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣤\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣘⣿⣿⣀⡀\n");
    printf("⠀⠀⠀⣀⣀⡀⠀⠀⠀⢀⣀⠘⠛⠛⠛⠛⠛⠛⠁⣀\n");
    printf("⠀⢠⡿⠋⠉⠛⠃⣠⣤⣈⣉⡛⠛⠛⠛⠛⠛⠛⢛⣉⣁⣤⣄⠀⠀⣾⣿⡿⠗\n");
    printf("⠀⢸⡇⠀⠀⠀⣰⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣆⠀⣿⣿\n");
    printf("⠀⢸⣇⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⢉⣉⣠⣿⣿⡀\n");
    printf("⠀⠀⠙⠷⡆⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠃⢰⣿⣿⣿⣿⣿⡇\n");
    printf("⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡄⠸⣿⣿⣿⣿⠟\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠄⠈⠉⠁\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⢄⣉⠉⠛⠛⠛⠛⠛⠋⢉⣉⡠\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⠿⠿⠿⠿⠿⠿⠛⠋\n");
    sleep(3);
}
void choicePokemonEmulator(choice *c)
{
    char to_print[70];
    int i = 1;

    FILE *buffer = fopen("special.txt", "r");
    if (buffer != NULL)
    {
        do
        {
            fgets(to_print, 70, buffer);
            printf("%70s", to_print);
            if (i % 26 == 0)
            {
                usleep(33000);
                system("clear");
            }
            i++;
        } while (i < 20018);
        fclose(buffer);
        system("clear");
        printf("But good by Nadine! ;(\n");
        sleep(1);
        return;
    }
}
void choiceLoadSettings(choice *c)
{
    bool success;
    char filename[100];

    askFileName(filename);
    success = loadSettings(filename);
    printf("%s", (success) ? "Settings loaded!\n" : "An error has occured...\n");
    sleep(1);
}
void choiceResetSettings(choice *c)
{
    bool success;
    char filename[100];

    askFileName(filename);
    success = resetSettings(filename);
    if (success)
    {
        success = loadSettings(filename);
    }
    printf("%s", (success) ? "Settings reset!\n" : "An error has occured...\n");
    sleep(2);
}
void choiceShowSettings(choice *c)
{
    displaySettings();
}


// Init all menu and return the main menu
menu *initMenus()
{
    // Menus
    menu *main_menu = createMenu("~/main");
    menu *sub_menu_others = createMenu("~/main/others");
    menu *sub_menu_settings = createMenu("~/main/others/settings");

    // Choices main menu
    addChoice(createChoice("Start normal game", choicePlay, NULL, main_menu), main_menu);
    addChoice(createChoice("Load game", choiceLoadGame, NULL, main_menu), main_menu);
    addChoice(createChoice("Others", choiceNothing, sub_menu_others, sub_menu_others), main_menu);
    addChoice(createChoice("Exit game", choiceLeave, NULL, main_menu), main_menu);

    // Choices submenu others
    addChoice(createChoice("Settings", choiceNothing, sub_menu_settings, sub_menu_settings), sub_menu_others);
    addChoice(createChoice("\x1B[31mError 418\x1B[0m", choice418, NULL, sub_menu_others), sub_menu_others);
    addChoice(createChoice("\x1B[35mPokemon Emulator\x1B[0m", choicePokemonEmulator, NULL, sub_menu_others), sub_menu_others);
    addChoice(createChoice("Back", choiceNothing, NULL, main_menu), sub_menu_others);

    // Choices submenu settings
    addChoice(createChoice("Show settings", choiceShowSettings, NULL, sub_menu_settings), sub_menu_settings);
    addChoice(createChoice("Load settings", choiceLoadSettings, NULL, sub_menu_settings), sub_menu_settings);
    addChoice(createChoice("Reset settings", choiceResetSettings, NULL, sub_menu_settings), sub_menu_settings);
    addChoice(createChoice("Back", choiceNothing, NULL, sub_menu_others), sub_menu_settings);

    return main_menu;
}
