/*
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include<math.h>
#include<stdlib.h>
#include<pthread.h>
#include<error.h>
#include<string.h>
#include<strings.h>
*/
#include<unistd.h>
#include<iostream>
using namespace std;

#include"GameStructures.h"
#include"GameState.h"
#include"NetworkServer.h"

NetworkServer server;
/*initialize game parameters*/
void setupNewGame(int argc, char **argv)
{
    float hl=0.0;
    switch(argc)
    {
        case 5://hardness_level
            hl=atof(argv[4]);
        case 4://balls
            gstate.num_balls=atoi(argv[3]);
            cout<<"Num balls:"<<gstate.num_balls<<endl;
        case 3://game type
            gstate.num_players=atoi(argv[2]);
            cout<<"Num players:"<<gstate.num_players<<endl;
        case 2://port no
            server.serv_addr.sin_port = htons(atoi(argv[1]));
            cout<<"port:"<<ntohs(server.serv_addr.sin_port)<<endl;
    }
    switch(argc)
    {
        case 1://no port no.
            int port;
            cout<<"Enter port no. for server:"<<endl;
            cin>>port;
            server.serv_addr.sin_port = htons(port);
        case 2://no Game type etc.
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
        case 3://no balls
            do
            {
                cout<<"\nNo. of balls (1-"<<MAX_BALLS<<")?"<<endl;
                cin>>num;
            }
            while(num<1 || num>MAX_BALLS);
            gstate.num_balls=num;
            cout<<"Num balls:"<<num<<endl;
        case 4://no hardness_level
            if(gstate.num_players==1)
            {
                do
                {
                    cout<<"\nHardness level (Probability of failure of Computer player)?"<<endl;
                    cin>>hl;
                }
                while(hl<0.0 || hl>1.0);
            }
    }
    gstate.initializeState();//since num-balls was not known earlier
    if(gstate.num_players==1)
    {
        gstate.paddle[2].ptype=COMPUTER;
        gstate.paddle[2].pstate=PLAYER_READY;
    }
    gstate.hardness_level=hl;
    if(gstate.hardness_level>0.5)
        gstate.hit_no=0;
    cout<<"Hardness level:"<<gstate.hardness_level<<endl;
    cout<<"Hit no."<<gstate.hit_no<<endl;
    //  gstate.printState();
}
void executeGameLoop()
{
    while(gstate.status!=GAME_FINISHED)
    {
        gstate.calculateNextState();
        usleep(40000);
    }
    cout<<"Game finished!"<<endl;
}

void* sender_main(void*)
{
    while(gstate.status!=GAME_FINISHED)
    {
        switch(gstate.status)
        {
            case GAME_STARTED:
                for(int i=0;i<4;i++)
                    if(gstate.paddle[i].ptype==HUMAN)
                        server.sendMessage(POSITION,i);
            default:
                usleep(30000);
        }
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

    setupNewGame(argc,argv);

    pthread_create( &serverthread, NULL,server_main,NULL);
    pthread_create( &senderthread, NULL,sender_main,NULL);
    executeGameLoop();
    pthread_join(serverthread,NULL);
    pthread_join(senderthread,NULL);
}
