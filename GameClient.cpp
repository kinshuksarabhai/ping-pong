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
public:
  /*game logic*/
  int connect_count;
  GameClient();
  void setupNewGame(int,char**);
  void executeGameLoop();
};
GameClient::GameClient()
{
  connect_count=0;
}
void GameClient::executeGameLoop()
{
  //for interpolation
}
void GameClient::setupNewGame(int argc,char** argv)
{
  switch(argc)
    {
    case 3:
      client.serv_addr.sin_port = htons(atoi(argv[2]));
      cout<<"Port:"<<ntohs(client.serv_addr.sin_port)<<endl;
    case 2:
      inet_pton(AF_INET,argv[1], &client.serv_addr.sin_addr);
    }
  /*now ask...
  switch(argc)
    {
    case 1://no host
      char str[30];
      cout<<"Enter hostname of server:"<<endl;
      cin>>str;
      inet_pton(AF_INET,str, &client.serv_addr.sin_addr);
    case 2://no port no.
      int port;
      cout<<"Enter port no. of server:"<<endl;
      cin>>port;
      client.serv_addr.sin_port = htons(port);
    }*/
}
GameClient gc;

void* sender_main(void*)
{
    while(gstate.status!=GAME_FINISHED)
      {
	switch(gstate.status)
	  {
	  case GAME_INIT://if server missed CONNECT
	    client.sendMessage(CONNECT);
	    usleep(1000000);
	    break;
	  case GAME_READY://if server missed READY
	    client.sendMessage(READY);
	    usleep(1000000);
	    break;
	  case GAME_PAUSED://if server missed PAUSED
	    client.sendMessage(PAUSE);
	    usleep(1000000);
	    break;
	  case GAME_STARTED://once in a while
	    client.sendMessage(POSITION);
	    usleep(1000000);
	    cout<<"I m working..."<<endl;
	  }
      }
}

int main(int argc, char **argv)
{

  pthread_t clientthread,guithread,senderthread;
  bzero((char *) &client.serv_addr, sizeof(client.serv_addr));
  client.serv_addr.sin_family = AF_INET;
  client.serv_addr.sin_addr.s_addr = INADDR_ANY;
  client.serv_addr.sin_port=htons(PORT_NO);

  gc.setupNewGame(argc,argv);
  pthread_create( &clientthread, NULL,client_main,NULL);
  pthread_create( &senderthread, NULL,sender_main,NULL);
  pthread_create( &guithread, NULL,cgui_main,NULL);

  //gc.executeGameLoop();

  pthread_join(clientthread,NULL);
  pthread_join(senderthread,NULL);
  pthread_join(guithread,NULL);
  /*
  client_main(NULL);*/
}
