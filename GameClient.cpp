#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<unistd.h>
#include<strings.h>

#include<math.h>
#include<stdlib.h>
#include<pthread.h>
#include<iostream>
using namespace std;

#include"GameStructures.h"
#include"ServerMessage.cpp"
#include"GameState.cpp"
#include"NetworkClient.cpp"
#include"ClientGUI.cpp"

class GameClient
{

  /*game logic*/
  void executeGameLoop();
};

void GameClient::executeGameLoop()
{
}
GameClient gc;

int main(int argc, char **argv)
{
  /*
  pthread_t clientthread,guithread;

  pthread_create( &clientthread, NULL,client_main,NULL);
  pthread_create( &guithread, NULL,cgui_main,NULL);

  gc.executeGameLoop();

  pthread_join(clientthread,NULL);
  pthread_join(guithread,NULL);*/
  client_main(NULL);
}
