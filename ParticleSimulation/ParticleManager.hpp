#include"Particle.hpp"

class ParticleManager {

private:
	Particle* pParticles;
	float increaseRateOfRed = 0.0001f;
	float increaseRateOfGreen = 0.0002f;
	float increaseRateOfBlue = 0.0003f;
	const static int numberOfParticles = 5000;
	int lastTime;

public:
	ParticleManager() {
		lastTime = 0;
		pParticles = new Particle[numberOfParticles];
	}
	~ParticleManager() {
		delete[] pParticles;
	}
	Particle* getParticles() {
		return pParticles;
	}

	void update(int currentTime) {
		if (currentTime > lastTime + 33.33f) { //60 fps for PC MASTER RACE PogChamp
			for (int i = 0; i < numberOfParticles; i++) {
				pParticles[i].update(currentTime - lastTime);
			}
			lastTime = currentTime;
		}
	}

	void applyColorChangeOverTime(int time, Screen& s) {
		unsigned char red = (unsigned char)((1 + sin(time * increaseRateOfRed)) * 128); //never becomes 256
		unsigned  green = (unsigned char)((1 + sin(time * increaseRateOfGreen)) * 128);
		unsigned char blue = (unsigned char)((1 + sin(time * increaseRateOfBlue)) * 128);
		Particle* pParticles = getParticles();
		for (int i = 0; i < numberOfParticles; i++) {
			Particle particle = pParticles[i];
			int x = (int)((particle.getX() + 1) * Screen::WINDOW_WIDTH / 2);
			int y = (int)(particle.getY() * Screen::WINDOW_WIDTH / 2 + Screen::WINDOW_HEIGHT / 2);
			s.setPixel(x, y, red, green, blue);
		}
	}
};

