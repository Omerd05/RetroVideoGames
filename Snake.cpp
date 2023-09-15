#include <iostream>
#include <conio.h>
#include <chrono>
#include <vector>
#include <set>
#include <map>
#include <deque>
#include <random>
#include <thread>

using namespace std;
using namespace chrono;


const int N = 15;
const int M = 25;

vector<vector<char>> gameMap(N, vector<char>(M));
uniform_int_distribution<int> xGen(5, 95);
uniform_int_distribution<int> yGen(5, 25);
random_device rd;
mt19937 rng(rd());

deque<pair<int, int>> cords;
set<pair<int, int>> occupied;
pair<int, int> apple;

pair<int, int> randomDot() {
	return { xGen(rng),yGen(rng) };
}

void changeChar(int x, int y, char ch) {
	cout << "\033[" << y << ";" << x << "H";
	cout << ch;
}

void spawnApple() {
	cout << "\033[31m";
	auto it = randomDot();
	while (occupied.count(it))it = randomDot();
	apple = it;
	changeChar(apple.first, apple.second, '0');
	std::cout << "\033[0m";
}

void printBorder() {
	for (int i = 0; i < 100; i++) {
		changeChar(i, 0, 'Q');
		changeChar(i, 30, 'Q');
	}
	for (int i = 0; i < 30; i++) {
		changeChar(0, i, 'Q');
		changeChar(100, i, 'Q');
	}
}

void init() {
	int x, y;
	auto it = randomDot();
	x = it.first;
	y = it.second;
	cords.push_front({ x,y });
	occupied.insert({ x,y });
	changeChar(x,y,'X');


	spawnApple();
	printBorder();
}

bool update(pair<int,int> p) {
	auto it = cords.front();
	pair<int,int> face = { it.first + p.first,it.second + p.second };
	if (occupied.count(face) || (face.first == 0 || face.first > 99) || (face.second == 0 || face.second >= 30))return 0;
	
	cords.push_front(face);
	occupied.insert(face);

	
	if (face == apple) { //double check correctness
		spawnApple();
	}
	else {
		changeChar(cords.back().first, cords.back().second,' ');
		occupied.erase(cords.back());
		cords.pop_back();
	}
	changeChar(face.first, face.second, 'X');
	return 1;
}

int main()
{
	ios::sync_with_stdio(0); cout.tie(0);
	init();

	char input;
	bool running = 1;


	pair<int, int> op = {-1,0};

	auto speedT = steady_clock::now();
	auto fpsT = steady_clock::now();

	int pace = 100;

	while (running) {
		if (duration_cast<milliseconds>(steady_clock::now() - speedT).count() >= pace/sqrt(cords.size())) {
			if (_kbhit()) {
				input = _getch();
				if (input == 0 || input == -32) {
					input = _getch();
					pair<int, int> tempo = { op.first,op.second };
					switch (input) {
					case 72: //Up
						op = { 0,-1 };
						break;
					case 75: //Left
						op = { -1,0 };
						break;
					case 80: //Down
						op = { 0,1};
						break;
					case 77: //Right
						op = { 1,0 };
						break;
					}
					if (tempo.first + op.first == 0 && tempo.second + op.second == 0)op = tempo;
				}
			}
			if(!update(op))return 0;
			speedT = steady_clock::now();
		}
		this_thread::sleep_for(milliseconds(10));
	}

	system("cls");
	cout << "Game Over"; cin;
}
