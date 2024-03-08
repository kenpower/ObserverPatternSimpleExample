// ObserverPatternSimpleExample.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include <map>
#include <windows.h>
using namespace std;

enum Command {
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	FIRE
};

class Observer {
public:
    	virtual void update(Command) = 0;
};

class Subject {
    vector<Observer*> observers;

public:
    void addObserver(Observer* observer) {
		observers.push_back(observer);
	}
    void removeObserver(Observer* observer) {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }
    void notifyAll(Command c) {
        for (auto observer : observers) {
			observer->update(c);
		}
	}
};

class InputChecker : public Subject{
	map<UINT, Command> keyCodeCommandMap = {
		{VK_LEFT, Command::MOVE_LEFT},
		{VK_UP, Command::MOVE_UP},
		{VK_RIGHT, Command::MOVE_RIGHT},
		{VK_DOWN, Command::MOVE_DOWN},
		{'A', Command::MOVE_LEFT},
		{'S', Command::MOVE_DOWN},
		{'D', Command::MOVE_RIGHT},
		{'W', Command::MOVE_UP},
		{VK_SPACE, Command::FIRE}
		};
public:
	void getInput() {
		for (auto key_command : keyCodeCommandMap) {
			if (GetAsyncKeyState(key_command.first)<0) {
				notifyAll(key_command.second);
			}
		}

	}
};

class Score : public Observer {
    int score = 0;
	int level = 1;
public:
    void update(Command _) override {
        score += 1; 
		if(score%10==0)
			cout << "Level: " << level++ << endl;
	}
};

class AudioSystem : public Observer {
	void fireSound() {
		// Start frequency at 500 Hz, end at 1000 Hz
		int startFrequency = 500;
		int endFrequency = 2000;
		int duration = 25; // Duration of each beep in milliseconds

		// Increase the frequency with each beep to create a "power-up" effect
		for (int frequency = startFrequency; frequency <= endFrequency; frequency *=1.2) {
			Beep(frequency, duration);
		}
	}
public:	
	void update(Command c) override {
		if (c==Command::FIRE) {
			fireSound();
		}
	}
};


class Mario : public Observer {
	int posx = 0;
	int posy = 0;

public:
	void update(Command c) override {
		switch (c)
		{
		case MOVE_LEFT:
			posx--; break;
		case MOVE_RIGHT:
			posx++; break;
		case MOVE_UP:
			posy++; break;
		case MOVE_DOWN:
			posy--; break;
		}
		cout << "Mario has moved to position: " << posx << ", " << posy << endl;
	}
};

int main() {
	InputChecker ic;
	
	Score score;
	AudioSystem audio;
	Mario mario;

	ic.addObserver(&score);
	ic.addObserver(&audio);
	ic.addObserver(&mario);

	while (true) { // Game loop
		ic.getInput();
		Sleep(100);
	}
	return 0;
}
