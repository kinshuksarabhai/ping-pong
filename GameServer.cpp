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
