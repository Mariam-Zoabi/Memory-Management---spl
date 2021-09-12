# All Targets
default: link

#Clean the build directory
clean: 
	@rm -f bin/*

#Link all of our cpp files together
link:	bin/Main.o bin/Session.o bin/Action.o bin/Watchable.o bin/User.o bin/Tags.o
	g++ -o bin/splflix bin/Main.o bin/Session.o bin/Action.o bin/Watchable.o bin/User.o bin/Tags.o

relink: clean link
	

# Complie Main
bin/Main.o: src/Main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Main.o src/Main.cpp

# Complie Session
bin/Session.o: src/Session.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Session.o src/Session.cpp

# Complie Action
bin/Action.o: src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

# Complie Watchable
bin/Watchable.o: src/Watchable.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Watchable.o src/Watchable.cpp

# Complie User
bin/User.o: src/User.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/User.o src/User.cpp

# Complie Tags
bin/Tags.o: src/Tags.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Tags.o src/Tags.cpp

	
