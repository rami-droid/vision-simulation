
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>

const int WIDTH = 1024;
const int HEIGHT = 1024;

#define SIZE 128
#define NSMULT 1000

struct player{
	int x;
	int y;
	int vision_rad;
};

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

void draw_grid(uint8_t grid[SIZE][SIZE], SDL_Renderer *prender){

	SDL_Rect cellRect;
	cellRect.w = WIDTH/SIZE;
	cellRect.h = HEIGHT/SIZE;
	for (int i = 0; i<SIZE; i++) {
		for (int j = 0; j<SIZE; j++) {
			cellRect.x = i * WIDTH/SIZE;
			cellRect.y = j * HEIGHT/SIZE;
			switch (grid[i][j]) {
				case 0:
					SDL_SetRenderDrawColor(prender, 0, 0, 0, 255);
					break;
				case 1:
					SDL_SetRenderDrawColor(prender, 255, 255, 255, 255);
					break;
			}
			SDL_RenderFillRect(prender, &cellRect);
		}
	}
}

int main(int argc, char *argv[])
{
	SDL_Window *pwindow = SDL_CreateWindow("physics sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer *prenderer = SDL_CreateRenderer(pwindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Rect rect = {100, 100, 100, 100};

	struct player currPlayer;
	currPlayer.x = 5;
	currPlayer.y = 5;


	//0 is no vision, 1 is vision, 2 is player
	uint8_t matrix[SIZE][SIZE] = {0};

	setCell(matrix, 0, 0, 1);
	setCell(matrix, 0, 4, 1);
	setCell(matrix, 23, 4, 1);


	int running = 1;

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT){
				SDL_Quit();
				running = 0;
			} 

			if (event.type == SDL_KEYDOWN) {
				move_rect(&rect, event.key.keysym.sym);
			}

		}

		clock_t start = clock();

		SDL_RenderClear(prenderer);
		//SDL_SetRenderDrawColor(prenderer, 0xff, 0xff, 0xff, 0xff);
		//SDL_RenderFillRect(prenderer, &rect);
		draw_grid(matrix, prenderer);

		SDL_RenderPresent(prenderer);
		clock_t end = clock();
		double time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
		
		usleep((16 - time_ms) * NSMULT);
		
	}
	return 0;
}
