#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include<unistd.h>

#include<math.h>
#include<pthread.h>
#include<error.h>
#include<strings.h>
#include<iostream>
using namespace std;

#include"GameStructures.h"
#include"GameState.cpp"
#include"NetworkServer.cpp"

class GameServer
{
public:
  /*game state*/
  //  GameServer();
  void setupNewGame();
  void executeGameLoop();
};
/*initialize game parameters*/
void GameServer::setupNewGame()
{
  int num;
  do
  {
  cout<<"\nNo. of players (1-4):"<<endl;
  cin>>num;
  }
  while(num<1 || num>4);
  gstate.num_players=num;
 if(num==1)
   {
     do
       {
	 cout<<"\nHardness level:"<<endl;
	 cin>>gstate.hardness_level;
       }
     while(gstate.hardness_level<0.0 || gstate.hardness_level>1.0);
   }
  do
  {
    cout<<"\nNo. of balls (1-"<<MAX_BALLS<<"):"<<endl;
  cin>>num;
  }
  while(num<1 || num>MAX_BALLS);
  gstate.num_balls=num;
}
void GameServer::executeGameLoop()
{
  while(1)
    {
      gstate.calculateNextState();
	usleep(40000);
    }
}
GameServer gs;

int main(int argc, char **argv)
{

  pthread_t serverthread;
  gs.setupNewGame();
  pthread_create( &serverthread, NULL,server_main,NULL);
  gs.executeGameLoop();
  pthread_join(serverthread,NULL);
}
