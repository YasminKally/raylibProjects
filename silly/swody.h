#include <raylib.h>

typedef struct Player {
    Rectangle body;
    Vector2 velocity;
    int jumped;
    int onLand;
    int slowTime;
    int coyoteTime;
} Player;

typedef struct Input {
    int left;
    int right;
    int up;
    int down;
    int gamepad;
} Input;

void movePlayer(Player* player, Input input, Rectangle surfaces[]);
int moveX(float amount, Rectangle* player, Rectangle surfaces[]);
void moveY(float* amount, Rectangle* player, Rectangle surfaces[], int* jumped, int* onLand);
int collision(Rectangle r, Rectangle surfaces[]);
int playerCollision(Player* ref, Player* other, Rectangle surfaces[]);
int reset(Player* p1, Player* p2);