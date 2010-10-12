#include<math.h>
#include<iostream>

#define MAX_BALLS 3

#define BALL_SIZE 0.07
#define PADDLE_LENGTH 0.3
#define PADDLE_WIDTH 0.05
#define L (1-2*PADDLE_WIDTH)
using namespace std;

typedef struct
{
  float x,y;
} Vector;

enum {BRICKED,OPEN};

typedef struct
{
  int state;
  float position;
  int player_id;
} Paddle;

typedef struct
{
  Vector position,velocity;
} Ball;

typedef struct
{
  int player_id;
  int wall_no;
  //  socket sockfd;
  int state;
} Player;

//-------------------------------

class Game
{
public:
  /*board parameters*/
int hardness_level;
int game_state;

/*paddle parameters*/
Paddle paddle[4];

/*ball parameters*/
Ball ball[MAX_BALLS];
int num_balls;


  Game();
  float distance(Vector,Vector);
  void calculateNextState();
  void printState();
};

Game::Game()
{
  hardness_level=0;
  game_state=0;

  /*paddle*/
  for(int i=0;i<4;i++)
    {
      paddle[i].position=0.5;
    }
  /*balls*/
  num_balls=1;
  for(int i=0;i<MAX_BALLS;i++)
    {
      /*initial position*/
      ball[i].position.x=0.1;
      ball[i].position.y=0.9;

      /*initial velocity*/
    ball[i].velocity.x=0.01;
    ball[i].velocity.y=-0.03;
    }
}
float Game::distance(Vector v1,Vector v2)
{
  int dx=v1.x-v2.x;
  int dy=v1.y-v2.y;
  return sqrt(dx*dx+dy*dy);
}

void Game::calculateNextState()
{
      /*interball collisions*/
  for(int i=0;i<num_balls;i++)
    for(int j=0;j<i;j++)
      {
	if(distance(ball[i].position,ball[j].position)<=BALL_SIZE)
	  {
	    /*collision*/
	    cout<<"Interball Collision\n";
	  }
      }
  for(int i=0;i<num_balls;i++)
    {
      /*collision with paddles*/


      /*collision with walls*/
      if(ball[i].position.x*L<BALL_SIZE || PADDLE_WIDTH+ball[i].position.x*L>1-PADDLE_WIDTH-BALL_SIZE)
	{
	  ball[i].velocity.x= -ball[i].velocity.x;
	}
      if(ball[i].position.y*L<BALL_SIZE || PADDLE_WIDTH+ball[i].position.y*L>1-PADDLE_WIDTH-BALL_SIZE)
	{
	  ball[i].velocity.y= -ball[i].velocity.y;	  
	}

	ball[i].position.x+=ball[i].velocity.x;
	ball[i].position.y+=ball[i].velocity.y;
  }
}
void Game::printState()
{
  cout<<"\nPos:("<<ball[0].position.x<<","<<ball[0].position.y<<")\n";
}
/*
int main()
{
  Game g;
  for(int i=0;i<50;i++)
    {
      g.calculateNextState();
      g.printState();
    }
    }*/
