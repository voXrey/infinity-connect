#include "structs.h"
#include "functions.h"
#include "menus.h"

int main()
{
    loadSettings("settings.txt"); // load current setting if possible

    // Menus
    menu *main_menu = initMenus();
    navigateInMenu(main_menu); // start navigation and more
    destroyMenu(main_menu); // last action of the program
}
