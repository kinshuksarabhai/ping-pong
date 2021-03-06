#include"GameState.h"

GameState::GameState()
{
  wall_no=-1;
  hardness_level=0;
  hit_no=1;
  winner=-1;
  status=GAME_INIT;
  /*paddle*/
  for(int i=0;i<4;i++)
    {
      paddle[i].ptype=AUTO;
      paddle[i].pstate=PLAYER_NA;
      paddle[i].position=0.5;
      paddle[i].life=5;
    }
  initializeState();
}
void GameState::initializeState()
{
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
	    /*	    cout<<"Interball Collision\n";

	    float nx,ny,nmag;
	    /*calculate normal vector
	    nx=ball[i].position.x-ball[j].position.x;
	    ny=ball[i].position.y-ball[j].position.y;
	    nmag=sqrt(nx*nx+ny*ny);*/

	    //find v on n (for ball i)
	  }
      }
  for(int i=0;i<num_balls;i++)
    {
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
	  //for computer player at wall 2
	  if(paddle[2].ptype==COMPUTER)
	    {
	      hit_no=(drand48()<hardness_level)?0:1;//probability of failure
//	      cout<<"Hardness level:"<<hardness_level<<endl;
//	      cout<<"Hit no.:"<<hit_no<<endl;
	    }
//	  else
//	    cout<<"Wrong ptype:"<<paddle[2].ptype<<endl;
	}

	ball[i].position.x+=ball[i].velocity.x;
	ball[i].position.y+=ball[i].velocity.y;
    }
  //update paddles too...
for(int i=0;i<4;i++)//walls
  if(paddle[i].ptype!=HUMAN)
      {
	/*find nearest incoming ball*/
	float dist=1.0;
	int ball_no=-1;
	for(int j=0;j<num_balls;j++)
	  {
	    switch(i)
	      {
	      case 0:
		if(ball[j].velocity.y<0 && dist>=ball[j].position.y)
		  {
		    ball_no=j;
		  }
		break;
	      case 1:
		if(ball[j].velocity.x>0 && dist>=1.0-ball[j].position.x)
		  {
		    ball_no=j;		    
		  }
		break;
	      case 2:
		if(ball[j].velocity.y>0 && dist>=1.0-ball[j].position.y)
		  {
		    ball_no=j;		    
		  }
		break;
	      case 3:
		if(ball[j].velocity.x<0 && dist>=ball[j].position.x)
		  {
		    ball_no=j;
		  }
		break;
	      }
	  }
	    /*find new position update*/
	    if(ball_no!=-1)
	      {
		switch(i)
		  {
		  case 0:
		    paddle[i].position=PADDLE_WIDTH+ball[ball_no].position.x*L;
		    break;
		  case 1:
		    paddle[i].position=PADDLE_WIDTH+ball[ball_no].position.y*L;
		    break;
		  case 2:
		    if(paddle[i].ptype!=COMPUTER || hit_no==1)
		      paddle[i].position=1.0-PADDLE_WIDTH-ball[ball_no].position.x*L;
		    break;
		  case 3:
		    paddle[i].position=1.0-PADDLE_WIDTH-ball[ball_no].position.y*L;
		  }
	      }
      }
 float x0,x1,ball_pos;
/*life loss+game winning condition*/
 for(int i=0;i<num_balls;i++)
   {
     if(ball[i].position.x*L<BALL_SIZE)
       {//wall 3
	 x0=1-PADDLE_WIDTH-L*PADDLE_LENGTH+paddle[3].position*(2*PADDLE_WIDTH+L*PADDLE_LENGTH-1);
	 x1=1-PADDLE_WIDTH+paddle[3].position*(2*PADDLE_WIDTH+L*PADDLE_LENGTH-1);

	 ball_pos=PADDLE_WIDTH+ball[i].position.y*L;
	 if(paddle[3].ptype!=AUTO &&(ball_pos<x0 || ball_pos>x1) && paddle[3].life>0)
	   {
	     paddle[3].life--;
//	     cout<<"Player 3 Life:"<<paddle[3].life<<endl;
//     cout<<x0<<" to "<<x1<<":"<<ball_pos<<endl;
	     if(paddle[3].life<=0)
	       {
	       paddle[3].pstate=PLAYER_FINISHED;
	       }
	   }
       }
     else if(PADDLE_WIDTH+ball[i].position.x*L>1-PADDLE_WIDTH-BALL_SIZE)
       {//wall 1
	 x0=PADDLE_WIDTH+paddle[1].position*(1-2*PADDLE_WIDTH-L*PADDLE_LENGTH);
	 x1=PADDLE_WIDTH+L*PADDLE_LENGTH+paddle[1].position*(1-2*PADDLE_WIDTH-L*PADDLE_LENGTH);
	 ball_pos=PADDLE_WIDTH+ball[i].position.y*L;
	 if(paddle[1].ptype!=AUTO &&(ball_pos<x0 || ball_pos>x1) && paddle[1].life>0)
	   {
	     paddle[1].life--;
//	     cout<<"Player 1 Life:"<<paddle[1].life<<endl;
	     if(paddle[1].life<=0)
	       {
	       paddle[1].pstate=PLAYER_FINISHED;
	       }
	   }

       }
     if(ball[i].position.y*L<BALL_SIZE)
       {//wall 0
	 x0=PADDLE_WIDTH+paddle[0].position*(1-2*PADDLE_WIDTH-L*PADDLE_LENGTH);
	 x1=PADDLE_WIDTH+L*PADDLE_LENGTH+paddle[0].position*(1-2*PADDLE_WIDTH-L*PADDLE_LENGTH);
	 ball_pos=PADDLE_WIDTH+ball[i].position.x*L;
	 if(paddle[0].ptype!=AUTO &&(ball_pos<x0 || ball_pos>x1) && paddle[0].life>0)
	   {
	     paddle[0].life--;
//	     cout<<"Player 0 Life:"<<paddle[0].life<<endl;
	     if(paddle[0].life<=0)
	       {
	       paddle[0].pstate=PLAYER_FINISHED;
	       }
	   }

       }
     else if(PADDLE_WIDTH+ball[i].position.y*L>1-PADDLE_WIDTH-BALL_SIZE)
       {//wall 2
	 x0=1-PADDLE_WIDTH-L*PADDLE_LENGTH+paddle[2].position*(2*PADDLE_WIDTH+L*PADDLE_LENGTH-1);
	 x1=1-PADDLE_WIDTH+paddle[2].position*(2*PADDLE_WIDTH+L*PADDLE_LENGTH-1);
	 ball_pos=PADDLE_WIDTH+ball[i].position.x*L;
	 if(paddle[2].ptype!=AUTO &&(ball_pos<x0 || ball_pos>x1) && paddle[2].life>0)
	   {
	     paddle[2].life--;
//	     cout<<"Player 2 Life:"<<paddle[2].life<<endl;
//	     cout<<x0<<" to "<<x1<<":"<<ball_pos<<endl;
	     if(paddle[2].life<=0)
	       {
	       paddle[2].pstate=PLAYER_FINISHED;
	       }
	   }
//	 else
//	   cout<<"Gadbad:"<<paddle[2].ptype<<","<<ball_pos<<","<<x0<<","<<x1<<endl;

       }

   }
 //check when game ends,and find winner
 if(num_players==1)
   {
     if(paddle[0].pstate==PLAYER_FINISHED)
       {
	 winner=2;
	 status=GAME_FINISHED;
	 cout<<winner<<" won"<<endl;
       }
     else if(paddle[2].pstate==PLAYER_FINISHED)
       {
	 winner=0;
	 status=GAME_FINISHED;
	 cout<<winner<<" won"<<endl;
       }
   }
 else
   {
     int flag=0;
     for(int i=0;i<4;i++)
       {
	 if(paddle[i].ptype!=AUTO)
	   {
	     if(paddle[i].pstate==PLAYER_FINISHED)
	       flag++;
	     else
	       {
		 winner=i;
		 //	     cout<<winner<<"won assign"<<endl;
	       }
	   }
       }
     if(flag==num_players-1)
       {
	 status=GAME_FINISHED;
	 cout<<winner<<" won"<<endl;
       }
   }
 //done
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
  if(status==GAME_FINISHED)
    sm.winner=winner;
  else
    sm.winner=-1;
}
void GameState::getClientMessage(ClientMessage &cm)
{
  cm.paddle_position=paddle[wall_no].position;
}
void GameState::updateGameState(ServerMessage sm)
{
  for(int i=0;i<4;i++)
    {
    if(i!=wall_no)
      paddle[i]=sm.paddle[i];
    else
      paddle[i].life=sm.paddle[i].life;
    }
  for(int i=0;i<num_balls;i++)
    ball[i]=sm.ball[i];

  /*check if game finished*/
  if(sm.winner!=-1)
    {
      winner=sm.winner;
      status=GAME_FINISHED;
    }
}
void GameState::updateGameState(ClientMessage cm,int w)
{
    paddle[w].position=cm.paddle_position;
}

GameState gstate;
