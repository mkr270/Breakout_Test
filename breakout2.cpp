#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

// Constants
const int WIDTH = 60;  // Increased width
const int HEIGHT = 30; // Increased height
const int PADDLE_WIDTH = 15;
const int PADDLE_SPEED = 3; // Increased paddle speed
const int BRICK_ROWS = 5;
const int BRICK_COLS = 10;
const int BRICK_WIDTH = WIDTH / BRICK_COLS;
const int BRICK_HEIGHT = HEIGHT / BRICK_ROWS;

// Game objects
int ballX = WIDTH / 2;
int ballY = HEIGHT - 2; // Ball starts from the paddle
int ballDX = 1;
int ballDY = -1;
int paddleX = (WIDTH - PADDLE_WIDTH) / 2;
bool bricks[BRICK_ROWS][BRICK_COLS] = { {true, true, true, true, true, true, true, true, true, true},
                                        {true, true, true, true, true, true, true, true, true, true},
                                        {true, true, true, true, true, true, true, true, true, true},
                                        {true, true, true, true, true, true, true, true, true, true},
                                        {false, false, false, false, false, false, false, false, false, false} };

// Function to draw the game objects
void draw() {
    clear();
    // Draw bricks
    attron(COLOR_PAIR(1)); // Set color for bricks
    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            if (bricks[row][col]) {
                for (int i = 0; i < BRICK_HEIGHT; ++i) {
                    for (int j = 0; j < BRICK_WIDTH; ++j) {
                        mvprintw(row * BRICK_HEIGHT + i, col * BRICK_WIDTH + j, "#");
                    }
                }
            }
        }
    }
    attroff(COLOR_PAIR(1)); // Turn off color for bricks

    // Draw paddle
    attron(COLOR_PAIR(2)); // Set color for paddle
    mvprintw(HEIGHT - 1, paddleX, "===========");
    attroff(COLOR_PAIR(2)); // Turn off color for paddle

    // Draw ball
    attron(COLOR_PAIR(3)); // Set color for ball
    mvprintw(ballY, ballX, "O");
    attroff(COLOR_PAIR(3)); // Turn off color for ball

    refresh();
}

// Function to update the game state
void update() {
    // Update ball position
    ballX += ballDX;
    ballY += ballDY;

    // Ball collision with walls
    if (ballX <= 0 || ballX >= WIDTH - 1) {
        ballDX = -ballDX;
    }
    if (ballY <= 0) {
        ballDY = -ballDY;
    }

    // Ball collision with paddle
    if (ballY == HEIGHT - 1 && ballX >= paddleX && ballX < paddleX + PADDLE_WIDTH) {
        ballDY = -ballDY;
    }

    // Ball collision with bricks
    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            if (bricks[row][col]) {
                int brickTop = row * BRICK_HEIGHT;
                int brickBottom = (row + 1) * BRICK_HEIGHT - 1;
                int brickLeft = col * BRICK_WIDTH;
                int brickRight = (col + 1) * BRICK_WIDTH - 1;

                if (ballY == brickTop && ballX >= brickLeft && ballX <= brickRight) {
                    bricks[row][col] = false;
                    ballDY = -ballDY;
                } else if (ballY == brickBottom && ballX >= brickLeft && ballX <= brickRight) {
                    bricks[row][col] = false;
                    ballDY = -ballDY;
                } else if (ballX == brickLeft && ballY >= brickTop && ballY <= brickBottom) {
                    bricks[row][col] = false;
                    ballDX = -ballDX;
                } else if (ballX == brickRight && ballY >= brickTop && ballY <= brickBottom) {
                    bricks[row][col] = false;
                    ballDX = -ballDX;
                }
            }
        }
    }

    // Check for game over
    if (ballY >= HEIGHT) {
        endwin();
        std::cout << "Game Over! Press 'r' to restart or 'q' to quit." << std::endl;
        while (true) {
            int ch = getch();
            if (ch == 'r') {
                // Reset game state
                ballX = WIDTH / 2;
                ballY = HEIGHT - 2; // Ball starts from the paddle
                ballDX = (std::rand() % 2 == 0) ? 1 : -1; // Random horizontal direction
                ballDY = -1; // Ball moves upwards
                paddleX = (WIDTH - PADDLE_WIDTH) / 2;
                for (int row = 0; row < BRICK_ROWS; ++row) {
                    for (int col = 0; col < BRICK_COLS; ++col) {
                        bricks[row][col] = true;
                    }
                }
                initscr();
                cbreak();
                noecho();
                nodelay(stdscr, TRUE);
                keypad(stdscr, TRUE);
                start_color();
                init_pair(1, COLOR_RED, COLOR_BLACK); // Brick color
                init_pair(2, COLOR_GREEN, COLOR_BLACK); // Paddle color
                init_pair(3, COLOR_BLUE, COLOR_BLACK); // Ball color
                curs_set(0); // Hide cursor
                break;
            } else if (ch == 'q') {
                exit(0);
            }
        }
    }

    // Check for win
    bool allBricksDestroyed = true;
    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            if (bricks[row][col]) {
                allBricksDestroyed = false;
                break;
            }
        }
    }
    if (allBricksDestroyed) {
        endwin();
        std::cout << "You Win! Press 'r' to restart or 'q' to quit." << std::endl;
        while (true) {
            int ch = getch();
            if (ch == 'r') {
                // Reset game state
                ballX = WIDTH / 2;
                ballY = HEIGHT - 2; // Ball starts from the paddle
                ballDX = (std::rand() % 2 == 0) ? 1 : -1; // Random horizontal direction
                ballDY = -1; // Ball moves upwards
                paddleX = (WIDTH - PADDLE_WIDTH) / 2;
                for (int row = 0; row < BRICK_ROWS; ++row) {
                    for (int col = 0; col < BRICK_COLS; ++col) {
                        bricks[row][col] = true;
                    }
                }
                initscr();
                cbreak();
                noecho();
                nodelay(stdscr, TRUE);
                keypad(stdscr, TRUE);
                start_color();
                init_pair(1, COLOR_RED, COLOR_BLACK); // Brick color
                init_pair(2, COLOR_GREEN, COLOR_BLACK); // Paddle color
                init_pair(3, COLOR_BLUE, COLOR_BLACK); // Ball color
                curs_set(0); // Hide cursor
                break;
            } else if (ch == 'q') {
                exit(0);
            }
        }
    }
}

// Function to handle user input
void input() {
    int ch = getch();
    switch (ch) {
    case 'a':
        if (paddleX > 0) paddleX -= PADDLE_SPEED;
        break;
    case 'd':
        if (paddleX < WIDTH - PADDLE_WIDTH) paddleX += PADDLE_SPEED;
        break;
    case 'q':
        endwin();
        exit(0);
        break;
    }
}

int main() {
    std::srand(std::time(0)); // Seed the random number generator

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK); // Brick color
    init_pair(2, COLOR_GREEN, COLOR_BLACK); // Paddle color
    init_pair(3, COLOR_BLUE, COLOR_BLACK); // Ball color
    curs_set(0); // Hide cursor

    while (true) {
        draw();
        update();
        input();
        usleep(100000);
    }

    endwin();
    return 0;
}