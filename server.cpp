#define PORT_NO 3000

#include <game.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

class Server
{
  Game g;
  sockaddr_in serv_addr;

  /*game state*/
  void setupNewGame();
  void calculateNewState();

  void executeGameLoop();

  /*networking*/
  void listenForPlayers();
  void sendDataToClient();
  void recieveDataFromClient();

};
void listenForPlayers()
{
     int sockfd, newsockfd, portno, clilen;
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);

}
void Server::calculateNewState()
{

}
int main()
{
}
