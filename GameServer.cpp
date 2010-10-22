#include <sys/time.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>

#include<math.h>
#include<stdlib.h>
#include<pthread.h>
#include<error.h>
#include<string.h>
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
    cout<<endl;
    cout<<"Select Game Type:"<<endl;
    cout<<"1. Single player against computer."<<endl;
    cout<<"2. Two player game."<<endl;
    cout<<"3. Three player game."<<endl;
    cout<<"4. Four player game."<<endl;
    cout<<"Your choice?"<<endl;
    cin>>num;
  }
  while(num<1 || num>4);
  gstate.num_players=num;
 if(num==1)
   {
     gstate.paddle[2].ptype=COMPUTER;
     gstate.paddle[2].pstate=PLAYER_READY;
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
  cout<<"Num balls:"<<num<<endl;
  gstate.initializeState();
  gstate.printState();
}
void GameServer::executeGameLoop()
{
  while(gstate.status!=GAME_FINISHED)
    {
      gstate.calculateNextState();
	usleep(40000);
    }
  cout<<"Game finished!"<<endl;
}
GameServer gs;

void* sender_main(void*)
{
    while(gstate.status!=GAME_FINISHED)
      {
	switch(gstate.status)
	  {
	  case GAME_PAUSED:

	  case GAME_STARTED:
	    for(int i=0;i<4;i++)
	      if(gstate.paddle[i].ptype==HUMAN)
		server.sendMessage(POSITION,i);
	  }
	usleep(30000);
      }
  for(int i=0;i<4;i++)
    if(gstate.paddle[i].ptype==HUMAN)
      server.sendMessage(POSITION,i);
}

int main(int argc, char **argv)
{

  pthread_t serverthread,senderthread;

  bzero((char *) &server.serv_addr, sizeof(sockaddr_in));
  server.serv_addr.sin_family = AF_INET;
  server.serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server.serv_addr.sin_port=htons(PORT_NO);

  if(argc>=2)
    {
      server.serv_addr.sin_port = htons(atoi(argv[1]));
      cout<<"args:"<<server.serv_addr.sin_port<<argv[1]<<endl;
    }

  gs.setupNewGame();
  pthread_create( &serverthread, NULL,server_main,NULL);
  pthread_create( &senderthread, NULL,sender_main,NULL);
  gs.executeGameLoop();
  pthread_join(serverthread,NULL);
  pthread_join(senderthread,NULL);
}
