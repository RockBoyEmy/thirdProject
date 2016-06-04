#include "Player.h"
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <random>
#include <chrono>
using namespace std;

//getters
string& Player::getName()
{
	return playerName;
}
int& Player::getScore()
{
	return playerScore;
}
int& Player::getBullAccuracy()
{
	return bullAccuracy;
}
int& Player::getSingleAccuracy() 
{
	return singleAccuracy;
}

//setters
void Player::setName(string& name)
{
	playerName = name;
}
void Player::setScore(int& score)
{
	playerScore = score;
}
void Player::setBullAccuracy(unsigned int& bull_accuracy)
{
	bullAccuracy = bull_accuracy;
}
void Player::setSingleAccuracy(unsigned int& single_accuracy)
{
	singleAccuracy = single_accuracy;
}

void Player::setAllAccuracy(unsigned int & all_accuracy)
{
	if (all_accuracy <= 99 && all_accuracy > 89)
	{
		bullAccuracy = all_accuracy - 10;
		singleAccuracy = all_accuracy - 4;
	}
	else if (all_accuracy <= 89 && all_accuracy > 79)
	{
		bullAccuracy = all_accuracy - 7;
		singleAccuracy = all_accuracy - 3;
	}
	else if (all_accuracy <= 79 && all_accuracy > 64)
	{
		bullAccuracy = all_accuracy - 5;
		singleAccuracy = all_accuracy - 2;
	}
	else if (all_accuracy <= 64 && all_accuracy > 50)
	{
		bullAccuracy = all_accuracy - 3;
		singleAccuracy = all_accuracy -1;
	}
	else
	{
		bullAccuracy = all_accuracy;
		singleAccuracy = all_accuracy;
	}
}

//throw functions
int Player::bull(const vector<int>& theDartboard)
{
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count(); //seeds high precision time for mt19937 randomizer
	mt19937 mt_rand(seed); //uses the previous variable for seeding the function in all subsequent calls
	int randomization = (mt_rand() % 100);
	int forTheOuter = getBullAccuracy() + 4; //sets an interval for for outer bull

	if (randomization <= getBullAccuracy())
	{
		return 50;
	}
	
	/*remaining accuracy % is split in two: one interval for hitting outer bull
	and the other for the rest of board numbers*/
	else if (randomization > getBullAccuracy() && randomization <= forTheOuter) 
	{
		return 25;
	}
	else
	{
		int dartPosRand = (mt_rand() % theDartboard.size());
		return theDartboard[dartPosRand];
	}
}
int Player::singleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int& target)
{
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count(); //seeds high precision time for mt19937 randomizer
	mt19937 mt_rand(seed); //uses the previous variable for seeding the function in all subsequent calls
	int randomization = (mt_rand() % 100);
	if (target == 25)
	{
		int outerAccuracy = getBullAccuracy() + 4; //sets an interval for the outer bull (slightly more difficult to hit than the bull)
		if (randomization <= outerAccuracy)
		{
			return 25;
		}
		else
		{
			/*remaining accuracy % is split in two: one interval for hitting the bull
			and the other for the rest of board numbers*/
			int half = (100 - outerAccuracy) / 2;
			if (randomization > outerAccuracy && randomization <= (outerAccuracy + half)) 
			{
				return 50;
			}
			else
			{
				int dartPosRand = (mt_rand() % theDartboard.size());
				return theDartboard[dartPosRand];
			}
		}
	}
	else if (randomization <= getSingleAccuracy())
	{
		return target;
	}
	else
	{
		/*accuracy difference here is split in 4 parts, for hitting single at left and right, 
		or hitting the double or treble of the target*/
		int aFourth = (100 - getSingleAccuracy()) / 4;
		if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + aFourth))
		{
			bIter = find(theDartboard.begin(), theDartboard.end(), target);
			if (bIter == theDartboard.begin())
			{
				bIter = theDartboard.end() - 1; //exception if the chosen number is the first of the vector
			}
			else
			{
				bIter--; //hit the dartboard number left of chosen one
			}
			return *bIter;
		}
		else if (randomization > (getSingleAccuracy() + aFourth) && randomization <= (getSingleAccuracy() + 2 * aFourth))
		{
			bIter = find(theDartboard.begin(), theDartboard.end(), target);
			if (bIter == (theDartboard.end() - 1))
			{
				bIter = theDartboard.begin(); //exception if the chosen number is the last of the vector
			}
			else
			{
				bIter++; //makes sure to hit the dartboard number right of chosen one
			}
			return *bIter;
		}
		else if (randomization > (getSingleAccuracy() + 2 * aFourth) && randomization <= (getSingleAccuracy() + 3 * aFourth))
		{
			return target * 2; //hits the double of the intended number
		}
		else
		{
			return target * 3; //hits the treble of the intended number
		}
	}
}
int Player::doubleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int & target)
{
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count(); //seeds high precision time for mt19937 randomizer
	mt19937 mt_rand(seed); //uses the previous variable for seeding the function in all subsequent calls
	int randomization = (mt_rand() % 100);
	
	/*Splits % difference in 3 parts, for hitting the single,
	or the double at the left or right of the intended target*/
	int aThird = (100 - getSingleAccuracy()) / 3;
	if (randomization <= getSingleAccuracy())
	{
		return target * 2; //hits correct double
	}
	else if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + aThird)) //first alternative interval
	{
		return target; //hits single
	}
	else if (randomization > (getSingleAccuracy() + aThird) && randomization <= (getSingleAccuracy() + 2 * aThird)) //second alternative interval
	{
		bIter = find(theDartboard.begin(), theDartboard.end(), target);
		if (bIter == theDartboard.begin())
		{
			bIter = theDartboard.end() - 1; //exception if the chosen number is the first of the vector
		}
		else
		{
			bIter--; //makes sure to hit the dartboard number left of chosen one
		}
		return *bIter * 2; //hits the double at the left
	}
	else
	{
		bIter = find(theDartboard.begin(), theDartboard.end(), target); //third alternative interval
		if (bIter == (theDartboard.end() - 1))
		{
			bIter = theDartboard.begin();
		}
		else
		{
			bIter++; //makes sure to hit the dartboard number right of chosen one
		}
		return *bIter * 2; //hits double at the right
	}
}
int Player::trebleThrow(const vector<int>& theDartboard, vector<int>::const_iterator bIter, unsigned int & target)
{
	auto seed = chrono::high_resolution_clock::now().time_since_epoch().count(); //seeds high precision time for mt19937 randomizer
	mt19937 mt_rand(seed); //uses the previous variable for seeding the function in all subsequent calls
	int randomization = (mt_rand() % 100);
	
	/*Splits accuracy % difference in 4 parts: 2 for hitting singles above and below 
	the treble, and 2 for trebles left and right of desired one*/
	int aFourth = (100 - getSingleAccuracy()) / 4;
	if (randomization <= getSingleAccuracy())
	{
		return target * 3; //hits correct treble
	}
	else if (randomization > getSingleAccuracy() && randomization <= (getSingleAccuracy() + 2 * aFourth))
	{
		return target; //hits single
	}
	else if (randomization > (getSingleAccuracy() + 2 * aFourth) && randomization <= (getSingleAccuracy() + 3 * aFourth))
	{
		bIter = find(theDartboard.begin(), theDartboard.end(), target);
		if (bIter == theDartboard.begin())
		{
			bIter = theDartboard.end() - 1; //exception if the chosen number is the first of the vector
		}
		else
		{
			bIter--;
		}
		return *bIter * 3; //hits the treble at the left
	}
	else
	{
		bIter = find(theDartboard.begin(), theDartboard.end(), target);
		if (bIter == (theDartboard.end() - 1))
		{
			bIter = theDartboard.begin(); //exception if the chosen number is the last of the vector
		}
		else
		{
			bIter++;
		}
		return *bIter * 3; //hits treble at the right
	}
}

Player::Player()	
{
	playerScore = 501;
	bullAccuracy = 0;
	singleAccuracy = 0;
	gamesWon = 0;
	setsWon = 0;
	matchesWon = 0;
	nTurns = 3;
}
Player::~Player()
{

}
