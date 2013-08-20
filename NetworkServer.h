#ifndef NETWORK_SERVER
#define NETWORK_SERVER

#include<stdio.h>
#include<string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/time.h>

#include<iostream>
using namespace std;

#include"GameStructures.h"
#include"GameState.h"
#define PORT_NO 3000
const int alloc_seq[]={0,2,1,3};
class NetworkServer
{
public:
  int sockfd;
  sockaddr_in serv_addr;//initialized in main

  pthread_mutex_t sockmutex;
  int num_players;
  PlayerInfo players[4];

  /*networking*/
  NetworkServer();
  void initializeServer();

  void sendMessage(Command cmd,int wall_no);
  void receiveMessage(ClientMessage&,sockaddr_in&);
  void processMessage(ClientMessage,sockaddr_in);

  int getWallNo(sockaddr_in client_addr);
  void tryToStartGame(int);
  void playerQuit(int);
};

extern GameState gstate;
extern NetworkServer server;
void* server_main(void*);
#endif
