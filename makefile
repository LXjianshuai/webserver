OBJECT=http_main.o https_main.o handler.o
FLAGS=-lpthread -lboost_system -std=c++11 -lssl -lcrypto -I/usr/include/openssl -I/usr/local/openssl/include -L/usr/lib

all:
	make http
	make https

http:http_main.o handler.o
	g++ -o http_main http_main.o handler.o $(FLAGS) 

https:https_main.o handler.o
	g++ -o https_main https_main.o handler.o $(FLAGS)

http_main.o:http_main.cpp
	g++ -c http_main.cpp $(FLAGS)	

https_main.o:https_main.cpp
	g++ -c https_main.cpp $(FLAGS)	

handler.o:handler.cpp
	g++ -c handler.cpp $(FLAGS)

clean:
	rm $(OBJECT)
