#include <iostream>
#include <math.h>
#include <stdlib.h>
class Particle {

private:
	double x;
	double y;
	double speed;
	double direction;
	double xspeed;
	double yspeed;

public:
	Particle() {
		assignInitialValues();
	}

	void assignInitialValues() {
		x = 0;
		y = 0;
		direction = M_PI * 2 * rand() / RAND_MAX; // (0,2π)
		speed = 0.02 * rand() / RAND_MAX;
		speed *= speed;
	}

	double getX() { return x; }
	double getY() { return y; }

	void update(int deltaTime) {
		direction += deltaTime * 0.0003;
		xspeed = speed * (sin(8 * direction) - cos(4 * direction));
		yspeed = speed * (cos(8 * direction) - sin(4 * direction));
		x += xspeed * deltaTime;
		y += yspeed * deltaTime;

		if (x < -1 || x > 1 || y < -0.75 || y > 0.75) {
			assignInitialValues();
		}
	}
};

