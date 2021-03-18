/*
 * Player.h
 *
 *  Created on: 21/02/2021
 *      Author: dongmo
 */

#ifndef PLAYER_H_
#define PLAYER_H_
class Player {
protected:
	int type;
	string name;
public:
	Player(int t, string i) :
			type(t), name(i) {
	}

	string getPlayerName() {
		return name;
	}

	int getType() {
		return type;
	}

	virtual bool getMove(Board *board, int &x, int &y) = 0;

	virtual ~Player() {
	}
};

#endif /* PLAYER_H_ */
