#include"NetworkClient.h"

void NetworkClient::initializeClient()
{
    pkt_num=1;
    total_pkts_lost=0;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
    {
        cout<<"ERROR opening socket...Exiting!";
        exit(1);
    }
    else
        cout<<"Client connecting to "<<inet_ntoa(serv_addr.sin_addr)<<":"<<ntohs(serv_addr.sin_port)<<endl;

    if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    {
        cout<<"Error in connecting...";
        exit(1);
    }
}

//NetworkClient client;
void* client_main(void*)
{
    ServerMessage sm;
    //game init
    client.initializeClient();
    client.sendMessage(CONNECT);
    while(gstate.status!=GAME_FINISHED)
    {
        client.receiveMessage(sm);
        client.processMessage(sm);
    }
}
void NetworkClient::receiveMessage(ServerMessage &sm)
{
    int err=recv(sockfd,&sm,sizeof(sm),0);
    if(err==-1)
    {
        perror("Receive error");
        exit(1);
    }
}

void NetworkClient::processMessage(ServerMessage sm)
{

    if(serv_last_pkt_num>=sm.pkt_num)//discard duplicate/pld pkts.
    {
        //      cout<<"Duplicate/old pkt."<<endl;
        return;
    }
    else
    {
        timeval tv;
        gettimeofday(&tv,NULL);
        int lost=sm.pkt_num-serv_last_pkt_num-1;
        //      cout<<"Last Pkt no.:"<< serv_last_pkt_num<<endl;
        //      cout<<lost<<" pkts lost"<<endl;
        serv_last_msg_time=tv;
        serv_last_pkt_num=sm.pkt_num;
        total_pkts_lost+=lost;
    }

    //process the new pkt.
    switch(sm.command)
    {
        case INIT:
            //      cout<<"recieved init"<<endl;
            if(gstate.status==GAME_INIT)
            {
                gstate.wall_no=sm.wall_no;
                gstate.num_balls=sm.num_balls;
                //	  cout<<"Balls:"<<gstate.num_balls<<endl;
                //	  cout<<"Wall:"<<gstate.wall_no<<endl;

                timeval tv;
                gettimeofday(&tv,NULL);
                serv_last_msg_time=tv;
                serv_last_pkt_num=sm.pkt_num;

                gstate.updateGameState(sm);
                gstate.status=GAME_WAITING;
            }
            break;
        case START:
            //      cout<<"recieved start"<<endl;
            if(gstate.status==GAME_READY ||
                    gstate.status==GAME_PAUSED)
            {
                gstate.status=GAME_STARTED;
                //	  cout<<"game started...\n";
            }
            else
                //	cout<<"start:state:"<<gstate.status<<endl;
                break;
        case POSITION:
            switch(gstate.status)
            {
                case GAME_STARTED:
                    //      	 cout<<"."<<endl;
                    gstate.updateGameState(sm);
                    break;
                case GAME_READY:
                case GAME_PAUSED:
                    /*missed START*/
                    //	 cout<<"hey i missed a 'start'..."<<endl;
                    gstate.status=GAME_STARTED;
                    gstate.updateGameState(sm);
            }
            break;
        case PAUSE:
            if(gstate.status==GAME_STARTED)
            {
                gstate.status=GAME_PAUSED;
                //	  cout<<"game paused...\n";
            }
            break;
        case QUIT:
            gstate.status=GAME_FINISHED;
            cout<<"\nServer finished";
            break;
    }
}
void NetworkClient::sendMessage(Command cmd)
{
    ClientMessage cm;
    int sent=0;
    float avg_pkt_loss=(float)total_pkts_lost/(serv_last_pkt_num+1);
    int dup=1.0/(1-avg_pkt_loss);
    gstate.getClientMessage(cm);
    cm.command=cmd;
    cm.pkt_num=pkt_num;
    pkt_num++;

    cout<<"Client: Avg pkt loss:"<<avg_pkt_loss<<endl;
    while(sent<dup && sent<5)
    {
        int err=send(sockfd,&cm,sizeof(cm),0);
        sent++;
        if(err==-1)
        {
            perror("Sending error");
            exit(1);
        }
    }
}
void NetworkClient::startGame()
{
    gstate.status=GAME_READY;
    sendMessage(READY);
}
void NetworkClient::pauseGame()
{
    gstate.status=GAME_PAUSED;
    sendMessage(PAUSE);
}
void NetworkClient::quitGame()
{
    sendMessage(QUIT);
    exit(0);
}
