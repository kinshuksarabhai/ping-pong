#define PORT_NO 3000

class NetworkClient
{
public:
  int sockfd;
  sockaddr_in serv_addr;

  int serv_pkt_num;
  int pkt_num;
  /*networking*/
  void initializeClient();
  void sendMessage(Command);
  void recieveMessage();

  void processMessage(ServerMessage cm);//updates gstate
  void dispatchMessage();

  void startGame();
  void pauseGame();
  void quitGame();
};
void NetworkClient::initializeClient()
{
  pkt_num=1;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    cout<<"ERROR opening socket";

  cout<<"Client connecting to "<<inet_ntoa(serv_addr.sin_addr)<<":"<<ntohs(serv_addr.sin_port)<<endl;

  if(connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    cout<<"error in connecting";
}

NetworkClient client;
void* client_main(void*)
{
  //game init
  client.initializeClient();
  client.sendMessage(CONNECT);
  gstate.status=GAME_WAITING;
  while(gstate.status!=GAME_FINISHED)
    {
      client.dispatchMessage();
    }
}
void NetworkClient::dispatchMessage()
{
  ServerMessage sm;
  int err=recv(sockfd,&sm,sizeof(sm),0);
  if(err==-1)
    perror("Receive error");
  else
    {
     cout<<"Mesg recvd from:"<<inet_ntoa(client_addr.sin_addr)<<":"<<ntohs(client_addr.sin_port)<<endl;
     cout<<"Command:"<<cm.command<<endl;
     cout<<"Pkt no.:"<<cm.pkt_num<<endl;
  switch(sm.command)
    {
    case INIT:
      cout<<"recieved init"<<endl;
      if(gstate.status==GAME_READY||gstate.status==GAME_WAITING)
	{
	  gstate.wall_no=sm.wall_no;
	  gstate.num_balls=sm.num_balls;
	  cout<<"Balls:"<<gstate.num_balls<<endl;
	  cout<<"Wall:"<<gstate.wall_no<<endl;
	 gstate.updateGameState(sm);
	 gstate.status=GAME_READY;
	}
      break;
    case START:
      cout<<"recieved start"<<endl;
      if(gstate.status==GAME_READY ||
	 gstate.status==GAME_PAUSED)
	{
	gstate.status=GAME_STARTED;
	  cout<<"game started...\n";
	}
      else
	cout<<"start:state:"gstate.status<<endl;
      break;
    case POSITION:
     if(gstate.status==GAME_STARTED)
       {
      	 cout<<".";
	 gstate.updateGameState(sm);
       }
      break;
    case PAUSE:
      if(gstate.status==GAME_STARTED)
	{
	gstate.status=GAME_PAUSED;
	  cout<<"game paused...\n";
	}
      break;
    case QUIT:
      gstate.status=GAME_FINISHED;
      cout<<"\nServer finished";
      break;
    }
    }
}
void NetworkClient::sendMessage(Command cmd)
{
  ClientMessage cm;
  int sent=0;
  gstate.getClientMessage(cm);
  cm.command=cmd;
  cm.pkt_num=pkt_num;
  pkt_num++;
  cout<<"Sending command:"<<cm.command<<endl;
  int err=send(sockfd,&cm,sizeof(cm),0);
  sent++;
  if(err==-1)
    perror("Sending error");
}
void NetworkClient::startGame()
{
  sendMessage(READY);
  gstate.status=GAME_READY;
}
void NetworkClient::pauseGame()
{
  sendMessage(PAUSE);
  gstate.status=GAME_PAUSED;
}
void NetworkClient::quitGame()
{
  sendMessage(QUIT);
  exit(0);
}
