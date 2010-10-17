#define PORT_NO 3000
int alloc_seq[]={0,2,1,3};
class NetworkServer
{
public:
  int sockfd;
  int num_players;

  /*networking*/
  NetworkServer();
  void initializeServer();
  void sendMessage(Command,int);
  void recieveMessage();
  void processMessage(ClientMessage cm,sockaddr_in src_addr,socklen_t addrlen);//updates gstate
};
NetworkServer::NetworkServer()
{
  num_players=0;
}
void NetworkServer::initializeServer()
{
     int portno;
     sockaddr_in serv_addr;

     sockfd = socket(AF_INET, SOCK_DGRAM, 0);
     if (sockfd < 0) 
       cout<<"ERROR opening socket";

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = PORT_NO;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
       cout<<"ERROR on binding";
     cout<<"Server started @ "<<PORT_NO<<"\n";
}
void NetworkServer::sendMessage(Command cmd,int wall_no)
{
  ServerMessage sm;
  gstate.getServerMessage(sm);
  sm.command=cmd;
  sm.wall_no=wall_no;
  sendto(sockfd,&sm,sizeof(sm),0,(sockaddr*)&gstate.paddle[wall_no].client_addr,
	 (socklen_t)sizeof(gstate.paddle[wall_no].client_addr));
}
void NetworkServer::recieveMessage()
{
  ClientMessage cm;
  sockaddr_in client_addr;
  socklen_t addrlen;
  recvfrom(sockfd,&cm,sizeof(cm),0,(sockaddr*)&client_addr,&addrlen);
  processMessage(cm,client_addr,addrlen);
}
void NetworkServer::processMessage(ClientMessage cm,
sockaddr_in client_addr,socklen_t addrlen)
{
  ServerMessage sm;
  switch(cm.command)
    {
      //    case INIT:
    case CONNECT:
     if(gstate.status==GAME_WAITING && num_players<4)
	{
	  int num=alloc_seq[num_players];
	  gstate.paddle[num].client_addr=client_addr;
	  gstate.paddle[num].pstate=PLAYER_CONNECTED;
	  sendMessage(INIT,num);
	  num_players++;
	}
      break;
    case READY:
      break;
      //    case START:
    case POSITION:
      if(gstate.status==GAME_STARTED)
	gstate.updateGameState(cm);
      break;
    case PAUSE:
      if(gstate.status==GAME_STARTED)
	{
	gstate.status=GAME_PAUSED;
	for(int i=0;i<num_players;i++)
	  sendMessage(PAUSE,alloc_seq[i]);
	  }
      break;
    case QUIT:
      break;
    }
}

NetworkServer server;
void* server_main(void*)
{
  ServerMessage sm;

  server.initializeServer();
  gstate.status=GAME_WAITING;
  while(server.num_players<2)
    {
      server.recieveMessage();
    }

  //  gstate.status=GAME_READY;
  gstate.status=GAME_STARTED;
  while(1)
    {
      for(int i=0;i<server.num_players;i++)
	{
      cout<<"recv pos:\n";
	server.recieveMessage();
      cout<<"send pos:\n";
      cout<<"("<<gstate.ball[0].position.x<<","<<gstate.ball[0].position.y<<")\n";
	server.sendMessage(POSITION,alloc_seq[i]);
	usleep(40000);
	}
    }
}
