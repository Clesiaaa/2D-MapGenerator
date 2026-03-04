#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 256

int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));

    uint16_t **matrix = (uint16_t **)calloc(SIZE + 1, sizeof(uint16_t *));
    for (int i = 0; i <= SIZE; i++)
        matrix[i] = (uint16_t *)calloc(SIZE + 1, sizeof(uint16_t));

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) return 1;
    if (SDL_CreateWindowAndRenderer(1024, 1024, 0, &window, &renderer) != 0) {
        SDL_Quit();
        return 1;
    }
    SDL_RenderSetScale(renderer, 4, 4);

    SDL_Event event;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT) quit = 1;

        matrix[0][0]       = rand() % 6;
        matrix[SIZE][0]    = rand() % 6;
        matrix[0][SIZE]    = rand() % 6;
        matrix[SIZE][SIZE] = rand() % 6;

        int step = SIZE;

        while (step > 1) {
            int half = step / 2;

            // --- DIAMOND STEP ---
            for (int x = half; x < SIZE; x += step) {
                for (int y = half; y < SIZE; y += step) {
                    int avg = (matrix[x - half][y - half]
                             + matrix[x + half][y - half]
                             + matrix[x - half][y + half]
                             + matrix[x + half][y + half]) / 4;
                    matrix[x][y] = avg + (rand() % (step + 1));
                }
            }

            // --- SQUARE STEP ---
            for (int x = 0; x <= SIZE; x += half) {
                for (int y = (x / half % 2 == 0) ? half : 0; y <= SIZE; y += step) {
                    int sum = 0, n = 0;
                    if (x >= half) { sum += matrix[x - half][y]; n++; }
                    if (x + half <= SIZE) { sum += matrix[x + half][y]; n++; }
                    if (y >= half)   { sum += matrix[x][y - half]; n++; }
                    if (y + half <= SIZE) { sum += matrix[x][y + half]; n++; }
                    matrix[x][y] = sum / n + (rand() % (step + 1));
                }
            }

            step = half;
        }

        uint16_t max_val = 1;
        for (int x = 0; x < SIZE; x++)
            for (int y = 0; y < SIZE; y++)
                if (matrix[x][y] > max_val) max_val = matrix[x][y];

        for (int x = 0; x < SIZE; x++) {
            for (int y = 0; y < SIZE; y++) {
                uint8_t color = (uint8_t)((matrix[x][y] * 5) / max_val);

                if      (color == 0) { SDL_SetRenderDrawColor(renderer, 34,  85,  20,  255); }
                else if (color == 1) { SDL_SetRenderDrawColor(renderer, 160, 82,  45,  255); }
                else if (color == 2) { SDL_SetRenderDrawColor(renderer, 80,  160, 50,  255); }
                else if (color == 3) { SDL_SetRenderDrawColor(renderer, 210, 190, 100, 255); }
                else if (color == 4) { SDL_SetRenderDrawColor(renderer, 20,  80,  200, 255); }
                else                 { SDL_SetRenderDrawColor(renderer, 101, 40,  10,  255); }
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
   
        SDL_RenderPresent(renderer);
        SDL_Delay(4000);
    }

    for (int i = 0; i <= SIZE; i++) free(matrix[i]);
    free(matrix);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}