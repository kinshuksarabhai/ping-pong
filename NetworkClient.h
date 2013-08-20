#ifndef NETWORK_CLIENT
#define NETWORK_CLIENT

#include<stdio.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<sys/time.h>

#include<iostream>
using namespace std;

#include"GameStructures.h"
#include"GameState.h"
#define PORT_NO 3000

class NetworkClient
{
    public:
        int sockfd;
        sockaddr_in serv_addr;

        int serv_last_pkt_num;//incoming
        timeval serv_last_msg_time;//incoming
        int pkt_num;//sending count

        /*packet loss*/
        int total_pkts_lost;//incoming

        /*networking*/
        void initializeClient();
        void sendMessage(Command);
        void receiveMessage(ServerMessage &sm);

        void processMessage(ServerMessage sm);//updates gstate
        void dispatchMessage();

        void startGame();
        void pauseGame();
        void quitGame();
};

extern GameState gstate;
extern NetworkClient client;
void* client_main(void*);
#endif
