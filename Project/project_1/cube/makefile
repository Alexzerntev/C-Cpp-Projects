#
# In order to execute this "Makefile" just type "make"
#

OBJS = main.o initialize_base.o initialize_cube.o data_manager.o data_item.o result_item.o metrics.o bf_execution.o enums.o cube_execution.o random_generator.o h_function.o f_function.o vertex.o
OUT	= cube
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

initialize_cube.o: ./initialize/initialize_cube.cpp
	$(CC) $(FLAGS) ./initialize/initialize_cube.cpp

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

cube_execution.o: cube_execution.cpp
	$(CC) $(FLAGS) cube_execution.cpp

enums.o: ../common/enums.cpp
	$(CC) $(FLAGS) ../common/enums.cpp

h_function.o: ../common/h_function.cpp
	$(CC) $(FLAGS) ../common/h_function.cpp

f_function.o: f_function.cpp
	$(CC) $(FLAGS) f_function.cpp

vertex.o: vertex.cpp
	$(CC) $(FLAGS) vertex.cpp

# clean house
clean:
	rm -f $(OBJS) $(OUT)
	#rm -f ../results/cube/*