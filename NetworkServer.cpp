#define PORT_NO 3000

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

class NetworkServer
{
  GameState gstate;
  int sockfd;

  /*networking*/
  void listenForPlayers();
  void sendDataToClient();
  void recieveDataFromClient();
  void processMessage();//updates gstate

};
NetworkServer::NetworkServer(GameState gs)
{
  gstate=gs;
}
void NetworkServer::listenForPlayers()
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

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

}
void NetworkServer::sendDataToClient()
{
}
void NetworkServer::recieveDataFromClient()
{
  char msg[25];
  recv(sockfd,msg,sizeof(msg),0);
  cout<<msg;
}
void NetworkServer::processMessage()
{
}
int NetworkServer::mainThread()
{
}
