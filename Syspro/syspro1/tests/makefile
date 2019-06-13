OBJS = tests_main.o io_manager_test.o error_handler.o io_manager.o settings.o \
string_handler.o string_handler_test.o user.o bitcoin_tree_test.o bitcoin_tree.o \
user_test.o transaction_list_test.o transaction_list.o hashtable.o hashtable_test.o \
date_time_test.o date_time.o user_interaction_test.o user_interaction.o
OUT	= tests
CC = g++ -g
FLAGS = -c -Wall -std=c++11 -pthread
LIBS = -lgtest_main -lgtest -lpthread
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

# Test Files
tests_main.o: tests_main.cpp
	$(CC) $(FLAGS) tests_main.cpp $(LIBS)

io_manager_test.o: io_manager_test.cpp
	$(CC) $(FLAGS) io_manager_test.cpp $(LIBS)

string_handler_test.o: string_handler_test.cpp
	$(CC) $(FLAGS) string_handler_test.cpp $(LIBS)

bitcoin_tree_test.o: bitcoin_tree_test.cpp
	$(CC) $(FLAGS) bitcoin_tree_test.cpp $(LIBS)

user_test.o: user_test.cpp
	$(CC) $(FLAGS) user_test.cpp $(LIBS)

transaction_list_test.o: transaction_list_test.cpp
	$(CC) $(FLAGS) transaction_list_test.cpp $(LIBS)

hashtable_test.o: hashtable_test.cpp
	$(CC) $(FLAGS) hashtable_test.cpp $(LIBS)

date_time_test.o: date_time_test.cpp
	$(CC) $(FLAGS) date_time_test.cpp $(LIBS)

user_interaction_test.o: user_interaction_test.cpp
	$(CC) $(FLAGS) user_interaction_test.cpp $(LIBS)


# Files TO Test
error_handler.o: ../error_handler/error_handler.c
	$(CC) $(FLAGS) ../error_handler/error_handler.c $(LIBS)

io_manager.o: ../io/io_manager.c
	$(CC) $(FLAGS) ../io/io_manager.c $(LIBS)

settings.o: ../model/settings/settings.c
	$(CC) $(FLAGS) ../model/settings/settings.c $(LIBS)

user.o: ../model/user/user.c
	$(CC) $(FLAGS) ../model/user/user.c $(LIBS)

string_handler.o: ../string/string_handler.c
	$(CC) $(FLAGS) ../string/string_handler.c $(LIBS)

bitcoin_tree.o: ../data_structures/bitcoin_tree/bitcoin_tree.c
	$(CC) $(FLAGS) ../data_structures/bitcoin_tree/bitcoin_tree.c $(LIBS)

transaction_list.o: ../data_structures/transaction_list/transaction_list.c
	$(CC) $(FLAGS) ../data_structures/transaction_list/transaction_list.c $(LIBS)

hashtable.o: ../data_structures/hashtable/hashtable.c
	$(CC) $(FLAGS) ../data_structures/hashtable/hashtable.c $(LIBS)

date_time.o: ../date_time/date_time.c
	$(CC) $(FLAGS) ../date_time/date_time.c $(LIBS)

user_interaction.o: ../io/user_interaction.c
	$(CC) $(FLAGS) ../io/user_interaction.c $(LIBS)

id_generator.o: ../id_generator/id_generator.c
	$(CC) $(FLAGS) ../id_generator/id_generator.c $(LIBS)


# clean house
clean:
	rm -f $(OBJS) $(OUT)