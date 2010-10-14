#include <sys/types.h>
#include <sys/socket.h>

#define PORT_NO 3000

class Client
{
  /*game logic*/
  void executeGameLoop();

  /*networking*/
  void connectToServer();
  void sendDataToServer();
  void recieveDataFromServer();

  /*GUI*/
  void displayGUI();
};

void connectToServer()
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

void Client::sendDataToServer()
{
int size=send(sockfd, const void *buf, size_t len,0);
}
int main()
{

}
