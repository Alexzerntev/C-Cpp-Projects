#
# In order to execute this "Makefile" just type "make"
#

OBJS = main.o data_manager.o data_item.o argument_initialize.o \
enums.o metrics.o random_generator.o cube_execution.o f_function.o \
vertex.o h_function.o g_function.o cluster.o cluster_initialize.o \
cluster_helpers.o cluster_insert.o cluster_update.o

OUT	= cluster
CC = g++ -g
FLAGS = -c -Wall -std=c++11
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@

# create/compile the individual files >>separately<< 
main.o: main.cpp
	$(CC) $(FLAGS) main.cpp

data_manager.o: common/data_manipulation/data_manager.cpp
	$(CC) $(FLAGS) common/data_manipulation/data_manager.cpp

data_item.o: common/data_manipulation/data_item.cpp
	$(CC) $(FLAGS) common/data_manipulation/data_item.cpp

argument_initialize.o: common/argument_initialize.cpp
	$(CC) $(FLAGS) common/argument_initialize.cpp

enums.o: common/enums.cpp
	$(CC) $(FLAGS) common/enums.cpp

metrics.o: common/metrics.cpp
	$(CC) $(FLAGS) common/metrics.cpp

random_generator.o: common/random_generator.cpp
	$(CC) $(FLAGS) common/random_generator.cpp

cube_execution.o: knn/cube/cube_execution.cpp
	$(CC) $(FLAGS) knn/cube/cube_execution.cpp

f_function.o: knn/cube/f_function.cpp
	$(CC) $(FLAGS) knn/cube/f_function.cpp

vertex.o: knn/cube/vertex.cpp
	$(CC) $(FLAGS) knn/cube/vertex.cpp

h_function.o: knn/knn_common/h_function.cpp
	$(CC) $(FLAGS) knn/knn_common/h_function.cpp

g_function.o: knn/lsh/g_function.cpp
	$(CC) $(FLAGS) knn/lsh/g_function.cpp

lsh_execution.o: knn/lsh/lsh_execution.cpp
	$(CC) $(FLAGS) knn/lsh/lsh_execution.cpp

cluster.o: clustering/cluster.cpp
	$(CC) $(FLAGS) clustering/cluster.cpp

cluster_initialize.o: clustering/cluster_initialize.cpp
	$(CC) $(FLAGS) clustering/cluster_initialize.cpp

cluster_helpers.o: clustering/cluster_helpers.cpp
	$(CC) $(FLAGS) clustering/cluster_helpers.cpp

cluster_insert.o: clustering/cluster_insert.cpp
	$(CC) $(FLAGS) clustering/cluster_insert.cpp

cluster_update.o: clustering/cluster_update.cpp
	$(CC) $(FLAGS) clustering/cluster_update.cpp

# clean house
clean:
	rm -f $(OBJS) $(OUT)

#clear:
#	rm -f ../results/lsh/*