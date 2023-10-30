#ifndef MENUS_H   /* Include guard */
#define MENUS_H

#include "structs.h"

menu *initMenus();
void navigateInMenu(menu *m);
void destroyMenu(menu *m);

#endif // MENUS_H
