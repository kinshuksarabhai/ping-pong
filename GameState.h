#ifndef GAME_STATE
#define GAME_STATE

#include<math.h>
#include<stdlib.h>

#include<iostream>
using namespace std;

#include"GameStructures.h"

enum Status{GAME_INIT,GAME_WAITING,GAME_READY,
GAME_STARTED,GAME_PAUSED,GAME_FINISHED};

class GameState
{
public:
  float hardness_level;//server only: probabilty of failure
  int hit_no;

  int num_balls;
  int num_players;
  int winner;

  /*updatable parameters*/
  int wall_no;//for client
  Status status;
  Paddle paddle[4];
  Ball ball[MAX_BALLS];

  GameState();
  float distance(Vector,Vector);
  void printState();

  /*for server*/
  void initializeState();
  void getClientMessage(ClientMessage&);
  void updateGameState(ClientMessage,int);
  void calculateNextState();

  /*for client*/
  void getServerMessage(ServerMessage&);
  void updateGameState(ServerMessage);
  void movePaddle(int);
};

#endif
