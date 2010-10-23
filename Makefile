all: server_target client_target

client_target: GameClient.cpp NetworkClient.cpp ClientGUI.cpp GameState.cpp GameStructures.h
	g++ -g -o client -lm -lGL -lGLU -lglut GameClient.cpp

server_target: GameServer.cpp NetworkServer.cpp GameState.cpp GameStructures.h
	g++ -g -o server -lm -lpthread GameServer.cpp

false:

run: all false
	./server 3000 1 1 0.3 &
	./client
	@ killall server

run-network-2: all false
#	PING-PONG-REMOTE-SERVER = localhost
#	ssh siy107536@palasi.cse.iitd.ac.in
	./server 3000 2 1 &
	./client &
	./client
	@ killall server
