
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <unistd.h>
#include <SDL2/SDL.h>

const int WIDTH = 900;
const int HEIGHT = 600;

#define SIZE 128

void setCell(uint8_t grid[SIZE][SIZE], int row, int col, uint8_t value) {
    if (row >= 0 && row < SIZE && col >= 0 && col < SIZE) {
        grid[row][col] = value;
    }
}

void move_rect(SDL_Rect *prect, SDL_KeyCode key) {
	switch (key) {
		case SDLK_w:
			prect->y -= 10;
			printf("moving with w...\n");
			break;
		case SDLK_a:
			prect->x -= 10;
			printf("moving with a...\n");
			break;
		case SDLK_s:
			prect->y += 10;
			printf("moving with s...\n");
			break;
		case SDLK_d:
			prect->x += 10;
			printf("moving with d...\n");
			break;
		default:
			break;
	}
}

int main(int argc, char *argv[])
{
	SDL_Window *pwindow = SDL_CreateWindow("physics sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer *prenderer = SDL_CreateRenderer(pwindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Rect rect = {100, 100, 100, 100};

	uint8_t matrix[SIZE][SIZE] = {0};


	int running = 1;

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT){
				SDL_Quit();
			} 

			if (event.type == SDL_KEYDOWN) {
				move_rect(&rect, event.key.keysym.sym);
			}

		}

		SDL_SetRenderDrawColor(prenderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(prenderer);
		SDL_SetRenderDrawColor(prenderer, 0xff, 0xff, 0xff, 0xff);
		SDL_RenderFillRect(prenderer, &rect);

		SDL_RenderPresent(prenderer);
		
	}
	return 0;
}
