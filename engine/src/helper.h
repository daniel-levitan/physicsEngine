#include <stdint.h>

#define GRAVITY 9.81

typedef struct {
    int x;
    int y;
    int leftDown;
    int rightDown;
    int buttons;
} Mouse;