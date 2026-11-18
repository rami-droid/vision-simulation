
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <SDL2/SDL.h>

const int WIDTH = 1024;
const int HEIGHT = 1024;

#define SIZE 64
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

int is_in_center(int col, int row, struct player *pplayer){
	int center_x = pplayer->x;
	int center_y = pplayer->y;

	int dist_squared = pow((col - center_x), 2) + pow((row - center_y), 2);
	int r_squared = pow(pplayer->vision_rad, 2);

	if (dist_squared <= r_squared) {
		return 1;
	}

	return 0;
}

void calculate_vision(uint8_t vision[SIZE][SIZE], struct player *pplayer) {

	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (vision[i][j] != 2) {
				vision[i][j] = 0;
			}
		}
	}

	int start_row = pplayer->y - pplayer->vision_rad;
	int end_row   = pplayer->y + pplayer->vision_rad;

	int start_col = pplayer->x - pplayer->vision_rad;
	int end_col   = pplayer->x + pplayer->vision_rad;

	//clamp boundaries
	if (start_row < 0) start_row = 0;
	if (start_col < 0) start_col = 0;

	if (end_row >= SIZE) end_row = SIZE - 1;
	if (end_col >= SIZE) end_col = SIZE - 1;

	for (int i = start_row; i <= end_row; i++) {
		for (int j = start_col; j <= end_col; j++) {

			if (is_in_center(j, i, pplayer)) {
			if (vision[i][j] != 2) {
				vision[i][j] = 1;
			}
			}
		}
	}
}

void move_player(struct player *pplayer, uint8_t matrix[SIZE][SIZE], SDL_KeyCode key) {
	int old_x = pplayer->x;
	int old_y = pplayer->y;
	switch (key) {
		case SDLK_w:
			pplayer->y -= 1;
			break;
		case SDLK_a:
			pplayer->x -= 1;
			break;
		case SDLK_s:
			pplayer->y += 1;
			break;
		case SDLK_d:
			pplayer->x += 1;
			break;
		default:
			break;
	}

	matrix[old_y][old_x] = 0;

	if (pplayer->y < 0) pplayer->y = 0;
	if (pplayer->x < 0) pplayer->x = 0;

	if (pplayer->y > SIZE) pplayer->y = SIZE;
	if (pplayer->x > SIZE) pplayer->x = SIZE;
	matrix[pplayer->y][pplayer->x] = 2;
}

void draw_grid(uint8_t grid[SIZE][SIZE], SDL_Renderer *prender){
	SDL_Rect cellRect;
	cellRect.w = WIDTH/SIZE;
	cellRect.h = HEIGHT/SIZE;
	for (int i = 0; i<SIZE; i++) {
		for (int j = 0; j<SIZE; j++) {
			cellRect.x = j * WIDTH/SIZE;
			cellRect.y = i * HEIGHT/SIZE;

			switch (grid[i][j]) {
				case 0:
					SDL_SetRenderDrawColor(prender, 0, 255, 0, 255);
					break;
				case 1:
					SDL_SetRenderDrawColor(prender, 0, 000, 255, 255);
					break;
				case 2:
					SDL_SetRenderDrawColor(prender, 255, 0, 0, 255);
					break;
			}
			SDL_RenderFillRect(prender, &cellRect);
		}
	}
	
}

void draw_vision(uint8_t vision[SIZE][SIZE], SDL_Renderer *prender){

	SDL_Rect cellRect;
	cellRect.w = WIDTH/SIZE;
	cellRect.h = HEIGHT/SIZE;
	for (int i = 0; i<SIZE; i++) {
		for (int j = 0; j<SIZE; j++) {
			cellRect.x = j * WIDTH/SIZE;
			cellRect.y = i * HEIGHT/SIZE;

			switch (vision[i][j]) {
				case 0:
					SDL_SetRenderDrawColor(prender, 0, 0, 0, 255);
					break;
				case 1:
					SDL_SetRenderDrawColor(prender, 0, 0, 0, 128);
					break;
			}
			SDL_RenderFillRect(prender, &cellRect);
		}
	}
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *pwindow = SDL_CreateWindow("physics sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Renderer *prenderer = SDL_CreateRenderer(pwindow, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(prenderer, SDL_BLENDMODE_BLEND);

	struct player currPlayer;
	currPlayer.x = 5;
	currPlayer.y = 5;
	currPlayer.vision_rad = 5;



	//0 is no vision, 1 is vision, 2 is player
	uint8_t matrix[SIZE][SIZE] = {0};
	matrix[currPlayer.y][currPlayer.x] = 2;

	uint8_t vision_matrix[SIZE][SIZE] = {0};

	calculate_vision(vision_matrix, &currPlayer);

	int running = 1;

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT){
				SDL_Quit();
				running = 0;
			} 

			if (event.type == SDL_KEYDOWN) {
				move_player(&currPlayer, matrix, event.key.keysym.sym);
				calculate_vision(vision_matrix, &currPlayer);
			}

		}

		clock_t start = clock();

		SDL_RenderClear(prenderer);
		//SDL_SetRenderDrawColor(prenderer, 0xff, 0xff, 0xff, 0xff);
		//SDL_RenderFillRect(prenderer, &rect);
		draw_grid(matrix, prenderer);
		draw_vision(vision_matrix, prenderer);

		SDL_RenderPresent(prenderer);
		clock_t end = clock();
		double time_ms = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
		
		usleep((16 - time_ms) * NSMULT);
		
	}
	return 0;
}
