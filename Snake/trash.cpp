void Init() {
    printw("XXXXXXXXXXXX\n"
           "X          X\n"
           "X          X\n"
           "X          X\n"
           "X          X\n"
           "X          X\n"
           "X          X\n"
           "X          X\n"
           "X          X\n"
           "X          X\n"
           "X          X\n"
           "XXXXXXXXXXXX\n");
    mvprintw(5, 5, "oo");
    refresh();
    body.push((pair<int, int>){5, 6});
    body.push((pair<int, int>){5, 5});
}

void Update() {
    pair<int, int> tail = body.front();
    pair<int, int> head = body.back();

    int input = getch();
    if (input=='w'||input=='a'||input=='s'||input=='d') key = input;
    mvaddch(3, 20, (input=='w'||input=='a'||input=='s'||input=='d'));
    mvaddch(4, 20, (char)input);
    mvaddch(5, 20, (char)key);
    switch (key) {
        case 'w':
            body.pop();
            mvprintw(tail.first, tail.second, "x");
            mvprintw(head.first - 1, head.second, "0");
            body.push((pair<int, int>) {head.first - 1, head.second});
            break;
        case 'a':
            body.pop();
            mvprintw(tail.first, tail.second, "x");
            mvprintw(head.first, head.second - 1, "0");
            body.push((pair<int, int>) {head.first, head.second - 1});
            break;
        case 's':
            body.pop();
            mvprintw(tail.first, tail.second, "x");
            mvprintw(head.first + 1, head.second, "0");
            body.push((pair<int, int>) {head.first + 1, head.second});
            break;
        case 'd':
            body.pop();
            mvprintw(tail.first, tail.second, "x");
            mvprintw(head.first, head.second + 1, "0");
            body.push((pair<int, int>) {head.first, head.second + 1});
            break;
        default:
            break;
    }
}