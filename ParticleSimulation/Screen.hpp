#include <iostream>
using namespace std;

class Screen {

private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	Uint32 *firstBuffer;
	Uint32 *secondBuffer;
	int lastTime = 0;

public:
	const static int WINDOW_WIDTH = 800;
	const static int WINDOW_HEIGHT = 600;

	Screen() {
		window = NULL;
		renderer = NULL;
		texture = NULL;
		firstBuffer = NULL;
		secondBuffer = NULL;
	}

	bool init() {
		//error checks
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return false;
		}

		window = SDL_CreateWindow("Particle Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); //PRESENTVSYNC is also preferable
		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, WINDOW_WIDTH, WINDOW_HEIGHT);

		if (window == NULL) {
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			SDL_Delay(3000);
			return false;
		}
		if (texture == NULL) {
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			SDL_Delay(3000);
			return false;
		}
		if (renderer == NULL) {
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			SDL_Delay(3000);
			return false;
		}
		//allocating buffers
		firstBuffer = new Uint32[WINDOW_WIDTH * WINDOW_HEIGHT];
		secondBuffer = new Uint32[WINDOW_WIDTH * WINDOW_HEIGHT];
		memset(firstBuffer, 0, WINDOW_WIDTH*WINDOW_HEIGHT * sizeof(Uint32));
		memset(secondBuffer, 0, WINDOW_WIDTH*WINDOW_HEIGHT * sizeof(Uint32));

		return true;
	}


	void update() {
		SDL_UpdateTexture(texture, NULL, firstBuffer, WINDOW_WIDTH * sizeof(Uint32)); // param Texture: the texture to update, param Rect:A pointer to the rectangle of pixels to update;NULL to update entire, param Pixels: raw pixel data, param Pitch: The number of bytes in each row
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL); //last 2 params are null because we want to render entire texture
		SDL_RenderPresent(renderer);
	}

	void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
		if ((x < WINDOW_WIDTH && x >= 0) && (y < WINDOW_HEIGHT && y >= 0)) {
			Uint32 color = 0;
			Uint8 alpha = 0xFF;
			color += red;
			color = color << 8;
			color += green;
			color = color << 8;
			color += blue;
			color = color << 8;
			color += alpha;

			firstBuffer[y * WINDOW_WIDTH + x] = color;
		}
	}

	void setBackGround(Uint8 red, Uint8 green, Uint8 blue) {
		Uint32 color = 0;
		Uint8 alpha = 0xFF;
		color += red;
		color = color << 8;
		color += green;
		color = color << 8;
		color += blue;
		color = color << 8;
		color += alpha;

		for (int y = 0; y < WINDOW_HEIGHT; y++) {
			for (int x = 0; x < WINDOW_WIDTH; x++) {
				setPixel(x, y, red, green, blue);
			}
		}
	}

	bool processEvents() {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return false;
			}
		}
		return true;
	}

	//This is also known as boxBlur.Our kernel is 3x3 and filled by 1's
	void meanBlur() {
		Uint32 *temp = firstBuffer;
		firstBuffer = secondBuffer;
		secondBuffer = temp;

		for (int y = 1; y < WINDOW_HEIGHT - 1; y++) {
			for (int x = 1; x < WINDOW_WIDTH - 1; x++) {
				int redT = 0;
				int greenT = 0;
				int blueT = 0;

				for (int m = -1; m < 2; m++) {
					for (int n = -1; n < 2; n++) {
						Uint32 color = secondBuffer[(y + m)*WINDOW_WIDTH + x + n];
						Uint8 red = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;
						redT += red;
						greenT += green;
						blueT += blue;
					}
				}
				Uint8 redAv = redT / 9;
				Uint8 greenAv = greenT / 9;
				Uint8 blueAv = blueT / 9;

				setPixel(x, y, redAv, greenAv, blueAv);
			}
		}
	}

	void clearBuffer(int n) {
		if (n == 1) {
			memset(firstBuffer, 0, WINDOW_WIDTH*WINDOW_HEIGHT * sizeof(Uint32));
		}
		else if (n == 2) {
			memset(secondBuffer, 0, WINDOW_WIDTH*WINDOW_HEIGHT * sizeof(Uint32));
		}
	}

	void close() {
		delete[] firstBuffer;
		delete[] secondBuffer;
		SDL_DestroyRenderer(renderer);
		SDL_DestroyTexture(texture);
		SDL_DestroyWindow(window);
		SDL_Delay(200);
		SDL_Quit();
	}
};
