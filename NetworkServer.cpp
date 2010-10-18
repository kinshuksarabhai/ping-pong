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
      cout<<"Connection request...\n";
     if(gstate.status==GAME_WAITING && num_players<gstate.num_players)
	{
	  int num=alloc_seq[num_players];
	  gstate.paddle[num].client_addr=client_addr;
	  gstate.paddle[num].pstate=PLAYER_CONNECTED;
	  sendMessage(INIT,num);
	  num_players++;
	  cout<<num_players<<" Connected...\n";
	}
      break;
    case READY:
      cout<<"ready mesg...\n";
      if(gstate.status==GAME_READY || gstate.status==GAME_PAUSED)
	{
	  gstate.paddle[cm.wall_no].pstate=PLAYER_READY;
	  cout<<num_players<<" players\n";
	  /*check if all ready*/
	  int flag=1;
	  for(int i=0;i<num_players;i++)
	    {
	      int num=alloc_seq[i];
	      if(gstate.paddle[num].pstate!=PLAYER_READY)
		flag=0;
	    }
	  if(flag==1)
	    {
	      cout<<"All ready... starting game!!\n";
	      /*start the game*/
	      gstate.status=GAME_STARTED;
	      for(int i=0;i<num_players;i++)
		{
		  int num=alloc_seq[i];
		  gstate.paddle[num].pstate=PLAYER_PLAYING;
		  sendMessage(START,num);
		}
	    }
	  else
	    cout<<"Not ready yet...\n";
	}
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
	  {
	    int num=alloc_seq[i];
	    gstate.paddle[num].pstate=PLAYER_PAUSED;
	      sendMessage(PAUSE,num);
	  }
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
  //game init
  server.initializeServer();
  gstate.status=GAME_WAITING;
  //game waiting
  while(server.num_players<gstate.num_players)
    {
      server.recieveMessage();
    }
  gstate.status=GAME_READY;
  //game ready
  while(gstate.status!=GAME_STARTED)
    {
      server.recieveMessage();
    }
  //game started
  while(gstate.status!=GAME_FINISHED)
    {
      for(int i=0;i<server.num_players;i++)
	{
	server.recieveMessage();
	server.sendMessage(POSITION,alloc_seq[i]);
	cout<<"Position sent...\n";
	usleep(40000);
	}
    }
  //game finished
}
