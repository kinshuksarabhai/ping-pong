#define PORT_NO 3000

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
  Game g;
  int sockfd;

  /*game state*/
  void setupNewGame();
  void calculateNewState();

  void executeGameLoop();

  /*networking*/
  void sendDataToClient();
  void recieveDataFromClient();

};
void listenForPlayers()
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
void Server::recieveDataFromClient()
{
  recv();
}
int main()
{
}
