#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>

const int WIDTH = 40;
const int HEIGHT = 20;
const int SPEED = 100;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

class Snake {
private:
    std::vector<Point> body;
    Direction dir;
    Point food;
    bool game_over;

    void generate_food() {
        do {
            food.x = rand() % WIDTH;
            food.y = rand() % HEIGHT;
        } while (std::find(body.begin(), body.end(), food) != body.end());
    }

public:
    Snake() : dir(RIGHT), game_over(false) {
        body.push_back(Point(WIDTH / 2, HEIGHT / 2));
        generate_food();
    }

    void update() {
        move_snake();
        if (body[0] == food) {
            body.push_back(body.back());
            generate_food();
        }
        for (int i = 1; i < body.size() - 1; ++i) {
            if (body[0] == body[i]) {
                game_over = true;
                return;
            }
        }
    }

    void handle_input() {
        int ch = getch();
        switch (ch) {
            case 'w':
                if (dir != DOWN) dir = UP;
                break;
            case 's':
                if (dir != UP) dir = DOWN;
                break;
            case 'a':
                if (dir != RIGHT) dir = LEFT;
                break;
            case 'd':
                if (dir != LEFT) dir = RIGHT;
                break;
            case 'q':
                game_over = true;
                break;
        }
    }

    void move_snake() {
        for (int i = body.size() - 1; i > 0; --i) {
            body[i] = body[i - 1];
        }
        switch (dir) {
            case UP:
                --body[0].y;
                break;
            case DOWN:
                ++body[0].y;
                break;
            case LEFT:
                --body[0].x;
                break;
            case RIGHT:
                ++body[0].x;
                break;
        }
        if (body[0].x >= WIDTH) body[0].x = 0;
        if (body[0].x < 0) body[0].x = WIDTH - 1;
        if (body[0].y >= HEIGHT) body[0].y = 0;
        if (body[0].y < 0) body[0].y = HEIGHT - 1;
    }

    void draw() {
        clear();
        mvprintw(food.y, food.x, "F");
        for (size_t i = 0; i < body.size(); ++i) {
            const Point& p = body[i];
            mvprintw(p.y, p.x, "O");
        }
        refresh();
    }

    bool is_game_over() const {
        return game_over;
    }
};

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    srand(time(NULL));
    Snake snake;
    while (!snake.is_game_over()) {
        snake.handle_input();
        snake.update();
        snake.draw();
        napms(SPEED);
    }
    mvprintw(HEIGHT / 2, WIDTH / 2 - 5, "Game Over!");
    mvprintw(HEIGHT / 2 + 1, WIDTH / 2 - 10, "Press any key to exit...");
    getch();
    endwin();
    return 0;
}
