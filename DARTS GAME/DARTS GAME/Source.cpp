#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits> //for handling input error situations
#include <iomanip> //precision settings
#include <conio.h> //for kbhit()
#include <Windows.h>
#include "Player.h"
#define SCROLL_POS   34  // the point that scrolling occurs
using namespace std;

//graphics function prototypes
void Init_Graphics(void);
inline void Set_Color(int fcolor, int bcolor);
inline void Draw_String(int x, int y, char *string);
inline void Interface_Init(vector<Player>& player);

CONSOLE_SCREEN_BUFFER_INFO con_info; // holds screen info
HANDLE hconsole; // handle to console

int main()
{
	const int TOTAL_MATCHES = 100, MAX_THROWS_TURN = 3;
	unsigned int bAccuracy = 0, sAccuracy = 0, targetNo = 0, simOrPlay = 0;
	int tempScore = 0, tempCalc = 0, gameLen = 0, gameRunning = 1;
	bool isLoopGoing = false;
	string playerName = "";
	vector<int> dartboard = { 20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5 };
	vector<int>::const_iterator bIter;
	vector<int> doublesLeaveDoubles = { 40, 36, 32, 28, 24, 20, 16, 12, 8, 4 }; //stored in decrescent order to maximize efficiency in loop checks
	vector<int>::const_iterator dLd;
	vector<int> scoreCombinations(14); //stores all the possible Joe:Sid set winning combinations: 7:6, 6:7, 5:7, 4:7, 3:7, 2:7, 1:7 and viceversa
	vector<string> stringCombinations = { "7:0", "7:1", "7:2", "7:3", "7:4", "7:5", "7:6", \
		"6:7", "5:7", "4:7", "3:7", "2:7", "1:7", "0:7" };

	vector<Player> player(2);
	vector<Player>::iterator modIter;

	cout << "Press 1 for a simulation of the Finals, or 2 for playing the Finals yourself agains the PC! ";
	while (gameRunning)
	{
		cin >> simOrPlay;
		if (simOrPlay == 1)
		{
			Init_Graphics(); //initializing graphics engine
			cout << "Simulation chosen" << endl << endl;
			Sleep(500);
			system("cls");
			Sleep(100);
			cout << "Welcome to the World Dartboard Championship!" << endl << endl;

			for (int i = 0; i < player.size(); i++) 
			{
				//setting players' names
				cout << "Player " << (i + 1) << " name is: ";
				cin >> playerName;
				player[i].setName(playerName);
			}
			cout << "\nWould you prefer to manually set the accuracy values, set a master accuracy," << endl;
			cout << " or choose it from a randomization ? (1, 2 or 3) ";
			int accChoice = 0;
			isLoopGoing = true;
			while (isLoopGoing)
			{
				cin >> accChoice;
				switch (accChoice)
				{
					case 1:
					{
						cout << "\nSet the players' bull accuracy:" << endl;
						for (modIter = player.begin(); modIter != player.end(); modIter++)
						{
							cout << modIter->getName() << ": ";
							cin >> bAccuracy;
							modIter->setBullAccuracy(bAccuracy);
						}
						cout << "\nSet the players' single hit accuracy: " << endl;
						for (modIter = player.begin(); modIter != player.end(); modIter++)
						{
							cout << modIter->getName() << ": ";
							cin >> sAccuracy;
							modIter->setSingleAccuracy(sAccuracy);
						}
						isLoopGoing = false;
						break;
					}
					case 2:
					{
						cout << "\nSet the players' master accuracy" << endl;
						for (modIter = player.begin(); modIter != player.end(); modIter++)
						{
							cout << modIter->getName() << ": ";
							cin >> bAccuracy;
							modIter->setAllAccuracy(bAccuracy);
						}
						isLoopGoing = false;
						break;
					}
					case 3:
					{
						int acc[2];
						cout << "Try to aim for the accuracy that you prefer. " << endl;
						char key = '\0';
						for (int i = 0; i < player.size(); i++)
						{
							for (unsigned int j = 0; j <= 100; j++)
							{
								if (j == 100)
								{
									j = 0;
								}
								else if (_kbhit())
								{
									//if user presses either enter key at any time
									key = _getch();
									if (key == 13)
									{
										//sets all accuracies based on the general one
										player[i].setAllAccuracy(j);
										acc[i] = j;
										Draw_String(0, 9, "");
										cout << player[i].getName() << "'s accuracy is now " << j << endl;
										Draw_String(0, 8, "  ");
										Sleep(500);
										break;
									}
								}
								Draw_String(0, 8, "  ");
								Draw_String(0, 8, "");
								cout << j << endl;
								Sleep(50);
							}
						}
						Draw_String(0, 9, "                                                            ");
						Draw_String(0, 9, "");
						cout << player[0].getName() << "'s accuracy is " << acc[0] << endl;
						cout << player[1].getName() << "'s accuracy is " << acc[1] << endl;
						isLoopGoing = false;
						break;
					}
					default:
					{
						//cin.clear() and .ignore() to make sure discards any other type of input different from what required
						Draw_String(0, 7, "Wrong number inserted. Please try again");
						Sleep(1000);
						Draw_String(0, 7, "                                          ");
						cin.clear();
						cin.ignore(1000, '\n');
						Draw_String(47, 7, "  ");
						break;
					}
				}
			}

			cout << "\nPress \"1\" for 501 game, or \"2\" for 701 game: ";
			isLoopGoing = true;
			while (isLoopGoing)
			{
				cin >> gameLen;
				switch (gameLen)
				{
					case 1:
					{
						//the score for each player will remain default (501) as already initialized by the constructor
						//and then ready to be resetted by gameLen at the end of each game
						gameLen = 501;
						cout << "501 game was chosen!" << endl;
						isLoopGoing = false;
						break;
					}
					case 2:
					{
						gameLen = 701;
						player[0].setScore(gameLen);
						player[0].setScore(gameLen);
						cout << "701 game was chosen!" << endl;
						isLoopGoing = false;
						break;
					}
					default:
					{
						//cin.clear() and .ignore() to make sure discards any other type of input different from what required
						cout << "Invalid choice inserted. Please try again " << endl;
						Sleep(1000);
						cin.clear(); 
						cin.ignore(1000, '\n');
						break;
					}
				}
			}

			cout << "\nThe players will now attempt a 50:50 game to decide who will start first: " << endl;
			isLoopGoing = true;
			while (isLoopGoing)
			{
				modIter = player.begin();
				int tempResult1 = modIter->bull(dartboard);
				modIter++;
				int tempResult2 = modIter->bull(dartboard);

				if (tempResult1 == tempResult2)
				{
					continue;
				}
				else if (tempResult1 > tempResult2)
				{
					modIter = player.begin();
					cout << modIter->getName() << " will start. " << endl;
					isLoopGoing = false;
				}
				else
				{
					cout << modIter->getName() << " will start. " << endl;
					isLoopGoing = false;
				}
			}

			char simYN = '\0';
			while (true)
			{
				cout << "Start simulation? (y/n) ";
				cin >> simYN;
				if (simYN == 'y' || simYN == 'Y')
				{
					for (int i = 0; i < TOTAL_MATCHES; i++)
					{
						player[0].setsWon = 0;
						player[1].setsWon = 0;
						//when any player reaches 7 sets won in one match (can't lose the match anymore), the loop breaks
						while (player[0].setsWon < 7 && player[1].setsWon < 7)
						{
							player[0].gamesWon = 0;
							player[1].gamesWon = 0;
							//when any player reaches 3 games won in a set of 5 (can't lose the set anymore), the loop breaks
							while (player[0].gamesWon < 3 && player[1].gamesWon < 3) 
							{
								//real game begins - iterator position doesen't change, so same player who won the 50:50 will start, then keep alternating
								for (; modIter <= player.end(); modIter++) 
								{
									//if the end of the vector was reached, start again to keep going
									if (modIter == player.end()) 
									{
										modIter = player.begin();
									}

									//saves current score in case any of the 3 shots is invalid
									tempScore = modIter->getScore(); 
									while (modIter->getScore() >= 2 && modIter->nTurns != 0)
									{
										//start of AI action
										for (dLd = doublesLeaveDoubles.begin(); dLd != doublesLeaveDoubles.end(); dLd++)
										{
											if (modIter->getScore() == (*dLd))
											{
												//if score is equal to a double-leaving-double, goes for THAT first
												targetNo = (*dLd) / 2;
												tempCalc = (modIter->getScore() - modIter->doubleThrow(dartboard, bIter, targetNo));
												modIter->setScore(tempCalc);
												goto skipifs;
											}
										}
										for (bIter = dartboard.begin(); bIter != dartboard.end(); bIter++)
										{											
											if (modIter->getScore() == ((*bIter) * 2)) 
											{
												//if not then check if score is equal to ANY of the doubles, go for THAT shot to end the game
												targetNo = *bIter;
												tempCalc = (modIter->getScore() - modIter->doubleThrow(dartboard, bIter, targetNo));
												modIter->setScore(tempCalc);
												goto skipifs;
											}
										}
										if (modIter->getScore() == 50) 
										{
											//if able to win with a bull, go for that shot
											tempCalc = (modIter->getScore() - modIter->bull(dartboard));
											modIter->setScore(tempCalc);
										}
										if (modIter->getScore() > 50)
										{
											for (bIter = dartboard.begin(); bIter != dartboard.end(); bIter++)
											{
												//checks if there's any double or treble possible that will leave
												//a double-leaving-double (most efficient even in case of fail) to then end with
												for (dLd = doublesLeaveDoubles.begin(); dLd != doublesLeaveDoubles.end(); dLd++) 
												{
													if (modIter->getScore() == (((*bIter) * 3) + (*dLd)))
													{
														targetNo = *bIter;
														tempCalc = (modIter->getScore() - modIter->trebleThrow(dartboard, bIter, targetNo));
														modIter->setScore(tempCalc);
														goto skipifs;
													}
													else if (modIter->getScore() == (((*bIter) * 2) + (*dLd)))
													{
														targetNo = *bIter;
														tempCalc = (modIter->getScore() - modIter->doubleThrow(dartboard, bIter, targetNo));
														modIter->setScore(tempCalc);
														goto skipifs;
													}
												}

												//checks then if there's any double or treble possible that will leave
												//a bull to win the game, and goes for that (for question 2, 3 & 4 in adv AI)
												if (modIter->getScore() == (((*bIter) * 3) + 50)) 
												{
													targetNo = *bIter;
													tempCalc = (modIter->getScore() - modIter->trebleThrow(dartboard, bIter, targetNo));
													modIter->setScore(tempCalc);
													goto skipifs;
												}
												else if (modIter->getScore() == (((*bIter) * 2) + 50))
												{
													targetNo = *bIter;
													tempCalc = (modIter->getScore() - modIter->doubleThrow(dartboard, bIter, targetNo));
													modIter->setScore(tempCalc);
													goto skipifs;
												}
											}
										}
										if (modIter->getScore() >= 62)
										{
											//in all the other cases, until down to this score, go only for the highest treble (3x20 - 60)
											targetNo = 20;
											tempCalc = (modIter->getScore() - modIter->trebleThrow(dartboard, bIter, targetNo));
											modIter->setScore(tempCalc);
										}
										else if (modIter->getScore() < 62 && modIter->getScore() > 50)  
										{
											//goes for the single throw equal to the score difference,
											//thus it can try to end with a bull at next throw
											targetNo = modIter->getScore() - 50;
											tempCalc = (modIter->getScore() - modIter->singleThrow(dartboard, bIter, targetNo));
											modIter->setScore(tempCalc);
										}
										else if (modIter->getScore() < 50)
										{
											//when score is under 4, must only end with double 1
											if (modIter->getScore() == 3)
											{
												targetNo = 1;
												tempCalc = (modIter->getScore() - modIter->singleThrow(dartboard, bIter, targetNo));
												modIter->setScore(tempCalc);
												goto skipifs;
											}

											//otherwise tries to get close to a double-leaving double by aiming at the difference
											for (dLd = doublesLeaveDoubles.begin(); dLd != doublesLeaveDoubles.end(); dLd++)
											{
												for (int j = 1; j <= 9; j++)
												{
													if (modIter->getScore() == ((*dLd) + j)) 
													{
														targetNo = j;
														tempCalc = (modIter->getScore() - modIter->singleThrow(dartboard, bIter, targetNo));
														modIter->setScore(tempCalc);
														dLd = doublesLeaveDoubles.end() - 1; //in order to end outer loop immediately
														break;
													}
												}
											}
										}

									//only usage of goto redirection - highly needed to exit the tricky previous loop checks
									skipifs:
										if (modIter->getScore() == 0) 
										{
											modIter->gamesWon++;
											break;
										}
										modIter->nTurns--;
									}
									if (modIter->getScore() == 0)
									{
										//scores get restored
										player[0].setScore(gameLen);
										player[1].setScore(gameLen);
										break;
									}
									else if (modIter->getScore() < 2 && modIter->getScore() != 0)
									{
										//resets the previously held score because score got under 2
										modIter->setScore(tempScore); 
										modIter->nTurns = MAX_THROWS_TURN;
									}
									else
									{
										modIter->nTurns = MAX_THROWS_TURN;
									}
								}
								modIter++; //next player will start
							}
							if (player[0].gamesWon == 3)
							{
								player[0].setsWon++;
							}
							else
							{
								player[1].setsWon++;
							}

							if (modIter == player.end())
							{
								modIter = player.begin(); //if the end of the vector was reached, start again
							}
						}
						if (player[0].setsWon == 7)
						{
							player[0].matchesWon++;
						}
						else
						{
							player[1].matchesWon++;
						}

						//counters for the sets winning percentage
						{
							if (player[0].setsWon == 7 && player[1].setsWon == 0) {
								scoreCombinations[0]++;
							}
							else if (player[0].setsWon == 7 && player[1].setsWon == 1) {
								scoreCombinations[1]++;
							}
							else if (player[0].setsWon == 7 && player[1].setsWon == 2) {
								scoreCombinations[2]++;
							}
							else if (player[0].setsWon == 7 && player[1].setsWon == 3) {
								scoreCombinations[3]++;
							}
							else if (player[0].setsWon == 7 && player[1].setsWon == 4) {
								scoreCombinations[4]++;
							}
							else if (player[0].setsWon == 7 && player[1].setsWon == 5) {
								scoreCombinations[5]++;
							}
							else if (player[0].setsWon == 7 && player[1].setsWon == 6) {
								scoreCombinations[6]++;
							}
							else if (player[0].setsWon == 6 && player[1].setsWon == 7) {
								scoreCombinations[7]++;
							}
							else if (player[0].setsWon == 5 && player[1].setsWon == 7) {
								scoreCombinations[8]++;
							}
							else if (player[0].setsWon == 4 && player[1].setsWon == 7) {
								scoreCombinations[9]++;
							}
							else if (player[0].setsWon == 3 && player[1].setsWon == 7) {
								scoreCombinations[10]++;
							}
							else if (player[0].setsWon == 2 && player[1].setsWon == 7) {
								scoreCombinations[11]++;
							}
							else if (player[0].setsWon == 1 && player[1].setsWon == 7) {
								scoreCombinations[12]++;
							}
							else if (player[0].setsWon == 0 && player[1].setsWon == 7) {
								scoreCombinations[13]++;
							}

						}
					}
					break; //stops the simulation loop, after all matches are concluded
				}
				else if (simYN == 'n' || simYN == 'N')
				{
					cout << "Ok then, not yet. Have some time to prepare for it." << endl;
					Sleep(10000);
				}
				else
				{
					cout << "Wrong key pressed." << endl;
				}
			}

			//results and percentages output
			cout << "\nSimulation ended. Players' results:" << endl;
			cout << player[0].getName() << ": " << player[0].matchesWon << " matches won" << endl;
			cout << player[1].getName() << ": " << player[1].matchesWon << " matches won" << endl;
			cout << "\nJoe:Sid\t\tFrequency" << endl;
			for (unsigned int i = 0; i<scoreCombinations.size(); i++) 
			{
				cout << string(2, ' ') << stringCombinations[i] << "\t\t" << string(3, ' ') << setprecision(5) << (((float)scoreCombinations[i]) / TOTAL_MATCHES) * 100 << "%" << endl;
			}
			gameRunning = 0; //will finish the main program
		}
		else if (simOrPlay == 2)
		{
			Init_Graphics(); //initializing graphics engine
			system("cls");
			cout << "Interactive game chosen" << endl;
			Sleep(1000);
			system("cls");
			Sleep(500);
			Set_Color(15, 0);
			Draw_String(38, 1, "Welcome to the World Dartboard Championship!");
			Draw_String(47, 3, "Insert the user's name: ");
			cin >> playerName;
			player[0].setName(playerName); //this way modIter.begin() will always be the user, and the computer will be begin() + 1
			Draw_String(43, 4, "Insert a name for the computer AI: ");
			cin >> playerName;
			player[1].setName(playerName);
			
			Draw_String(21, 6, "Would you prefer to manually set the accuracy values, set a master accuracy, ");
			Draw_String(36, 7, "or choose it from a randomization? (1, 2 or 3) ");
			Draw_String(30, 9, "Manual");
			Draw_String(57, 9, "Master");
			Draw_String(83, 9, "Random");
			Draw_String(83, 7, "");
			int accChoice = 0;
			isLoopGoing = true;
			while (isLoopGoing)
			{
				cin >> accChoice;
				Draw_String(83, 7, "  ");
				switch (accChoice)
				{
					case 1:
					{
						Draw_String(34, 10, "^");
						Draw_String(34, 11, "^");
						Sleep(500);
						Draw_String(45, 13, "Set the players' bull accuracy:");
						for (modIter = player.begin(); modIter != player.end(); modIter++)
						{
							Draw_String(56, 14, "                    ");
							Draw_String(56, 14, "");
							cout << modIter->getName() << ": ";
							cin >> bAccuracy;
							modIter->setBullAccuracy(bAccuracy);
						}
						Sleep(100);
						Draw_String(45, 13, "                                 ");
						Draw_String(44, 13, "Set the players' single accuracy:");
						for (modIter = player.begin(); modIter != player.end(); modIter++)
						{
							Draw_String(56, 14, "                    ");
							Draw_String(56, 14, "");
							cout << modIter->getName() << ": ";
							cin >> sAccuracy;
							modIter->setSingleAccuracy(sAccuracy);
						}
						Draw_String(56, 14, "                    ");
						isLoopGoing = false;
						break;
					}
					case 2:
					{
						Draw_String(60, 10, "^");
						Draw_String(60, 11, "^");
						Sleep(500);
						Draw_String(44, 13, "Set the players' master accuracy: ");
						for (modIter = player.begin(); modIter != player.end(); modIter++)
						{
							Draw_String(54, 14, "");
							cout << modIter->getName() << ": ";
							cin >> bAccuracy;
							modIter->setAllAccuracy(bAccuracy);
							Draw_String(54, 14, "                    ");
						}
						isLoopGoing = false;
						break;
					}
					case 3:
					{
						Draw_String(86, 10, "^");
						Draw_String(86, 11, "^");
						Sleep(500);
						Draw_String(29, 13, "Try to aim for the accuracy that you prefer for each player");
						char key = '\0';
						for (int i = 0; i < player.size(); i++)
						{
							for (unsigned int j = 0; j <= 100; j++)
							{
								if (j == 100)
								{
									j = 0;
								}
								else if (_kbhit())
								{
									//if user presses either enter key at any time
									key = _getch();
									if (key == 13)
									{
										//sets all accuracies based on the general one
										player[i].setAllAccuracy(j);
										Draw_String(47, 16, "                                                ");
										Draw_String(47, 16, "");
										cout << player[i].getName() << "'s accuracy is now " << j << endl;
										Draw_String(59, 15, "  ");
										Sleep(500);
										break;
									}
								}
								Draw_String(59, 15, "  ");
								Draw_String(59, 15, "");
								cout << j << endl;
								Sleep(50);
							}
						}
						Draw_String(28, 13, "                                                                      ");
						Draw_String(47, 16, "                                                ");
						isLoopGoing = false;
						break;
					}
					default:
					{
						//cin.clear() and .ignore() to make sure discards any other type of input different from what required
						Set_Color(15, 4);
						Draw_String(40, 13, "Wrong number inserted. Please try again");
						cin.clear();
						cin.ignore(1000, '\n');
						Sleep(1500);
						Set_Color(15, 0);
						Draw_String(40, 13, "                                        ");
						Draw_String(83, 7, "  ");
						Draw_String(83, 7, "");
						break;
					}
				}
			}

			Draw_String(38, 13, "                                                   ");
			Draw_String(38, 13, "Press \"1\" for 501 game, or \"2\" for 701 game: ");
			Draw_String(48, 15, "501");
			Draw_String(68, 15, "701");
			Draw_String(83, 13, "");
			isLoopGoing = true;
			while (isLoopGoing)
			{
				cin >> gameLen;
				Draw_String(83, 13, "  ");
				switch (gameLen)
				{
					case 1:
					{
						//the score for each player will remain default (501) as already initialized by the constructor
						//and then ready to be resetted by gameLen at the end of each game
						Draw_String(49, 16, "^");
						Draw_String(49, 17, "^");
						Sleep(500);
						gameLen = 501;
						isLoopGoing = false;
						break;
					}
					case 2:
					{
						Draw_String(69, 16, "^");
						Draw_String(69, 17, "^");
						Sleep(500);
						gameLen = 701;
						player[0].setScore(gameLen);
						player[0].setScore(gameLen);
						isLoopGoing = false;
						break;
					}
					default:
					{
						//cin.clear() and .ignore() to make sure discards any other type of input different from what required
						Set_Color(15, 4);
						Draw_String(40, 12, "Invalid choice inserted. Please try again");
						cin.clear();
						cin.ignore(1000, '\n');
						Sleep(1500);
						Set_Color(15, 0);
						Draw_String(40, 12, "                                             ");
						Draw_String(83, 13, "");
						break;
					}
				}
			}

			Draw_String(25, 19, "The players will now attempt a 50:50 game to decide who will start first");
			Sleep(1000);
			isLoopGoing = true;
			while (isLoopGoing)
			{
				modIter = player.begin();
				int tempResult1 = modIter->bull(dartboard);
				modIter++;
				int tempResult2 = modIter->bull(dartboard);

				if (tempResult1 == tempResult2)
				{
					continue;
				}
				else if (tempResult1 > tempResult2)
				{
					Draw_String(54, 20, "");
					modIter = player.begin();
					cout << modIter->getName() << " will start";
					isLoopGoing = false;
				}
				else
				{
					Draw_String(54, 20, "");
					cout << modIter->getName() << " will start. ";
					isLoopGoing = false;
				}
			}

			Draw_String(46, 22, "All set, we're ready to begin!");
			Sleep(2000);
			system("cls");
			Sleep(1000);

			while (true)
			{
				for (int i = 0; i < TOTAL_MATCHES; i++)
				{
					player[0].setsWon = 0;
					player[1].setsWon = 0;
					//when any player reaches 7 sets won in one match (can't lose the match anymore), the loop breaks
					while (player[0].setsWon < 7 && player[1].setsWon < 7) 
					{
						player[0].gamesWon = 0;
						player[1].gamesWon = 0;
						//when any player reaches 3 games won in a set of 5 (can't lose the set anymore), the loop breaks
						while (player[0].gamesWon < 3 && player[1].gamesWon < 3) 
						{
							//real game begins - iterator position doesen't change, so same player who won the 50:50 will start, then keep alternating
							for (; modIter <= player.end(); modIter++) 
							{
								//running game interface
								Interface_Init(player);

								//if the end of the player vector was reached, start again
								if (modIter == player.end()) 
								{
									modIter = player.begin();
								}

								//saves current score first, in case any of the 3 shots is invalid
								tempScore = modIter->getScore(); 
								while (modIter->getScore() >= 2 && modIter->nTurns != 0)
								{
									//dartboard illustration
									Draw_String(10, 2, "                 20               ");
									Draw_String(10, 3, "            5          1          ");
									Draw_String(10, 4, "        12                18      ");
									Draw_String(10, 5, "                                   ");
									Draw_String(10, 6, "      9                      4     ");
									Draw_String(10, 7, "                                   ");
									Draw_String(10, 8, "    14           25           13  ");
									Draw_String(10, 9, "                                   ");
									Draw_String(10, 10, "   11         25 50 25          6 ");
									Draw_String(10, 11, "                                   ");
									Draw_String(10, 12, "    8            25           10   ");
									Draw_String(10, 13, "                                   ");
									Draw_String(10, 14, "     16                      15     ");
									Draw_String(10, 15, "                                   ");
									Draw_String(10, 16, "       7                   2      ");
									Draw_String(10, 17, "           19         17          ");
									Draw_String(10, 18, "                 3                ");

									//displaying user's stats
									Set_Color(2, 0);
									Draw_String(100, 2, "    ");
									Draw_String(100, 2, "");
									cout << player[0].getScore();
									Draw_String(100, 3, "    ");
									Draw_String(100, 3, "");
									cout << player[0].getBullAccuracy();
									Draw_String(100, 4, "    ");
									Draw_String(100, 4, "");
									cout << player[0].getSingleAccuracy();

									//displaying AI's stats
									Set_Color(4, 0);
									Draw_String(100, 13, "    ");
									Draw_String(100, 13, "");
									cout << player[1].getScore();
									Draw_String(100, 14, "    ");
									Draw_String(100, 14, "");
									cout << player[1].getBullAccuracy();
									Draw_String(100, 15, "    ");
									Draw_String(100, 15, "");
									cout << player[1].getSingleAccuracy();

									if (modIter == player.begin()) 
									{
										//shows little arrow to point who's turn it is
										Set_Color(6, 0);
										Draw_String(67, 4, "---->");
										Draw_String(67, 15, "     ");
										
										//displaying player's left darts
										if (modIter->nTurns == 3)
										{
											Set_Color(2, 0);
											Draw_String(95, 6, "<---<<");
											Draw_String(95, 7, "<---<<");
											Draw_String(95, 8, "<---<<");
										}
										else if (modIter->nTurns == 2)
										{
											Set_Color(15, 0);
											Draw_String(95, 6, "        ");
										}
										else if (modIter->nTurns == 1)
										{
											Set_Color(15, 0);
											Draw_String(95, 6, "        ");
											Draw_String(95, 7, "        ");
										}
										
										//if the user's turn make everything in the interactive way
										int throwType = 0, scoreHit = 0;
										unsigned int numberAim = 0;
										Set_Color(15, 0);
										Draw_String(40, 25, "                                                          ");
										Draw_String(10, 23, "Press 3 for attempting a treble, 2 for a double, 1 for a single or 0 for trying to hit the bull: ");
										while (true)
										{
											cin >> throwType;
											if (throwType >= 0 && throwType <= 3) 
											{
												if (throwType == 0)
												{
													Draw_String(10, 25, "                                                                                                     ");
													Draw_String(45, 25, "");
													cout << "Attempting for a bull - scored ";
													scoreHit = modIter->bull(dartboard);
													cout << scoreHit;
													Sleep(1000);
													Draw_String(107, 23, "    ");
													Draw_String(107, 23, "");
													scoreHit = modIter->getScore() - scoreHit;
													modIter->setScore(scoreHit);
												}
												else
												{
													Draw_String(10, 23, "                                                                                                    ");
													Draw_String(25, 23, "Now choose to which number you're attempting to throw (1-20 or 25): ");
													while (true)
													{
														cin >> numberAim;
														if (numberAim >= 1 && numberAim <= 20)
														{
															Draw_String(35, 25, "                                                                       ");
															Draw_String(45, 25, "");
															cout << "Attempting for " << throwType << "x" << numberAim << " - scored: ";
															switch (throwType)
															{
																case 1:
																{
																	scoreHit = modIter->singleThrow(dartboard, bIter, numberAim);
																	cout << scoreHit << endl;
																	scoreHit = modIter->getScore() - scoreHit;
																	modIter->setScore(scoreHit);
																	break;
																}
																case 2:
																{
																	scoreHit = modIter->doubleThrow(dartboard, bIter, numberAim);
																	cout << scoreHit << endl;
																	scoreHit = modIter->getScore() - scoreHit;
																	modIter->setScore(scoreHit);
																	break;
																}
																case 3:
																{
																	scoreHit = modIter->trebleThrow(dartboard, bIter, numberAim);
																	cout << scoreHit << endl;
																	scoreHit = modIter->getScore() - scoreHit;
																	modIter->setScore(scoreHit);
																	break;
																}
															}
															Sleep(1000);
															break;
														}
														else if (numberAim == 25)
														{
															Draw_String(35, 25, "                                                                       ");
															Draw_String(40, 25, "");
															cout << "Attempting to hit the outer bull - scored ";
															numberAim = 25;
															scoreHit = modIter->singleThrow(dartboard, bIter, numberAim);
															cout << scoreHit;
															scoreHit = modIter->getScore() - scoreHit;
															modIter->setScore(scoreHit);
															break;
														}
														else
														{
															Set_Color(15, 4);
															Draw_String(40, 12, "Invalid choice inserted. Please try again");
															cin.clear();
															cin.ignore(1000, '\n');
															Sleep(1500);
															Set_Color(15, 0);
															Draw_String(40, 12, "                                             ");
															Draw_String(93, 23, "     ");
															Draw_String(93, 23, "");
														}
													}
												}
												break;
											}
											else
											{
												Set_Color(15, 4);
												Draw_String(40, 12, "Invalid choice inserted. Please try again");
												cin.clear();
												cin.ignore(1000, '\n');
												Sleep(1500);
												Set_Color(15, 0);
												Draw_String(40, 12, "                                             ");
												Draw_String(107, 23, "    ");
												Draw_String(107, 23, "");
											}
										}
									}
									else 
									{
										//shows little arrow to point who's turn it is
										Set_Color(6, 0);
										Draw_String(67, 15, "---->");
										Draw_String(67, 4, "     ");

										//displaying AI's left darts
										if (modIter->nTurns == 3)
										{
											Set_Color(4, 0);
											Draw_String(95, 17, "<---<<");
											Draw_String(95, 18, "<---<<");
											Draw_String(95, 19, "<---<<");
										}
										else if (modIter->nTurns == 2)
										{
											Set_Color(15, 0);
											Draw_String(95, 17, "        ");
										}
										else if (modIter->nTurns == 1)
										{
											Set_Color(15, 0);
											Draw_String(95, 17, "        ");
											Draw_String(95, 18, "        ");
										}
										
										Set_Color(15, 0);
										//the computer plays, with same AI implementation as in the simulation
										for (dLd = doublesLeaveDoubles.begin(); dLd != doublesLeaveDoubles.end(); dLd++)
										{
											//if score is equal to a double-leaving-double, goes for THAT first
											if (modIter->getScore() == (*dLd))
											{
												targetNo = (*dLd) / 2;
												Draw_String(10, 23, "                                                                                                ");
												Draw_String(42, 23, "");
												cout << modIter->getName() << " is now attempting for 2x" << targetNo;
												tempCalc = modIter->doubleThrow(dartboard, bIter, targetNo);
												Draw_String(20, 25, "                                                                        ");
												Sleep(500);
												Draw_String(50, 25, "");
												cout << "They scored " << tempCalc;
												tempCalc = modIter->getScore() - tempCalc;
												modIter->setScore(tempCalc);
												goto skipifs2;
											}
										}
										for (bIter = dartboard.begin(); bIter != dartboard.end(); bIter++)
										{
											if (modIter->getScore() == ((*bIter) * 2)) 
											{
												//check if score is equal to ANY of the doubles, go for THAT shot to end the game
												targetNo = *bIter;
												Draw_String(10, 23, "                                                                                                ");
												Draw_String(42, 23, "");
												cout << modIter->getName() << " is now attempting for 2x" << targetNo;
												tempCalc = modIter->doubleThrow(dartboard, bIter, targetNo);
												Draw_String(20, 25, "                                                                        ");
												Sleep(500);
												Draw_String(50, 25, ""); 
												cout << "They scored " << tempCalc;
												tempCalc = modIter->getScore() - tempCalc;
												modIter->setScore(tempCalc);
												goto skipifs2;
											}
										}
										if (modIter->getScore() == 50) 
										{
											//if able to win with a bull, go for that shot
											Draw_String(10, 23, "                                                                                                ");
											Draw_String(40, 23, "");
											cout << modIter->getName() << " is now attempting for a bull hit";
											tempCalc = modIter->bull(dartboard);
											Draw_String(20, 25, "                                                                        ");
											Sleep(500);
											Draw_String(50, 25, "");
											cout << "They scored " << tempCalc;
											tempCalc = modIter->getScore() - tempCalc;
											modIter->setScore(tempCalc);
										}
										if (modIter->getScore() > 50)
										{
											for (bIter = dartboard.begin(); bIter != dartboard.end(); bIter++)
											{
												//checks if there's any double or treble possible that will leave
												//a double-leaving-double (the best in case of throw fail) to end (covers question 2, 3 & 4 in adv AI)
												for (dLd = doublesLeaveDoubles.begin(); dLd != doublesLeaveDoubles.end(); dLd++) 
												{
													if (modIter->getScore() == (((*bIter) * 3) + (*dLd)))
													{
														targetNo = *bIter;
														Draw_String(10, 23, "                                                                                                ");
														Draw_String(42, 23, "");
														cout << modIter->getName() << " is now attempting for 3x" << targetNo;
														tempCalc = modIter->trebleThrow(dartboard, bIter, targetNo);
														Draw_String(20, 25, "                                                                        ");
														Sleep(500);
														Draw_String(50, 25, "");
														cout << "They scored " << tempCalc;
														tempCalc = modIter->getScore() - tempCalc;
														modIter->setScore(tempCalc);
														goto skipifs2;
													}
													else if (modIter->getScore() == (((*bIter) * 2) + (*dLd)))
													{
														targetNo = *bIter;
														Draw_String(10, 23, "                                                                                                ");
														Draw_String(42, 23, "");
														cout << modIter->getName() << " is now attempting for 2x" << targetNo;
														tempCalc = modIter->doubleThrow(dartboard, bIter, targetNo);
														Draw_String(20, 25, "                                                                        ");
														Sleep(500);
														Draw_String(50, 25, "");
														cout << "They scored " << tempCalc;
														tempCalc = modIter->getScore() - tempCalc;
														modIter->setScore(tempCalc);
														goto skipifs2;
													}
												}

												//if nothing, checks then if there's any double or treble possible that will leave
												//a bull to win the game, and goes for that (covers question 2, 3 & 4 in adv AI)
												if (modIter->getScore() == (((*bIter) * 3) + 50)) 
												{
													targetNo = *bIter;
													Draw_String(10, 23, "                                                                                                ");
													Draw_String(42, 23, "");
													cout << modIter->getName() << " is now attempting for 3x" << targetNo;
													tempCalc = modIter->trebleThrow(dartboard, bIter, targetNo);
													Draw_String(20, 25, "                                                                        ");
													Sleep(500);
													Draw_String(50, 25, "");
													cout << "They scored " << tempCalc;
													tempCalc = modIter->getScore() - tempCalc;
													modIter->setScore(tempCalc);
													goto skipifs2;
												}
												else if (modIter->getScore() == (((*bIter) * 2) + 50))
												{
													targetNo = *bIter;
													Draw_String(10, 23, "                                                                                                ");
													Draw_String(42, 23, "");
													cout << modIter->getName() << " is now attempting for 2x" << targetNo;
													tempCalc = modIter->doubleThrow(dartboard, bIter, targetNo);
													Draw_String(20, 25, "                                                                        ");
													Sleep(500);
													Draw_String(50, 25, "");
													cout << "They scored " << tempCalc;
													tempCalc = modIter->getScore() - tempCalc;
													modIter->setScore(tempCalc);
													goto skipifs2;
												}
											}
										}
										if (modIter->getScore() >= 62) 
										{
											//in all the other cases, until down to this score, go only for the highest treble (3x20 - 60)
											targetNo = 20;
											Draw_String(10, 23, "                                                                                                ");
											Draw_String(42, 23, "");
											cout << modIter->getName() << " is now attempting for a 3x" << targetNo;
											tempCalc = modIter->trebleThrow(dartboard, bIter, targetNo);
											Draw_String(20, 25, "                                                                        ");
											Sleep(500);
											Draw_String(50, 25, "");
											cout << "They scored " << tempCalc;
											tempCalc = modIter->getScore() - tempCalc;
											modIter->setScore(tempCalc);
										}
										else if (modIter->getScore() < 62 && modIter->getScore() > 50)  
										{
											//goes for the single throw equal to the score difference, thus it can try to end with a bull at next throw
											targetNo = modIter->getScore() - 50;
											Draw_String(10, 23, "                                                                                                ");
											Draw_String(42, 23, "");
											cout << modIter->getName() << " is now attempting for a 1x" << targetNo;
											tempCalc = modIter->singleThrow(dartboard, bIter, targetNo);
											Draw_String(20, 25, "                                                                        ");
											Sleep(500);
											Draw_String(50, 25, "");
											cout << "They scored " << tempCalc;
											tempCalc = modIter->getScore() - tempCalc;
											modIter->setScore(tempCalc);
										}
										else if (modIter->getScore() < 50)
										{
											//if score is under 4, must only end with double 1
											if (modIter->getScore() == 3)
											{
												targetNo = 1;
												Draw_String(10, 23, "                                                                                                ");
												Draw_String(42, 23, "");
												cout << modIter->getName() << " is now attempting for a 1x" << targetNo;
												tempCalc = modIter->singleThrow(dartboard, bIter, targetNo);
												Draw_String(20, 25, "                                                                        ");
												Sleep(500);
												Draw_String(50, 25, "");
												cout << "They scored " << tempCalc;
												tempCalc = (modIter->getScore() - tempCalc);
												modIter->setScore(tempCalc);
												goto skipifs2;
											}

											//otherwise tries to get close to a double-leaving double by aiming at the difference
											for (dLd = doublesLeaveDoubles.begin(); dLd != doublesLeaveDoubles.end(); dLd++)
											{
												for (int j = 1; j <= 9; j++)
												{
													if (modIter->getScore() == ((*dLd) + j))
													{
														targetNo = j;
														Draw_String(10, 23, "                                                                                                ");
														Draw_String(42, 23, "");
														cout << modIter->getName() << " is now attempting for a 1x" << targetNo;
														tempCalc = modIter->singleThrow(dartboard, bIter, targetNo);
														Draw_String(20, 25, "                                                                        ");
														Sleep(500);
														Draw_String(50, 25, "");
														cout << "They scored " << tempCalc;
														tempCalc = (modIter->getScore() - tempCalc);
														modIter->setScore(tempCalc);
														dLd = doublesLeaveDoubles.end() - 1; //in order to end outer loop immediately
														break;
													}
												}
											}
										}
									}

								//only usage of goto redirection - highly needed to exit the tricky previous loop checks
								skipifs2: 
									if (modIter == player.end() - 1)
									{
										//make sure the outputs while AI is playing stay for a while
										Sleep(1500);
									}
									if (modIter->getScore() == 0)
									{
										modIter->gamesWon++;
										Set_Color(15, 0);
										Draw_String(10, 23, "                                                                                                          ");
										Draw_String(10, 25, "                                                                                                          ");
										Draw_String(40, 24, "");
										cout << modIter->getName() << " has won this game! Next one starting...";
										
										//decision to continue playing or end the program
										Draw_String(43, 25, "(Press 'y' to continue, 'n' to quit) ");
										char endGameDecision = '\0';
										cin >> endGameDecision;
										if (endGameDecision == 'y')
										{
											system("cls");
										}
										else if (endGameDecision == 'n')
										{
											exit(EXIT_SUCCESS);
										}
										break;
									}
									modIter->nTurns--;

									//eliminates the last dart
									if (player[0].nTurns == 0)
									{
										Set_Color(15, 0);
										Draw_String(95, 8, "        ");
									}
									else if (player[1].nTurns == 0)
									{
										Set_Color(15, 0);
										Draw_String(95, 19, "        ");
									}
								}
								if (modIter->getScore() == 0)
								{
									//scores get restored when someone's won, ready for the next game
									player[0].setScore(gameLen);
									player[1].setScore(gameLen);
									break;
								}
								else if (modIter->getScore() < 2 && modIter->getScore() != 0)
								{
									//resets the previously held score because throw is invalid and score got under 2
									Set_Color(15, 4);
									Draw_String(10, 27, "");
									cout << "Score went under 2, impossible to finish, the score will be reset to what it was before this turn";
									Sleep(3000);
									Set_Color(15, 0);
									Draw_String(10, 27, "                                                                                                      ");
									modIter->setScore(tempScore);
									modIter->nTurns = MAX_THROWS_TURN;
								}
								else
								{
									modIter->nTurns = MAX_THROWS_TURN;
								}
							}
							modIter++; //next player will start
						}
						if (player[0].gamesWon == 3)
						{
							player[0].setsWon++;
						}
						else
						{
							player[1].setsWon++;
						}

						//if the end of the vector was reached, start again
						if (modIter == player.end())
						{
							modIter = player.begin(); 
						}
					}
					if (player[0].setsWon == 7)
					{
						player[0].matchesWon++;
					}
					else
					{
						player[1].matchesWon++;
					}

					//counters for the sets winning percentage
					{
						if (player[0].setsWon == 7 && player[1].setsWon == 1)
						{
							scoreCombinations[0]++;
						}
						else if (player[0].setsWon == 7 && player[1].setsWon == 2)
						{
							scoreCombinations[1]++;
						}
						else if (player[0].setsWon == 7 && player[1].setsWon == 3)
						{
							scoreCombinations[2]++;
						}
						else if (player[0].setsWon == 7 && player[1].setsWon == 4)
						{
							scoreCombinations[3]++;
						}
						else if (player[0].setsWon == 7 && player[1].setsWon == 5)
						{
							scoreCombinations[4]++;
						}
						else if (player[0].setsWon == 7 && player[1].setsWon == 6)
						{
							scoreCombinations[5]++;
						}
						else if (player[0].setsWon == 6 && player[1].setsWon == 7)
						{
							scoreCombinations[6]++;
						}
						else if (player[0].setsWon == 5 && player[1].setsWon == 7)
						{
							scoreCombinations[7]++;
						}
						else if (player[0].setsWon == 4 && player[1].setsWon == 7)
						{
							scoreCombinations[8]++;
						}
						else if (player[0].setsWon == 3 && player[1].setsWon == 7)
						{
							scoreCombinations[9]++;
						}
						else if (player[0].setsWon == 2 && player[1].setsWon == 7)
						{
							scoreCombinations[10]++;
						}
						else if (player[0].setsWon == 1 && player[1].setsWon == 7)
						{
							scoreCombinations[11]++;
						}
						else if (player[0].setsWon == 0 && player[1].setsWon == 7)
						{
							scoreCombinations[12]++;
						}
						else if (player[0].setsWon == 7 && player[1].setsWon == 0)
						{
							scoreCombinations[13]++;
						}

					}
				}
				break; //stops the simulation after all matches are concluded
			} 

			//results and percentages output - still needs to be finished
			cout << "\nSimulation ended. Players' results:" << endl;
			cout << player[0].getName() << ": " << player[0].matchesWon << " matches won" << endl;
			cout << player[1].getName() << ": " << player[1].matchesWon << " matches won" << endl;
			break;
		}
		else
		{
			cout << "Wrong choice my friend. Please try again ";
		}
	}	
	system("PAUSE");

	return 0;
}

void Init_Graphics(void)
{
	// this function initializes the console graphics engine

	COORD console_size = { 80, 35 }; // size of console

	// open i/o channel to console screen
	hconsole = CreateFile(TEXT("CONOUT$"), GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		0L, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0L);

	// make sure we are in 150x35
	SetConsoleScreenBufferSize(hconsole, console_size);

	// get details for console screen                       
	GetConsoleScreenBufferInfo(hconsole, &con_info);

} // end Init_Graphics
inline void Set_Color(int fcolor, int bcolor = 0)
{
	// this function sets the color of the console output
	SetConsoleTextAttribute(hconsole, (WORD)((bcolor << 4) |
		fcolor));

} // Set_Color
inline void Draw_String(int x, int y, char *string)
{
	// this function draws a string at the given x,y

	COORD cursor_pos; // used to pass coords

	// set printing position
	cursor_pos.X = x;
	cursor_pos.Y = y;
	SetConsoleCursorPosition(hconsole, cursor_pos);

	// print the string in current color
	printf("%s", string);

} // end Draw_String
inline void Interface_Init(vector<Player>& player)
{
	//interactive game HUD
	Set_Color(2, 0); //green color for the user stats
	Draw_String(90, 1, "");
	cout << player[0].getName();
	Draw_String(85, 2, "Current score: ");
	Draw_String(85, 3, "Bull accuracy: ");
	Draw_String(83, 4, "Single accuracy: ");
	Draw_String(80, 7, "Throws left: ");
	Set_Color(15, 0);
	Draw_String(77, 10, "----------------------------------");
	Set_Color(4, 0); //red color for the AI stats
	Draw_String(90, 12, "");
	cout << player[1].getName();
	Draw_String(85, 13, "Current score: ");
	Draw_String(85, 14, "Bull accuracy: ");
	Draw_String(83, 15, "Single accuracy: ");
	Draw_String(80, 18, "Throws left: ");
	Set_Color(15, 0);
	Draw_String(5, 21, "==========================================================================================================");
}
