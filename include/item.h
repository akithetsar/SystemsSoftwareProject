// item.h
#ifndef ITEM_H
#define ITEM_H

typedef enum ITEM_KIND{
    ITEM_SYM = 0,
    ITEM_LITERAL = 1

} ITEM_KIND;

typedef struct {
    ITEM_KIND kind;
    int value;
    char* sym;
} Item;

#endif