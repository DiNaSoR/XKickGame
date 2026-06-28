
CC = g++
NAME = version_maker
DBG = -DMSG -D_REENTRANT -D_PTHREADS -D_THREAD_SAFE
WARN = -Wall
#DBG = -D_REENTRANT -D_PTHREADS -D_THREAD_SAFE
#WARN = -Wall -O2

$(NAME): main.o
		   	$(CC) -g -o $(NAME) main.o $(DBG)

main.o: main.cpp
			$(CC) -g -c main.cpp $(DBG)

