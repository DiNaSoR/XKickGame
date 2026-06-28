
CC = g++
OBJ = md5.o config.o database.o LogManager.o load.o server.o connect.o que.o util.o init.o packet.o member.o process.o thread.o sql.o table.o main.o
NAME = XKICK_Certify
#DBG = -DMSG -D_REENTRANT -D_PTHREADS -D_THREAD_SAFE
WARN = -Wall
DBG = -D_REENTRANT -D_PTHREADS -D_THREAD_SAFE
#WARN = -Wall -O2
THR_LIB = -lpthread
SQL_LIB = -L/usr/lib/mysql -lmysqlclient -lnsl
SQL_INC = -I/usr/include/mysql

$(NAME): $(OBJ)
	$(CC) -g -o $(NAME) $(OBJ) $(THR_LIB) $(SQL_LIB) $(SQL_INC) $(DBG)

config.o: config.cpp Main.h
	$(CC) -g -c config.cpp $(SQL_INC) $(WARN) $(DBG)

database.o: ../common/database.cpp Main.h
	$(CC) -g -c ../common/database.cpp $(SQL_INC) $(WARN) $(DBG)

LogManager.o: ../common/LogManager.cpp Main.h
	$(CC) -g -c ../common/LogManager.cpp $(SQL_INC) $(WARN) $(DBG)

load.o: load.cpp Main.h
	$(CC) -g -c load.cpp $(SQL_INC) $(WARN) $(DBG)

server.o: server.cpp Main.h
	$(CC) -g -c server.cpp $(SQL_INC) $(WARN) $(DBG)

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

member.o: member.cpp Main.h
	$(CC) -g -c member.cpp $(SQL_INC) $(WARN) $(DBG)

process.o: process.cpp Main.h
	$(CC) -g -c process.cpp $(SQL_INC) $(WARN) $(DBG)

thread.o: thread.cpp Main.h
	$(CC) -g -c thread.cpp $(SQL_INC) $(WARN) $(DBG)

sql.o: sql.cpp Main.h
	$(CC) -g -c sql.cpp $(SQL_INC) $(WARN) $(DBG)

main.o: main.cpp Main.h
	$(CC) -g -c main.cpp $(SQL_INC) $(WARN) $(DBG)

table.o: table.cpp Table.h
	$(CC) -g -c table.cpp $(SQL_INC) $(WARN) $(DBG) 

md5.o: md5.cpp md5.h
	$(CC) -g -c md5.cpp $(WARN) $(DBG) 

clean:
	rm $(OBJ)
	rm $(NAME)

copy:
	cp $(NAME) /sv/football/certify/$(NAME)_sv

