#define PORT_NO 3000

class NetworkClient
{
public:
  int sockfd;
  sockaddr_in serv_addr;

  /*networking*/
  void initializeClient();
  void sendMessage(Command);
  void recieveMessage();

  void processMessage(ServerMessage cm);//updates gstate
};
void NetworkClient::initializeClient()
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

     if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
       cout<<"error in connecting";
}

void NetworkClient::sendMessage(Command cmd)
{
  ClientMessage cm;
  gstate.getClientMessage(cm);
  cm.command=cmd;
  send(sockfd,&cm,sizeof(cm),0);
}
void NetworkClient::recieveMessage()
{
  ServerMessage sm;
  recv(sockfd,&sm,sizeof(sm),0);
  cout<<sm.wall_no<<"wall\n";
  processMessage(sm);
}
void NetworkClient::processMessage(ServerMessage sm)
{
  switch(sm.command)
    {
    case INIT:
      cout<<"recieved init"<<endl;
      if(gstate.status==GAME_WAITING)
	{
	  gstate.wall_no=sm.wall_no;
	  gstate.num_balls=sm.num_balls;
	  cout<<"Balls:"<<gstate.num_balls;
	  cout<<"Wall:"<<gstate.wall_no;
	 gstate.updateGameState(sm);
	}
      break;
    case START:
      cout<<"recieved start"<<endl;
      if(gstate.status==GAME_WAITING ||
	 gstate.status==GAME_PAUSED)
	gstate.status=GAME_STARTED;
      break;
    case POSITION:
     if(gstate.status==GAME_STARTED)
       {
	 cout<<"Updating...\n";
	 gstate.updateGameState(sm);
       }
      break;
    case PAUSE:
      if(gstate.status==GAME_STARTED)
	gstate.status=GAME_PAUSED;
      break;
    case QUIT:
      gstate.status=GAME_FINISHED;
      cout<<"\nServer finished";
      break;
    }
}

NetworkClient client;
void* client_main(void*)
{
  ClientMessage cm;
  //game init
  client.initializeClient();
  client.sendMessage(CONNECT);
  gstate.status=GAME_WAITING;
  //game waiting
  client.recieveMessage();
  //game ready
  while(gstate.status!=GAME_STARTED &&
	gstate.status!=GAME_FINISHED)
    {
      client.recieveMessage();
    }
  //game started
  while(gstate.status!=GAME_FINISHED)
    {
      client.sendMessage(POSITION);
      client.recieveMessage();
      usleep(40000);
    }
  //game finished
}
