#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define W 80
#define H 25
#define BIRD 'O'
#define PIPE '|'
#define EMPTY ' '
#define SPACE_GAP 5

typedef struct {
    int x, y;
    int vy;
} Bird;

typedef struct {
    int x;
    int gapY;
    int passed;
} Pipe;

typedef struct {
    Bird bird;
    Pipe pipes[10];
    int score;
    int level;
    int gameOver;
    int speed;
    int frameCounter;
} Game;

Game game;

void initGame() {
    game.bird.x = 10;
    game.bird.y = H / 2;
    game.bird.vy = 0;
    game.score = 0;
    game.level = 1;
    game.gameOver = 0;
    game.speed = 2;
    game.frameCounter = 0;

    for (int i = 0; i < 10; i++) {
        game.pipes[i].x = W + i * 20;
        game.pipes[i].gapY = 5 + rand() % 15;
        game.pipes[i].passed = 0;
    }
}

void updateLevel() {
    game.level = 1 + (game.score / 3);
    game.speed = 2 + (game.level / 2);
}

void updateGame() {
    if (game.gameOver) return;

    game.frameCounter++;

    game.bird.vy++;
    game.bird.y += game.bird.vy;

    if (game.bird.y < 0) game.bird.y = 0;
    if (game.bird.y >= H - 1) {
        game.gameOver = 1;
    }

    if (game.frameCounter % game.speed == 0) {
        for (int i = 0; i < 10; i++) {
            game.pipes[i].x--;

            if (game.pipes[i].x < -2) {
                game.pipes[i].x = W;
                game.pipes[i].gapY = 5 + rand() % 12;
                game.pipes[i].passed = 0;
            }

            if (game.pipes[i].x == game.bird.x) {
                if (game.bird.y < game.pipes[i].gapY || game.bird.y >= game.pipes[i].gapY + SPACE_GAP) {
                    game.gameOver = 1;
                }
            }

            if (!game.pipes[i].passed && game.pipes[i].x < game.bird.x) {
                game.pipes[i].passed = 1;
                game.score++;
                updateLevel();
            }
        }
    }
}

void drawGame() {
    system("cls");

    char screen[H][W + 1];
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            screen[y][x] = EMPTY;
        }
        screen[y][W] = '\0';
    }

    screen[game.bird.y][game.bird.x] = BIRD;

    for (int i = 0; i < 10; i++) {
        if (game.pipes[i].x >= 0 && game.pipes[i].x < W) {
            for (int y = 0; y < game.pipes[i].gapY; y++) {
                screen[y][game.pipes[i].x] = PIPE;
            }
            for (int y = game.pipes[i].gapY + SPACE_GAP; y < H - 1; y++) {
                screen[y][game.pipes[i].x] = PIPE;
            }
        }
    }

    for (int y = 0; y < H - 1; y++) {
        printf("%s\n", screen[y]);
    }

    printf("Score: %d | Level: %d | Speed: %d\n", game.score, game.level, game.speed);

    if (game.gameOver) {
        printf("\n*** GAME OVER ***\nAppuyez sur ESPACE pour recommencer\n");
    }
}

int main() {
    srand((unsigned int)time(NULL));
    initGame();

    printf("Flappy Bird - Console Edition\n");
    printf("Appuyez sur UP (fleche haut) pour sauter\n");
    printf("Appuyez sur ESPACE pour commencer\n\n");
    getch();

    while (1) {
        if (kbio()) {
            int ch = getch();
            if (ch == 72 || ch == 'w' || ch == 'W') {
                if (!game.gameOver) {
                    game.bird.vy = -2;
                }
            }
            if (ch == 32) {
                if (game.gameOver) {
                    initGame();
                }
            }
            if (ch == 27) break;
        }

        updateGame();
        drawGame();

        Sleep(100);
    }

    printf("Merci d'avoir joue! Score final: %d\n", game.score);
    return 0;
}
