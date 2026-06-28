
CC = g++
OBJ = rndshop.o config.o load.o database.o LogManager.o secure.o table.o connect.o que.o util.o init.o packet.o player.o server.o item.o skill.o ceremony.o training.o quest.o room.o lobby.o process.o thread.o sql.o main.o
NAME = XKICK_Game
DBG = -DMSG -D_REENTRANT -D_PTHREADS -D_THREAD_SAFE
WARN = -Wall
#DBG = -D_REENTRANT -D_PTHREADS -D_THREAD_SAFE
#WARN = -Wall -O2
THR_LIB = -lpthread
SQL_LIB = -L/usr/lib/mysql -lmysqlclient -lnsl
SQL_INC = -I/usr/include/mysql


$(NAME): $(OBJ) 
	$(CC) -g -c init.cpp $(SQL_INC) $(WARN) $(DBG)
	$(CC) -g -o $(NAME) $(OBJ) $(THR_LIB) $(SQL_LIB) $(SQL_INC) $(DBG)

rndshop.o: rndshop.cpp Main.h
	$(CC) -g -c rndshop.cpp $(SQL_INC) $(WARN) $(DBG)

config.o: config.cpp Main.h
	$(CC) -g -c config.cpp $(SQL_INC) $(WARN) $(DBG)

load.o: load.cpp Main.h
	$(CC) -g -c load.cpp $(SQL_INC) $(WARN) $(DBG)

database.o: ../common/database.cpp Main.h
	$(CC) -g -c ../common/database.cpp $(SQL_INC) $(WARN) $(DBG)

LogManager.o: ../common/LogManager.cpp Main.h
	$(CC) -g -c ../common/LogManager.cpp $(SQL_INC) $(WARN) $(DBG)

secure.o: ../common/secure.cpp Main.h
	$(CC) -g -c ../common/secure.cpp $(SQL_INC) $(WARN) $(DBG)

table.o: table.cpp Main.h
	$(CC) -g -c table.cpp $(SQL_INC) $(WARN) $(DBG)

connect.o: connect.cpp Main.h
	$(CC) -g -c connect.cpp $(SQL_INC) $(WARN) $(DBG)

que.o: que.cpp Main.h
	$(CC) -g -c que.cpp $(SQL_INC) $(WARN) $(DBG)

util.o: util.cpp Main.h
	$(CC) -g -c util.cpp $(SQL_INC) $(WARN) $(DBG)

init.o: init.cpp Main.h version.h
	$(CC) -g -c init.cpp $(SQL_INC) $(WARN) $(DBG)

packet.o: packet.cpp Main.h
	$(CC) -g -c packet.cpp $(SQL_INC) $(WARN) $(DBG)

player.o: player.cpp Main.h
	$(CC) -g -c player.cpp $(SQL_INC) $(WARN) $(DBG)

server.o: server.cpp Main.h
	$(CC) -g -c server.cpp $(SQL_INC) $(WARN) $(DBG)

item.o: item.cpp Main.h
	$(CC) -g -c item.cpp $(SQL_INC) $(WARN) $(DBG)

skill.o: skill.cpp Main.h
	$(CC) -g -c skill.cpp $(SQL_INC) $(WARN) $(DBG)

ceremony.o: ceremony.cpp Main.h
	$(CC) -g -c ceremony.cpp $(SQL_INC) $(WARN) $(DBG)

training.o: training.cpp Main.h
	$(CC) -g -c training.cpp $(SQL_INC) $(WARN) $(DBG)

quest.o: quest.cpp Main.h
	$(CC) -g -c quest.cpp $(SQL_INC) $(WARN) $(DBG)

room.o: room.cpp Main.h
	$(CC) -g -c room.cpp $(SQL_INC) $(WARN) $(DBG)

lobby.o: lobby.cpp Main.h
	$(CC) -g -c lobby.cpp $(SQL_INC) $(WARN) $(DBG)

process.o: process.cpp Main.h
	$(CC) -g -c process.cpp $(SQL_INC) $(WARN) $(DBG)

thread.o: thread.cpp Main.h
	$(CC) -g -c thread.cpp $(SQL_INC) $(WARN) $(DBG)

sql.o: sql.cpp Main.h
	$(CC) -g -c sql.cpp $(SQL_INC) $(WARN) $(DBG)

main.o: main.cpp Main.h 
	$(CC) -g -c main.cpp $(SQL_INC) $(WARN) $(DBG)

clean:
	rm $(OBJ)
	rm $(NAME)

copy:
	cp $(NAME) /sv/football/game1/$(NAME)_1
	cp $(NAME) /sv/football/game2/$(NAME)_2

