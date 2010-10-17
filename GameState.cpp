enum Status{GAME_INIT,GAME_WAITING,GAME_READY,
GAME_STARTED,GAME_PAUSED,GAME_FINISHED};

class GameState
{
public:
  /*static*/
  float hardness_level;//server only
  int num_balls;
  int num_players;

  /*updatable parameters*/
  int wall_no;//for client
  Status status;
  Paddle paddle[4];
  Ball ball[MAX_BALLS];

  GameState();
  float distance(Vector,Vector);
  void printState();

  /*for server*/
  void getClientMessage(ClientMessage&);
  void updateGameState(ClientMessage);
  void calculateNextState();

  /*for client*/
  void getServerMessage(ServerMessage&);
  void updateGameState(ServerMessage);
  void movePaddle(int);
};

GameState::GameState()
{
  hardness_level=0;

  status=GAME_INIT;
  /*paddle*/
  for(int i=0;i<4;i++)
    {
      paddle[i].position=0.5;
      paddle[i].pstate=PLAYER_NA;
    }
  /*balls*/
  for(int i=0;i<MAX_BALLS;i++)
    {
      /*initial position*/
      ball[i].position.x=0.5+0.1*cos(2.0*M_PI*i/num_balls+M_PI/6.0);
      ball[i].position.y=0.5+0.1*sin(2.0*M_PI*i/num_balls+M_PI/6.0);

      /*initial velocity*/
      ball[i].velocity.x=0.2*(ball[i].position.x-0.5);
      ball[i].velocity.y=0.2*(ball[i].position.y-0.5);
    }
}

float GameState::distance(Vector v1,Vector v2)
{
  int dx=v1.x-v2.x;
  int dy=v1.y-v2.y;
  return sqrt(dx*dx+dy*dy);
}

void GameState::calculateNextState()
{
  if (status==GAME_STARTED)
    {
      /*interball collisions*/
  for(int i=0;i<num_balls;i++)
    for(int j=0;j<i;j++)
      {
	if(distance(ball[i].position,ball[j].position)<=BALL_SIZE)
	  {
	    /*collision*/
	    cout<<"Interball Collision\n";
	    /*	    ball[i].velocity.x=-ball[i].velocity.x;
	    ball[i].velocity.y=-ball[i].velocity.y;

	    ball[j].velocity.x=-ball[j].velocity.x;
	    ball[j].velocity.y=-ball[j].velocity.y;*/
	  }
      }
  for(int i=0;i<num_balls;i++)
    {
      /*collision with paddles*/


      /*collision with walls*/
      if(ball[i].position.x*L<BALL_SIZE ||
	 PADDLE_WIDTH+ball[i].position.x*L>1-PADDLE_WIDTH-BALL_SIZE)
	{
	  ball[i].velocity.x= -ball[i].velocity.x;
	}
      if(ball[i].position.y*L<BALL_SIZE || 
	 PADDLE_WIDTH+ball[i].position.y*L>1-PADDLE_WIDTH-BALL_SIZE)
	{
	  ball[i].velocity.y= -ball[i].velocity.y;	  
	}

	ball[i].position.x+=ball[i].velocity.x;
	ball[i].position.y+=ball[i].velocity.y;
    }
  //update paddles too...
  /*  for(int i=0;i<4;i++)
    if(paddle[i].state==BRICKED)
    paddle[i].position=ball[0].*/
    }
}
void GameState::printState()
{
  cout<<"\nPos:("<<ball[0].position.x<<","<<ball[0].position.y<<")\n";
}
void GameState::movePaddle(int dir)
{
  if(status==GAME_STARTED)
    {
  paddle[wall_no].position+=0.05*dir;
  if(paddle[wall_no].position>1)
    paddle[wall_no].position=1.0;
  else if(paddle[wall_no].position<0)
    paddle[wall_no].position=0.0;
    }
}

void GameState::getServerMessage(ServerMessage &sm)
{
  sm.num_balls=num_balls;
  for(int i=0;i<4;i++)
    sm.paddle[i]=paddle[i];
  for(int i=0;i<num_balls;i++)
    sm.ball[i]=ball[i];
}
void GameState::getClientMessage(ClientMessage &cm)
{
  cm.wall_no=wall_no;
  cm.paddle_position=paddle[wall_no].position;
}
void GameState::updateGameState(ServerMessage sm)
{
  for(int i=0;i<4;i++)
    if(i!=wall_no)
      paddle[i]=sm.paddle[i];
  for(int i=0;i<num_balls;i++)
    ball[i]=sm.ball[i];
}
void GameState::updateGameState(ClientMessage cm)
{
  if(cm.wall_no>=0 && cm.wall_no<4)
    paddle[cm.wall_no].position=cm.paddle_position;
}

GameState gstate;
