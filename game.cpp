#define MAX_BALLS 3
#define BOARD_SIZE 500


typedef struct
{
  float x,y;
} Vector;

enum {BRICKED,OPEN};
typedef struct
{
  int state;
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
  socket sockfd;
  int state;
} Player;

class Game
{
  /*board parameters*/
int hardness_level;
int game_state;

/*paddle parameters*/
float paddle_length;
Paddle paddle[4];

/*ball parameters*/
Ball ball[MAX_BALLS];
int num_balls;
int ball_size;


  Game();
  void move_balls();
  void calculateNextState();
};

void Game::Game()
{
  hardness_level=0;
  game_state=0;

  /*paddle*/
  paddle_length=100;
  for(int i=0;i<4;i++)
    {
      paddle[i]=0.5;
    }
  /*balls*/
  num_balls=1;
  for(int i=0;i<MAX_BALLS;i++)
    {
    ball[i].position.x=BOARD_SIZE/2;
    ball[i].position.y=BOARD_SIZE/2;
    ball[i].velocity.x=0.4;
    ball[i].velocity.y=1;
    }
}

void Game::calculateNextState()
{
  for(int i=0;i<num_balls;i++)
    {
      if(ball[i].position.x<0 || ball[i].position.x>500)
	{
	  ball[i].velocity.x= -ball[i].velocity.x;
	}
      if(ball[i].position.y<0 || ball[i].position.y>500)
	{
	  ball[i].velocity.y= -ball[i].velocity.y;	  
	}

	ball[i].position.x+=ball[i].velocity.x;
	ball[i].position.y+=ball[i].velocity.y;
  }
}
