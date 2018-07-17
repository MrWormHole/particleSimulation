#include <SDL.h>
#include <time.h>
#include "Screen.hpp"
#include "ParticleManager.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	srand(time(0));
	Screen screen;
	ParticleManager particleManager;

	if (screen.init() == false) {
		SDL_Quit();
	}

	while (true) {
		int time = SDL_GetTicks();

		particleManager.update(time);
		particleManager.applyColorChangeOverTime(time, screen);
		screen.meanBlur();
		screen.update();

		if (screen.processEvents() == false) {
			break;
		}
	}

	screen.close();
	return 0;
}