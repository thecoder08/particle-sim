#include <xgfx/window.h>
#include <xgfx/drawing.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct {
    float x;
    float y;
    float xVelocity;
    float yVelocity;
    int color;
} Particle;

Particle particles[100];

float getDistance(Particle a, Particle b) {
    return sqrtf((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}

int main() {
    srand(1);
    for (int i = 0; i < 100; i++) {
        particles[i].x = rand() / (RAND_MAX/640);
        particles[i].y = rand() / (RAND_MAX/480);
        //particles[i].xVelocity = (float)rand() / RAND_MAX * 10 - 5;
        //particles[i].yVelocity = (float)rand() / RAND_MAX * 10 - 5;
        float h = ((float)rand() / RAND_MAX) * (2*M_PI);
        int r = 128*sinf(h)+128;
        int g = 128*sinf(h + 2.f/3*M_PI)+128;
        int b = 128*sinf(h + 4.f/3*M_PI)+128;
        particles[i].color = 0xff000000 + (r<<16) + (g<<8) + b;
    }

    initWindow(640, 480, "Particle Simulation");
    while(1) {
        Event event;
        while(checkWindowEvent(&event)) {
            if (event.type == WINDOW_CLOSE) {
                return 0;
            }
        }

        for (int i = 0; i < 100; i++) {
            for (int j = 0; j < 100; j++) {
                if (i == j) {
                    continue;
                }
                float distance = getDistance(particles[i], particles[j]);
                if (distance == 0) {
                    distance = 0.01;
                }
                float distanceX = (particles[i].x - particles[j].x) / distance; // normalize distance vector
                float distanceY = (particles[i].y - particles[j].y) / distance; //
                distanceX *= 1/(distance*distance); // scale distance vector by inverse square?
                distanceY *= 1/(distance*distance);
                particles[i].xVelocity += distanceX;
                particles[i].yVelocity += distanceY;
            }

            float topEdgeDistance = particles[i].y;
            if (topEdgeDistance == 0) {
                topEdgeDistance = 0.01;
            }
            float bottomEdgeDistance = 480-particles[i].y;
            if (bottomEdgeDistance == 0) {
                bottomEdgeDistance = 0.01;
            }
            float leftEdgeDistance = particles[i].x;
            if (leftEdgeDistance == 0) {
                leftEdgeDistance = 0.01;
            }
            float rightEdgeDistance = 640-particles[i].x;
            if (rightEdgeDistance == 0) {
                rightEdgeDistance = 0.01;
            }
            topEdgeDistance = 1/(topEdgeDistance*topEdgeDistance);
            bottomEdgeDistance = 1/(bottomEdgeDistance*bottomEdgeDistance);
            leftEdgeDistance = 1/(leftEdgeDistance*leftEdgeDistance);
            rightEdgeDistance = 1/(rightEdgeDistance*rightEdgeDistance);
            particles[i].xVelocity -= rightEdgeDistance;
            particles[i].xVelocity += leftEdgeDistance;
            particles[i].yVelocity -= bottomEdgeDistance;
            particles[i].yVelocity += topEdgeDistance;
        }

        clear();
        for (int i = 0; i < 100; i++) {
            

            //particles[i].yVelocity += 0.3;
            particles[i].x += particles[i].xVelocity;
            particles[i].y += particles[i].yVelocity;
            /*if (particles[i].x < 0) {
                particles[i].x = 0;
                particles[i].xVelocity = -particles[i].xVelocity;
            }
            if (particles[i].x >= 640) {
                particles[i].x = 639;
                particles[i].xVelocity = -particles[i].xVelocity;
            }
            if (particles[i].y < 0) {
                particles[i].y = 0;
                particles[i].yVelocity = -particles[i].yVelocity;
            }
            if (particles[i].y >= 480) {
                particles[i].y = 479;
                particles[i].yVelocity = -particles[i].yVelocity;
            }*/
            circle(particles[i].x, particles[i].y, 5, particles[i].color);
        }
        updateWindow();
    }
}