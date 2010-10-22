#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<unistd.h>
#include<strings.h>

#include<math.h>
#include<stdlib.h>
#include<pthread.h>
#include<iostream>
using namespace std;

#include"GameStructures.h"
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

  pthread_t clientthread,guithread;
  bzero((char *) &client.serv_addr, sizeof(client.serv_addr));
  client.serv_addr.sin_family = AF_INET;
  client.serv_addr.sin_addr.s_addr = INADDR_ANY;
  client.serv_addr.sin_port=htons(PORT_NO);

  if(argc>=2)
    inet_pton(AF_INET,argv[1], &client.serv_addr.sin_addr);
  if(argc>=3)
    {
    client.serv_addr.sin_port = htons(atoi(argv[2]));
    cout<<"args:"<<client.serv_addr.sin_port<<argv[2]<<endl;
    }
  pthread_create( &clientthread, NULL,client_main,NULL);
  pthread_create( &guithread, NULL,cgui_main,NULL);

  //  gc.executeGameLoop();

  pthread_join(clientthread,NULL);
  pthread_join(guithread,NULL);
  /*
  client_main(NULL);*/
}
