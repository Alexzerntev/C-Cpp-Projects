#
# In order to execute this "Makefile" just type "make"
#

OBJS = main.o initialize_base.o initialize_lsh.o data_manager.o data_item.o result_item.o metrics.o bf_execution.o enums.o lsh_execution.o random_generator.o h_function.o g_function.o
OUT	= lsh
CC = g++ -g
FLAGS = -c -pedantic -ansi -Wall -std=c++11
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@

# create/compile the individual files >>separately<< 
main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

initialize_base.o: ../common/initialize_base.cpp
	$(CC) $(FLAGS) ../common/initialize_base.cpp

initialize_lsh.o: ./initialize/initialize_lsh.cpp
	$(CC) $(FLAGS) ./initialize/initialize_lsh.cpp

data_manager.o: ../common/data_manipulation/data_manager.cpp
	$(CC) $(FLAGS) ../common/data_manipulation/data_manager.cpp

data_item.o: ../common/data_manipulation/data_item.cpp
	$(CC) $(FLAGS) ../common/data_manipulation/data_item.cpp

result_item.o: ../common/data_manipulation/result_item.cpp
	$(CC) $(FLAGS) ../common/data_manipulation/result_item.cpp

metrics.o: ../common/metrics.cpp
	$(CC) $(FLAGS) ../common/metrics.cpp

random_generator.o: ../common/random_generator.cpp
	$(CC) $(FLAGS) ../common/random_generator.cpp

bf_execution.o: ../brute_force/bf_execution.cpp
	$(CC) $(FLAGS) ../brute_force/bf_execution.cpp

lsh_execution.o: lsh_execution.cpp
	$(CC) $(FLAGS) lsh_execution.cpp

enums.o: ../common/enums.cpp
	$(CC) $(FLAGS) ../common/enums.cpp

h_function.o: ../common/h_function.cpp
	$(CC) $(FLAGS) ../common/h_function.cpp

g_function.o: g_function.cpp
	$(CC) $(FLAGS) g_function.cpp

# clean house
clean:
	rm -f $(OBJS) $(OUT)
	rm -f ../results/lsh/*

clear:
	rm -f ../results/lsh/*