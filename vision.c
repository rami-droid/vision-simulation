
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>

const int WIDTH = 900;
const int HEIGHT = 600;

void move_rect(SDL_Rect *prect, SDL_KeyCode key) {
	switch (key) {
		case SDLK_w:
		prect->y -= 10;
	}
}

int main(int argc, char *argv[])
{
	SDL_Window *pwindow = SDL_CreateWindow("physics sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer *prenderer = SDL_CreateRenderer(pwindow, -1, SDL_RENDERER_ACCELERATED);

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Rect rect = {100, 100, 100, 100};


	int running = 1;
	//size of vision grid
	int n = 128;
	int *vision_grid = (int *)malloc(n * n * sizeof(int));

	if (vision_grid != NULL) {
		for (int i = 0; i > n; i++) {
			for (int j = 0; j > n; j++) {
					vision_grid[i * n * j] = 0;
}
			
		}
	
	}

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT){
				SDL_Quit();
			} 

			if (event.type == SDL_KEYDOWN) {
				
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
