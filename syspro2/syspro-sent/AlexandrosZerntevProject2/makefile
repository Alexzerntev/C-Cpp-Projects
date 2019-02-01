#
# In order to execute this "Makefile" just type "make"
#

INC		= include
SRC 	= src

OBJS 	= main.o user_interaction.o map_list_node.o path_list_node.o order_string.o result_string.o result_list.o worker_handler.o map.o posting_list.o posting_list_node.o trie_node.o string.o document_string.o
SOURCE	= main.cpp $(SRC)/user_interaction.cpp $(SRC)/string/map_list_node.cpp $(SRC)/string/path_list_node.cpp $(SRC)/string/order_string.cpp $(SRC)/string/result_string.cpp $(SRC)/string/result_list.cpp $(SRC)/worker_handler/worker_handler.cpp $(SRC)/project_one/data_structs/map/map.cpp $(SRC)/project_one/data_structs/posting_list/posting_list.cpp $(SRC)/project_one/data_structs/posting_list/posting_list_node.cpp $(SRC)/project_one/data_structs/trie/trie_node.cpp $(SRC)/project_one/string/string.cpp $(SRC)/project_one/string/document_string.cpp
HEADER  = main.hpp $(INC)/user_interaction.hpp $(INC)/string/map_list_node.hpp $(INC)/string/path_list_node.hpp $(INC)/string/order_string.hpp $(INC)/string/result_string.hpp $(INC)/string/result_list.hpp $(INC)/worker_handler/worker_handler.hpp $(INC)/project_one/data_structs/map/map.hpp $(INC)/project_one/data_structs/posting_list/posting_list.hpp $(INC)/project_one/data_structs/posting_list/posting_list_node.hpp $(INC)/project_one/data_structs/trie/trie_node.hpp $(INC)/project_one/string/string.hpp $(INC)/project_one/string/document_string.hpp
OUT  	= jobExecuter
CC	= g++
FLAGS   = -g -c -pedantic -ansi  -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) -g $(OBJS) -o $@

# create/compile the individual files >>separately<< 
user_interaction.o: $(SRC)/user_interaction.cpp
	$(CC) $(FLAGS) $(SRC)/user_interaction.cpp

map_list_node.o: $(SRC)/string/map_list_node.cpp
	$(CC) $(FLAGS) $(SRC)/string/map_list_node.cpp

path_list_node.o: $(SRC)/string/path_list_node.cpp
	$(CC) $(FLAGS) $(SRC)/string/path_list_node.cpp

order_string.o: $(SRC)/string/order_string.cpp
	$(CC) $(FLAGS) $(SRC)/string/order_string.cpp

result_string.o: $(SRC)/string/result_string.cpp
	$(CC) $(FLAGS) $(SRC)/string/result_string.cpp

result_list.o: $(SRC)/string/result_list.cpp
	$(CC) $(FLAGS) $(SRC)/string/result_list.cpp

worker_handler.o: $(SRC)/worker_handler/worker_handler.cpp
	$(CC) $(FLAGS) $(SRC)/worker_handler/worker_handler.cpp

map.o: $(SRC)/project_one/data_structs/map/map.cpp
	$(CC) $(FLAGS) $(SRC)/project_one/data_structs/map/map.cpp

posting_list.o: $(SRC)/project_one/data_structs/posting_list/posting_list.cpp
	$(CC) $(FLAGS) $(SRC)/project_one/data_structs/posting_list/posting_list.cpp

posting_list_node.o: $(SRC)/project_one/data_structs/posting_list/posting_list_node.cpp
	$(CC) $(FLAGS) $(SRC)/project_one/data_structs/posting_list/posting_list_node.cpp

trie_node.o: $(SRC)/project_one/data_structs/trie/trie_node.cpp
	$(CC) $(FLAGS) $(SRC)/project_one/data_structs/trie/trie_node.cpp

string.o: $(SRC)/project_one/string/string.cpp
	$(CC) $(FLAGS) $(SRC)/project_one/string/string.cpp

document_string.o: $(SRC)/project_one/string/document_string.cpp
	$(CC) $(FLAGS) $(SRC)/project_one/string/document_string.cpp

# clean house
clean:
	rm -f $(OBJS) $(OUT)
	rm -f log/*

# do a bit of accounting
count:
	wc $(SOURCE) $(HEADER)