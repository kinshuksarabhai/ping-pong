#define PORT_NO 3000

class NetworkClient
{
public:
  int sockfd;
  sockaddr_in serv_addr;

  /*networking*/
  void initializeClient();
  void sendMessage(ClientMessage);
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

void NetworkClient::sendMessage(ClientMessage cm)
{
send(sockfd,&cm,sizeof(cm),0);
}
void NetworkClient::recieveMessage()
{
  ServerMessage sm;
  recv(sockfd,&sm,sizeof(sm),0);
  processMessage(sm);
}
void NetworkClient::processMessage(ServerMessage sm)
{
  switch(sm.command)
    {
    case INIT:
      break;
    case START:
      break;
    case POSITION:
      gstate.updateGameState(sm);
      break;
    case PAUSE:
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
  client.sendMessage(cm);
  //  processMessage(smsg);
}
