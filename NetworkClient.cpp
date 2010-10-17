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
      gstate.wall_no=sm.wall_no;
      gstate.num_balls=sm.num_balls;
      cout<<"Balls:"<<gstate.num_balls;
      cout<<"Wall:"<<gstate.wall_no;
      break;
    case START:
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
      break;
    }
}

NetworkClient client;
void* client_main(void*)
{
  ClientMessage cm;
  client.initializeClient();
  client.sendMessage(CONNECT);
  gstate.status=GAME_WAITING;

  client.recieveMessage();
  cout<<gstate.wall_no<<"\n";

  //gstate.status=GAME_READY;
  gstate.status=GAME_STARTED;
  while(1)
    {
      cout<<"send pos:\n";
      gstate.getClientMessage(cm);
      client.sendMessage(POSITION);
      cout<<"recv pos:\n";
      client.recieveMessage();
      cout<<"("<<gstate.ball[0].position.x<<","<<gstate.ball[0].position.y<<")\n";
      usleep(40000);
    }
}
