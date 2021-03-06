#include"NetworkServer.h"

NetworkServer::NetworkServer()
{
  num_players=0;
  pthread_mutex_init(&sockmutex,NULL);
  for(int i=0;i<4;i++)
    {
      players[i].last_pkt_num=0;
      players[i].serv_pkt_num=1;
      players[i].total_pkts_lost=0;
    }
}
void NetworkServer::initializeServer()
{
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    {
      cout<<"ERROR opening socket...Exiting!";
      exit(1);
    }
  else
    cout<<"Server started at "<<ntohs(serv_addr.sin_port)<<"\n";
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0) 
    {
      cout<<"ERROR on binding...";
      exit(1);
    }
}

//NetworkServer server;
void* server_main(void*)
{
  ClientMessage cm;
  sockaddr_in client_addr;

  //game init
  server.initializeServer();
  gstate.status=GAME_WAITING;
  while(gstate.status!=GAME_FINISHED)
    {
      server.receiveMessage(cm,client_addr);
      server.processMessage(cm,client_addr);
    }
}

void NetworkServer::receiveMessage(ClientMessage &cm,sockaddr_in &client_addr)
{
  socklen_t addrlen=sizeof(sockaddr_in);
  int err=recvfrom(sockfd,&cm,sizeof(cm),0,(sockaddr*)&client_addr,&addrlen);

  if(err==-1)
    {
    perror("Receive Error");
    exit(1);
    }
}
void NetworkServer::processMessage(ClientMessage cm,sockaddr_in client_addr)
{
  int w=getWallNo(client_addr);

  if(w!=-1)//registered players
    {/*regular update*/

      if(players[w].last_pkt_num>=cm.pkt_num)//discard duplicate/pld pkts.
	{
//	  cout<<"Duplicate/old pkt."<<endl;
	  return;
	}
      else
	{
	  timeval tv;
	  gettimeofday(&tv,NULL);
	  int lost=cm.pkt_num-players[w].last_pkt_num-1;
// 	  cout<<"Last Pkt no.:"<< players[w].last_pkt_num<<endl;
// 	  cout<<lost<<" pkts lost"<<endl;
	  players[w].last_msg_time=tv;
	  players[w].last_pkt_num=cm.pkt_num;
	  players[w].total_pkts_lost+=lost;
	}
    }
  else if(cm.command!=CONNECT)//new player and not connecting...?
    return;

  //process the new pkt.
  switch(cm.command)
    {
      //    case INIT:
    case CONNECT:
      cout<<"Connection request from:"<<inet_ntoa(client_addr.sin_addr);
      cout<<":"<<ntohs(client_addr.sin_port)<<endl;
      if(w!=-1) //registered player? may be he did not get INIT...
	{
//	  cout<<"Rcvf connect again...";
	  sendMessage(INIT,w);
	}
      else if(gstate.status==GAME_WAITING && num_players<gstate.num_players)
	{
	  /*initialize/update player data*/
	  int num=alloc_seq[num_players];
	  gstate.paddle[num].ptype=HUMAN;
	  gstate.paddle[num].pstate=PLAYER_CONNECTED;
	  players[num].client_addr=client_addr;

	  timeval tv;
	  gettimeofday(&tv,NULL);
	  players[num].last_msg_time=tv;
	  players[num].last_pkt_num=cm.pkt_num;

	  num_players++;
	  cout<<num_players<<" Connected...\n";
	  sendMessage(INIT,num);
	}
      break;

    case READY:
//      cout<<"ready mesg...\n";
      switch(gstate.status)
	{
	case GAME_WAITING:
	case GAME_READY:
	case GAME_PAUSED:
	  tryToStartGame(w);
	  break;
	case GAME_STARTED:
	  //why waiting (the client)? start the game...
	  sendMessage(START,w);
	}
      break;
      //    case START:
    case POSITION:
      switch(gstate.status)
	{
	case GAME_STARTED:
	gstate.updateGameState(cm,w);
     //	cout<<"Updated paddle position...\n";
	break;
	case GAME_PAUSED:
	  //seems that client missed a PAUSE
	  server.sendMessage(PAUSE,w);
	}
      break;

    case PAUSE:
      if(gstate.status==GAME_STARTED)
	{
	  gstate.status=GAME_PAUSED;

	  /*tell other clients to pause*/
	  for(int i=0;i<4;i++)
	    {
	      if(gstate.paddle[i].ptype==HUMAN)
		{
		  gstate.paddle[i].pstate=PLAYER_PAUSED;
		  sendMessage(PAUSE,i);
		}
	    }
	}
      break;

    case QUIT:
      gstate.paddle[w].pstate=PLAYER_FINISHED;
      break;
    }
}
void NetworkServer::tryToStartGame(int wall_no)
{
//  cout<<"try start:"<<wall_no<<endl;
  gstate.paddle[wall_no].pstate=PLAYER_READY;

  /*check if all ready*/
  int flag=1;
  if(num_players!=gstate.num_players)
    flag=0;
  else
    {
      for(int i=0;i<4;i++)
	if(gstate.paddle[i].ptype==HUMAN && gstate.paddle[i].pstate!=PLAYER_READY)
	  flag=0;
    }

  if(flag==1)
    {
//      cout<<"All ready... starting game!!\n";
      /*start the game*/
      for(int i=0;i<4;i++)
	if(gstate.paddle[i].ptype==HUMAN)
	{
	  sendMessage(START,i);
	  gstate.paddle[i].pstate=PLAYER_PLAYING;
	}
      gstate.status=GAME_STARTED;
    }
//  else
//    cout<<"not all ready"<<endl;
}
void NetworkServer::sendMessage(Command cmd,int wall_no)
{
  ServerMessage sm;
  Command c;
  int sent=0;
  float avg_pkt_loss=(float)players[wall_no].total_pkts_lost/(players[wall_no].last_pkt_num+1);
  int dup=1.0/(1-avg_pkt_loss);

  gstate.getServerMessage(sm);
  sm.command=cmd;
  sm.pkt_num=players[wall_no].serv_pkt_num;
  sm.wall_no=wall_no;

  players[wall_no].serv_pkt_num++;

  pthread_mutex_lock(&sockmutex);
  cout<<"Server: Avg pkt loss:"<<avg_pkt_loss<<endl;
  while(sent<dup && sent<5)
    {
    int err=sendto(sockfd,&sm,sizeof(sm),0,
		   (sockaddr*)&players[wall_no].client_addr,
		   (socklen_t)sizeof(sockaddr_in));
    sent++;
    if(err==-1)
      {
      perror("Sending error");
      exit(1);
      }
    }
   pthread_mutex_unlock(&sockmutex);
}

 int NetworkServer::getWallNo(sockaddr_in client_addr)
 {
   for(int i=0;i<4;i++)
     {
       if(gstate.paddle[i].ptype==HUMAN &&
	  !memcmp(&players[i].client_addr,&client_addr,sizeof(sockaddr_in)))
	 return i;
     }
   return -1;
 }
void NetworkServer::playerQuit(int wall_no)
{
  gstate.paddle[wall_no].pstate=PLAYER_FINISHED;
}

