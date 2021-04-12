#include <SDL2/SDL.h>

#define CELL_SIZE 20
#define GRID_WIDTH 50
#define GRID_HEIGHT 30

#define WINDOW_WIDTH (CELL_SIZE * GRID_WIDTH) + 1
#define WINDOW_HEIGHT (CELL_SIZE * GRID_HEIGHT) + 1

struct Cell {
    SDL_Rect cellRect;
    bool alive = false;
    bool next = false;
};

void step(Cell cellArray[][GRID_HEIGHT]) {

    for (int i = 1; i < GRID_WIDTH - 1; i++) {
        for (int j = 1; j < GRID_HEIGHT - 1; j++) {
            int neighbourCount =
                    (cellArray[i - 1][j - 1].alive ? 1 : 0)
                    + (cellArray[i - 1][j].alive ? 1 : 0)
                    + (cellArray[i - 1][j + 1].alive ? 1 : 0)
                    + (cellArray[i][j - 1].alive ? 1 : 0)
                    + (cellArray[i][j + 1].alive ? 1 : 0)
                    + (cellArray[i + 1][j - 1].alive ? 1 : 0)
                    + (cellArray[i + 1][j].alive ? 1 : 0)
                    + (cellArray[i + 1][j + 1].alive ? 1 : 0);
            if (cellArray[i][j].alive) {
                if (neighbourCount != 2 && neighbourCount != 3) {
                    cellArray[i][j].next = false;
                }
            } else {
                if (neighbourCount == 3) {
                    cellArray[i][j].next = true;
                }
            }
        }
    }

    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            cellArray[i][j].alive = cellArray[i][j].next;
        }
    }
}

int main(int argc, char *args[]) {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error: %s", SDL_GetError());
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error: %s", SDL_GetError());
    }

    SDL_Color darkColor = {0x20, 0x20, 0x20, 0xFF};
    SDL_Color lightColor = {0x60, 0x60, 0x60, 0xFF};

    Cell CellArray[GRID_WIDTH][GRID_HEIGHT];

    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int j = 0; j < GRID_HEIGHT; j++) {
            CellArray[i][j].cellRect.x = i * CELL_SIZE + 1;
            CellArray[i][j].cellRect.y = j * CELL_SIZE + 1;
            CellArray[i][j].cellRect.w = CELL_SIZE - 1;
            CellArray[i][j].cellRect.h = CELL_SIZE - 1;
        }
    }

    SDL_Event e;
    bool running = true;
    bool mouseDown = false;

    while (running) {
        SDL_PollEvent(&e);
        switch (e.type) {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_c:
                        step(CellArray);
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!mouseDown) {
                    int cellXIndex = e.motion.x / CELL_SIZE;
                    int cellYIndex = e.motion.y / CELL_SIZE;
                    CellArray[cellXIndex][cellYIndex].alive = !CellArray[cellXIndex][cellYIndex].alive;
                    CellArray[cellXIndex][cellYIndex].next = CellArray[cellXIndex][cellYIndex].alive;
                    mouseDown = true;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                mouseDown = false;
                break;

            case SDL_QUIT:
                running = false;
                break;
        }

        SDL_SetRenderDrawColor(renderer, lightColor.r, lightColor.g, lightColor.b, lightColor.a);
        SDL_RenderClear(renderer);

        for (int i = 0; i < GRID_WIDTH; i++) {
            for (int j = 0; j < GRID_HEIGHT; j++) {
                if (!CellArray[i][j].alive) {
                    SDL_SetRenderDrawColor(renderer, darkColor.r, darkColor.g, darkColor.b, darkColor.a);
                } else {
                    SDL_SetRenderDrawColor(renderer, lightColor.r, lightColor.g, lightColor.b, lightColor.a);
                }
                SDL_RenderFillRect(renderer, &CellArray[i][j].cellRect);
            }
        }

        SDL_RenderPresent(renderer);
    }
}

