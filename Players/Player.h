#ifndef PLAYER_H_
#define PLAYER_H_

class Player
{
protected:
    int type;
    string name;
    string symbol;
public:
    Player(int t, string s, string n) : type(t), symbol(s), name(n)
    {}

    string GetName()
    {
        return name;
    }

    string GetSymbol()
    {
        return symbol;
    }

    int GetType()
    {
        return type;
    }

    virtual bool GetMove(Board *board, int &x, int &y) = 0;


    virtual ~Player()
    {
    }
};

#endif /* PLAYER_H_ */
