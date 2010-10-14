class GameServer
{
  GameState gstate;
  NetworkServer server;

  /*game state*/
  void setupNewGame();
  void calculateNewState();

  void executeGameLoop();

};
void GameServer::executeGameLoop()
{
}
