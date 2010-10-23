all: server_target client_target

client_target: GameClient.cpp NetworkClient.cpp ClientGUI.cpp GameState.cpp GameStructures.h
	g++ -g -o client -lm -lGL -lGLU -lglut GameClient.cpp

server_target: GameServer.cpp NetworkServer.cpp GameState.cpp GameStructures.h
	g++ -g -o server -lm -lpthread GameServer.cpp

false:

run: all
	./server
