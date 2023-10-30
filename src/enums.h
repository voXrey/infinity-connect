#ifndef ENUMS_H   /* Include guard */
#define ENUMS_H

enum action
{
    PLAY,
    FOCUS,
    NEXT,
    LAST,
    SAVE,
    QUIT
};
typedef enum action action;

enum response
{
    NO,
    YES
};
typedef enum response response;

#endif // ENUMS_H