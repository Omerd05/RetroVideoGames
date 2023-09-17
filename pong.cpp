#include <iostream>
#include <vector>
#include <conio.h>
#include <chrono>
#include <thread>
#include <random>



using namespace std;
using namespace chrono;

const int width = 90;
const int height = 30;

uniform_int_distribution<int> xGen(5, width-5);
uniform_int_distribution<int> yGen(3, 5);
random_device rd;
mt19937 rng(rd());

pair<int, int> randomDot() {
    return { xGen(rng),yGen(rng) };
}

struct Ball
{
    pair<int,int> cords[4]; //clockwise
    pair<int, int> direction; //{plusminus 1,plusminus 1}
};

struct Bar
{
    pair<int, int> wings;
    int h;
    int color;
};

Ball ball;
Bar player;
Bar bot;
bool turn; //1-player,0-bot
int cnt = 0;
int botLvl = 0;

void changeChar(int x, int y, char ch, int color) {
    cout << "\033[" << color << "m";
    cout << "\033[" << y << ";" << x << "H";
    cout << ch;
    cout << "\033[0m";
}

void init() {

    //for (int i = 0; i < width; i++) {
    //    changeChar(i,0,'-',0);
    //}
    for (int i = 1; i < height; i++) {
        changeChar(0, i, '|',36);
        changeChar(width, i, '|',36);
    }

    player.wings.first = 22;
    player.wings.second = 27;
    player.h = height;
    player.color = 32;

    bot.wings.first = 22;
    bot.wings.second = 27;
    bot.h = 0;


    for (int i = 0; i <= (player.wings.second - player.wings.first); i++) {
        changeChar(player.wings.first + i,player.h,'=',player.color);
    }

    for (int i = 0; i <= (bot.wings.second - bot.wings.first); i++) {
        changeChar(bot.wings.first + i, bot.h, '=', bot.color);
    }

    ball.direction = { 1,1 };

    auto pp = randomDot();
    ball.cords[0] = { pp.first,pp.second };
    ball.cords[1] = { pp.first+1,pp.second };
    ball.cords[2] = { pp.first,pp.second+1};
    ball.cords[3] = { pp.first+1,pp.second +1};


    for (auto p : ball.cords) {
        changeChar(p.first, p.second, '9',33);
    }

}

void updateBar(int diff, Bar& user) {
    cnt++;
    if (cnt == 2) {
        int akdjas = 0;
    }
    if (user.wings.first + diff > 0 && user.wings.second + diff < width) {
        if (diff > 0) {
            changeChar(user.wings.first, user.h, ' ', 0);
            changeChar(user.wings.second + diff, user.h, '=', user.color);
        }
        else {
            changeChar(user.wings.second, user.h, ' ', 0);
            changeChar(user.wings.first + diff, user.h, '=', user.color);
        }
        user.wings.first += diff;
        user.wings.second += diff;
    }
}

void updateBot() { //In order to calculate the point of meeting between the ball the line y=0, i've used basic analytic geometry.
    if (ball.direction.first == 1) {
        int target = ball.cords[0].first - ball.cords[0].second;
        target = max(target, 0);
        if (bot.wings.first > target) {
            updateBar(-1, bot);
        }
        else if (bot.wings.second < target) {
            updateBar(1, bot);
        }
    }
    else {
        int target = ball.cords[0].first + ball.cords[0].second;
        target = min(target, width - 1);
        if (bot.wings.first > target) {
            updateBar(-1, bot);
        }
        else if (bot.wings.second < target) {
            updateBar(1, bot);
        }
    }
}

bool updateGame(){
    if (ball.cords[2].second == height-1) {
        if (ball.direction.second == 1 && (ball.cords[1].first < player.wings.first || ball.cords[0].first > player.wings.second)) {
            return 0;
        }
        ball.direction.second *= -1;
    }
    if (ball.cords[0].first == 2 || ball.cords[1].first == width-1) {
        ball.direction.first *= -1;
    }
    if (ball.cords[0].second == 1) {
        if (ball.direction.second == -1 && (ball.cords[1].first < bot.wings.first || ball.cords[0].first > bot.wings.second)) {
            return 0;
        }
        ball.direction.second *= -1;
    }
    for (auto p : ball.cords) {
        changeChar(p.first, p.second, ' ',0);
    }
    for (int i = 0; i < 4; i++) {
        ball.cords[i].first += ball.direction.first;
        ball.cords[i].second += ball.direction.second;
    }
    for (auto p : ball.cords) {
        changeChar(p.first, p.second, '9',33);
    }
}

int main()
{
    ios::sync_with_stdio(0); cout.tie(0);
    cout << "Please choose a lvl of your choose to the bot, between 1 to 4" << '\n';
    cin >> botLvl;
    system("cls");
    init();
    char input;

    auto gameT = steady_clock::now();
    auto playerT = steady_clock::now();
    auto botT = steady_clock::now();

    while (1) {
        if (duration_cast<milliseconds>(steady_clock::now() - playerT).count() >= 5) {
            if (_kbhit()) {
                input = _getch();
                switch (input)
                {
                    case 77:
                        updateBar(1, player);
                        break;
                    case 75:
                        updateBar(-1, player);
                        break;
                }
            }
            playerT = steady_clock::now();
        }
        updateBot();
        if (duration_cast<milliseconds>(steady_clock::now() - botT).count() >= 50 / pow(2, botLvl)) {
            botT = steady_clock::now();
        }
        if (duration_cast<milliseconds>(steady_clock::now() - gameT).count() >= 100) {
            if (!updateGame()) {
                break;
            }
            gameT = steady_clock::now();
        }
    }
    system("cls");
    printf_s("Game Over");
}


//Direct hit with the bar sents y to (-y)
//Direct hit with the wall sents x to (-x)

//Size of ball : 4 chars
//The higher bar belongs to the computer meanwhile the lower is the player.

//First of - the player against the wall.
