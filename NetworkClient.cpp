#include <sys/types.h>
#include <sys/socket.h>

#define PORT_NO 3000

class NetworkClient
{
  GameState gstate;

public:
  NetworkClient(GameState);

  /*networking*/
  void connectToServer();
  void sendDataToServer();
  void recieveDataFromServer();

  void processMessage();//updates gstate
};
void NetworkClient::mainThread()
{
  ServerMessage smsg;
  connectToServer();
  smsg=recieveDataFromServer();
  processMessage(smsg);
}
NetworkClient(GameState gs)
{
  gstate=gs;
}
void NetworkClient::connectToServer()
{
     int portno;
     sockaddr_in serv_addr;

     sockfd = socket(AF_INET, SOCK_DGRAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = PORT_NO;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);

     if(connect(sockfd,(struct sockaddr *)serv_addr,sizeof(serv_addr))<0)
       cout<<"error in connecting";
}

void NetworkClient::sendDataToServer()
{
  char msg[]="hello";
  int size=send(sockfd,msg,sizeof(msg),0);
  cout<<"msg sent";
}
void NetworkClient::recieveDataFromServer()
{
}
int main()
{
  GameState gs;
  NetworkClient nc(gs);
  nc.connectToServer();
}
