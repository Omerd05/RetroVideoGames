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
uniform_int_distribution<int> xGen(2, 23);
uniform_int_distribution<int> yGen(2, 13);
random_device rd;
mt19937 rng(rd());

deque<pair<int, int>> cords;
set<pair<int, int>> occupied;


pair<int, int> randomDot() {
	return { yGen(rng),xGen(rng) };
}

void init() {
	for (int i = 1; i < N - 1; i++) {
		for (int j = 1; j < M - 1; j++) {
			gameMap[i][j] = '*';
		}
	}
	for (int i = 0; i < N; i++) {
		gameMap[i][0] = '|',gameMap[i][M - 1] = '|';
	}
	for (int i = 0; i < M; i++) {
		gameMap[0][i] = '_',gameMap[N - 1][i] = '-';
	}
	int x, y;
	auto it = randomDot();
	y = it.first;
	x = it.second;
	cords.push_front({ y,x });
	occupied.insert({ y,x });
	gameMap[y][x] = 'X';

	it = randomDot();
	while(it.first == y && it.second == x)it = randomDot();
	gameMap[it.first][it.second] = 'O';
}

void printMap() {
	cout << "\033[2J\033[1;1H";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << gameMap[i][j];
		}
		cout << '\n';
	}
	cout << "Score : " << cords.size();
}

bool update(pair<int,int> p) {
	auto it = cords.front();
	pair<int,int> face = { it.first + p.first,it.second + p.second };
	if (occupied.count(face) || (face.first == 0 || face.first == N-1) || (face.second == 0 || face.second == M-1))return 0;
	
	cords.push_front(face);
	occupied.insert(face);
	
	if (gameMap[face.first][face.second] == 'O') {
		pair<int, int> apl = randomDot();
		int t = 0;
		while (occupied.count(apl)) {
			apl = randomDot();
			if (t++ >= 100)return 0;
		}
		gameMap[apl.first][apl.second] = 'O';
	}
	else {
		gameMap[cords.back().first][cords.back().second] = '*';
		occupied.erase(cords.back());
		cords.pop_back();
	}
	gameMap[face.first][face.second] = 'X';
	printMap();
	return 1;
}

int main()
{
	init();

	char input;
	bool running = 1;


	pair<int, int> op = {-1,0};

	auto speedT = steady_clock::now();
	auto fpsT = steady_clock::now();

	int pace = 500;

	while (running) {
		if (duration_cast<milliseconds>(steady_clock::now() - speedT).count() >= pace/sqrt(cords.size())) {
			if (_kbhit()) {
				input = _getch();
				if (input == 0 || input == -32) {
					input = _getch();
					pair<int, int> tempo = { op.first,op.second };
					switch (input) {
					case 72: //Up
						op = { -1,0 };
						break;
					case 75: //Left
						op = { 0,-1 };
						break;
					case 80: //Down
						op = { 1,0 };
						break;
					case 77: //Right
						op = { 0,1 };
						break;
					}
					if (tempo.first + op.first == 0 && tempo.second + op.second == 0)op = tempo;
				}
			}
			if(!update(op))return 0;
			printMap();
			speedT = steady_clock::now();
		}
		this_thread::sleep_for(milliseconds(10));
	}

	cout << "\033[2J\033[1;1H";
	cout << "Game Over"; cin;
}
