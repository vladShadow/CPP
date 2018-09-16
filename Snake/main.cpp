#include <ncurses.h>
#include <queue>
#include <random>
#include <ctime>
using namespace std;

const int height = 20, width = 41;
char fence = 'X';
char space = ' ';
char* node = (char*)"O";
char* food = (char*)"@";
char* crash = (char*)"#";
int stepY = 1, stepX = 2;
int stepTime = 500;

bool alive;
bool ate;
int input;
int score;
int speed;
clock_t startTime;
clock_t currTime;
char map[(height+2)*(width+3)];
deque<pair<int, int> > body;
pair<int, int> head;
int moveY, moveX;
int foodY, foodX;

void Init();
void Input();
void Move();
void Draw();

int main()
{
    initscr();
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    curs_set(0);

    Init();
    while(alive) {
        napms(stepTime-speed);
        Input();
        Move();
        Draw();
    }
    napms(10000);
    endwin();
    return 0;
}

void Init() {
    alive = true;
    ate = false;
    input = 0;
    score = 0;
    speed = 1;
    time(&startTime);

    for (int i = 0; i < width+2; i++) map[i] = fence;
    map[width+2] = '\n';
    for (int i = 0; i < height; i++) {
        map[(width+3)*(i+1)] = fence;
        for (int j = 0; j < width; j++) {
            map[(width+3)*(i+1)+1+j] = space;
        }
        map[(width+3)*(i+1)+1+width] = fence;
        map[(width+3)*(i+1)+2+width] = '\n';
    }
    for (int i = 0; i < width+2; i++) map[(width+3)*(height+1)+i] = fence;

    body.push_back((pair<int,int>){height/2, width/2});
    body.push_back((pair<int,int>){height/2, width/2+stepX});
    head = body.back();
    moveY = 0;
    moveX = stepX;
    foodY = 1 + (rand() + 1) % height;
    foodX = 1 + (rand() + 1) % width;
    Draw();
}

void Input() {
    if (moveY == 0) {
        do {
            if (input == 'q') {
                endwin();
                exit(0);
            }
            if (input == 'e') {
                mvprintw(6, width + 4, "pause");
                while (getch() != 'e');
            }
            if (input == 'w' || input == 's') {
                moveY = (input == 'w') ? -stepY : stepY;
                moveX = 0;
                break;
            }
            input = getch();
        } while (input != EOF);
        while (input != EOF && input != 'a' && input != 'd') {
            input = getch();
        }
    }
    else {
        do {
            if (input == 'q') {
                endwin();
                exit(0);
            }
            if (input == 'e') {
                mvprintw(6, width + 4, "pause");
                while (getch() != 'e');
            }

            if (input == 'a' || input == 'd') {
                moveX = (input == 'a') ? -stepX : stepX;
                moveY = 0;
                break;
            }
            input = getch();
        } while (input != EOF);
        while (input != EOF && input != 'w' && input != 's') {
            input = getch();
        }
    }
    flushinp();
}

void Move() {
    head = (pair<int, int>) {head.first + moveY, head.second + moveX};
    body.push_back(head);

    head = body.back();
    //crush
    if (head.first < 1 || head.first > height || head.second < 1 || head.second > width) {
        alive = false;
    }
    for (int i = 1; i < body.size()-1; i++) {
        if (head.first == body[i].first && head.second == body[i].second) {
            alive = false;
        }
    }
    //tail destruction
    if (!ate && alive) body.pop_front();
    else ate = false;
    //food
    if (head.first == foodY && head.second == foodX) {
        foodY = rand() % height + 1;
        foodY = (foodY % stepY == 0) ? foodY : (foodY + 1);
        foodX = rand() % width + 1;
        foodX = (foodX % stepX == 0) ? foodX : (foodX + 1);
        ate = true;
        score++;
        if (speed != stepTime) speed += stepTime/60;
    }
}

void Draw() {
    clear();
    printw(map);
    mvprintw(foodY, foodX, food);
    for (int i = 0; i < body.size(); i++) {
        mvprintw(body[i].first, body[i].second, node);
    }
    if (!alive) {
        mvprintw(head.first, head.second, crash);
        mvprintw(6, width + 4, "game");
        mvprintw(7, width + 4, "over");
    }
    mvprintw(0, width + 4, "score");
    mvprintw(1, width + 4, to_string(score).c_str());
    mvprintw(2, width + 4, "speed");
    mvprintw(3, width + 4, to_string(speed*100/stepTime).c_str());
    mvprintw(3, width + 8, "%%");
    mvprintw(4, width + 4, "time");
    time(&currTime);
    mvprintw(5, width + 4, to_string((int)difftime(currTime, startTime)).c_str());
    refresh();
}
