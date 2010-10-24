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

game:
	./game

run-2player: all false
	./server 3000 1 1 0.3 &
	./client &
	./client
	@ killall server

run-network-2: all false
#	PING-PONG-REMOTE-SERVER = localhost
#	ssh siy107536@palasi.cse.iitd.ac.in
	./server 3000 2 1 &
	./client &
	./client
	@ killall server

#automated input test
test1: all false
	./server 3000 1 1 0.3 &
	./client &
	./tester.sh 
	@ killall server

#simulated network delay test
test2:
	sudo tc qdisc add dev lo root netem delay 100ms
	make run
	sudo tc qdisc del dev lo root

#simulated network losses test
test3:
	sudo tc qdisc add dev lo root netem loss 50%
	make run
	sudo tc qdisc del dev lo root

test4:
	sudo tc qdisc add dev lo root netem delay 100ms
	make run
	sudo tc qdisc del dev lo root

