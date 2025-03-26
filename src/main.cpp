#include <SDL2/SDL.h>
#include <iostream>

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PADDLE_WIDTH = 10;
const int PADDLE_HEIGHT = 100;
const int BALL_SIZE = 10;
const int PADDLE_SPEED = 10;
const int BALL_SPEED = 5;

// SDL variables
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Paddle and ball positions
SDL_Rect leftPaddle = { 50, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
SDL_Rect rightPaddle = { WINDOW_WIDTH - 50 - PADDLE_WIDTH, (WINDOW_HEIGHT - PADDLE_HEIGHT) / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
SDL_Rect ball = { WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE };

int ballVelX = BALL_SPEED;
int ballVelY = BALL_SPEED;

bool running = true;

void handleEvents() {
    SDL_Event event;
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT)
            running = false;
    }

    // Move left paddle
    if (keystates[SDL_SCANCODE_W] && leftPaddle.y > 0)
        leftPaddle.y -= PADDLE_SPEED;
    if (keystates[SDL_SCANCODE_S] && leftPaddle.y + PADDLE_HEIGHT < WINDOW_HEIGHT)
        leftPaddle.y += PADDLE_SPEED;

    // Move right paddle
    if (keystates[SDL_SCANCODE_UP] && rightPaddle.y > 0)
        rightPaddle.y -= PADDLE_SPEED;
    if (keystates[SDL_SCANCODE_DOWN] && rightPaddle.y + PADDLE_HEIGHT < WINDOW_HEIGHT)
        rightPaddle.y += PADDLE_SPEED;
}

void update() {
    // Move ball
    ball.x += ballVelX;
    ball.y += ballVelY;

    // Bounce top and bottom
    if (ball.y <= 0 || ball.y + BALL_SIZE >= WINDOW_HEIGHT)
        ballVelY = -ballVelY;

    // Bounce left paddle
    if (SDL_HasIntersection(&ball, &leftPaddle))
        ballVelX = BALL_SPEED;

    // Bounce right paddle
    if (SDL_HasIntersection(&ball, &rightPaddle))
        ballVelX = -BALL_SPEED;

    // Reset if ball goes off screen
    if (ball.x < 0 || ball.x > WINDOW_WIDTH) {
        ball.x = WINDOW_WIDTH / 2 - BALL_SIZE / 2;
        ball.y = WINDOW_HEIGHT / 2 - BALL_SIZE / 2;
        ballVelX = -ballVelX;
        ballVelY = BALL_SPEED;
    }
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white objects

    SDL_RenderFillRect(renderer, &leftPaddle);
    SDL_RenderFillRect(renderer, &rightPaddle);
    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer);
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init failed: " << SDL_GetError() << "\n";
        return false;
    }

    window = SDL_CreateWindow("Pong SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << "\n";
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << "\n";
        return false;
    }

    return true;
}

void cleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (!initSDL()) return -1;

    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16); // ~60 FPS
    }

    cleanUp();
    return 0;
}