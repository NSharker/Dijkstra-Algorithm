CC=g++
CPPFLAGS=-g -Wall -std=c++11 -pedantic

INCLUDES= -I.

DIJKSTRA_OBJ=dijkstra.o
DIJKSTRA_TEST=dijkstra



EXEC_DIR=.


%.o : %.cpp
	$(CC)  -c $< -o $@ $(CPPFLAGS)


$(DIJKSTRA_TEST): $(DIJKSTRA_OBJ)
	$(CC) $(CPPFLAGS) -o $(EXEC_DIR)/$@ $(DIJKSTRA_OBJ) $(INCLUDES)




all:
	make $(DIJKSTRA_TEST)





clean:
	(rm -f *.o;)
	rm -f $(DIJKSTRA_TEST)

