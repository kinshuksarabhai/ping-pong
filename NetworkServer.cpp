#define PORT_NO 3000

class NetworkServer
{
public:
  int sockfd;
  int num_players;
  PlayerInfo player[4];

  /*networking*/
  NetworkServer();
  void initializeServer();
  void sendMessage(ServerMessage,int);
  void recieveMessage();
  void processMessage(ClientMessage cm,sockaddr src_addr,socklen_t addrlen);//updates gstate
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
void NetworkServer::sendMessage(ServerMessage sm,int player_no)
{
  sendto(sockfd,&sm,sizeof(sm),0,(sockaddr*)&player[player_no].client_addr,
	 (socklen_t)sizeof(player[player_no].client_addr));
}
void NetworkServer::recieveMessage()
{
  ClientMessage cm;
  sockaddr client_addr;
  socklen_t addrlen;
  recvfrom(sockfd,&cm,sizeof(cm),0,&client_addr,&addrlen);
  processMessage(cm,client_addr,addrlen);
}
void NetworkServer::processMessage(ClientMessage cm,
struct sockaddr client_addr,socklen_t addrlen)
{
  switch(cm.command)
    {
      //    case INIT:
    case CONNECT:
      if(num_players<3)
	{
	  //	  player[num_players].client_addr=client_addr;
	  num_players++;
	}
      break;
    case READY:
      break;
      //    case START:
    case POSITION:
      gstate.updateGameState(cm);
      break;
    case PAUSE:
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
  while(1)
    {
      cout<<"in loop\n";
      server.recieveMessage();
      /*
      gstate.getServerMessage(sm);
      for(int i=0;i<num_players;i++)
      server.sendMessage(sm);
      usleep(100000);*/
      cout<<gstate.paddle[0].position<<"\n";
    }
}
