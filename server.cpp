#include <game.h>
class Server
{
  Game g;

  /*game state*/
  void setupNewGame();
  void calculateNewState();

  void executeGameLoop();

  /*networking*/
  void listenForPlayers();
  void sendDataToClient();
  void recieveDataFromClient();

};

void Server::calculateNewState()
{

}
int main()
{
}
