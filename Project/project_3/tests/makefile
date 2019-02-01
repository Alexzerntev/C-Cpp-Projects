#
# In order to execute this "Makefile" just type "make"
#

OBJS = tests_main.o data_manager_test.o argument_initialize_test.o \
argument_initialize.o enums.o cluster_initialize_test.o cluster_initialize.o \
random_generator.o mockup_dataset.o cluster_helpers.o cluster_helpers_test.o \
cluster_insert_lloyd_test.o cluster_insert_lsh_test.o cluster_insert_cube_test.o \
cluster_update.o cluster_update_kmeans_test.o cluster_update_pam_test.o \
silhouette.o silhouette_test.o user_data_item.o sentiment_test.o sentiment.o \
recomendation_helpers_test.o recomendation_helpers.o data_item_test.o
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

data_manager_test.o: data_manager_test.cpp
	$(CC) $(FLAGS) data_manager_test.cpp $(LIBS)

argument_initialize_test.o: argument_initialize_test.cpp
	$(CC) $(FLAGS) argument_initialize_test.cpp $(LIBS)

cluster_initialize_test.o: cluster_initialize_test.cpp
	$(CC) $(FLAGS) cluster_initialize_test.cpp $(LIBS)

cluster_helpers_test.o: cluster_helpers_test.cpp
	$(CC) $(FLAGS) cluster_helpers_test.cpp $(LIBS)

cluster_insert_lloyd_test.o: cluster_insert_lloyd_test.cpp
	$(CC) $(FLAGS) cluster_insert_lloyd_test.cpp $(LIBS)

cluster_insert_lsh_test.o: cluster_insert_lsh_test.cpp
	$(CC) $(FLAGS) cluster_insert_lsh_test.cpp $(LIBS)

cluster_insert_cube_test.o: cluster_insert_cube_test.cpp
	$(CC) $(FLAGS) cluster_insert_cube_test.cpp $(LIBS)

cluster_update_kmeans_test.o: cluster_update_kmeans_test.cpp
	$(CC) $(FLAGS) cluster_update_kmeans_test.cpp $(LIBS)

cluster_update_pam_test.o: cluster_update_pam_test.cpp
	$(CC) $(FLAGS) cluster_update_pam_test.cpp $(LIBS)

silhouette_test.o: silhouette_test.cpp
	$(CC) $(FLAGS) silhouette_test.cpp $(LIBS)

sentiment_test.o: sentiment_test.cpp
	$(CC) $(FLAGS) sentiment_test.cpp $(LIBS)

recomendation_helpers_test.o: recomendation_helpers_test.cpp
	$(CC) $(FLAGS) recomendation_helpers_test.cpp $(LIBS)

data_item_test.o: data_item_test.cpp
	$(CC) $(FLAGS) data_item_test.cpp $(LIBS)


# Files TO Test
argument_initialize.o: ../common/argument_initialize.cpp
	$(CC) $(FLAGS) ../common/argument_initialize.cpp $(LIBS)

enums.o: ../common/enums.cpp
	$(CC) $(FLAGS) ../common/enums.cpp $(LIBS)

random_generator.o: ../common/random_generator.cpp
	$(CC) $(FLAGS) ../common/random_generator.cpp $(LIBS)

cluster_initialize.o: ../clustering/cluster_initialize.cpp
	$(CC) $(FLAGS) ../clustering/cluster_initialize.cpp $(LIBS)

mockup_dataset.o: mockups/mockup_dataset.cpp
	$(CC) $(FLAGS) mockups/mockup_dataset.cpp $(LIBS)

cluster_helpers.o: ../clustering/cluster_helpers.cpp
	$(CC) $(FLAGS) ../clustering/cluster_helpers.cpp $(LIBS)

cluster_update.o: ../clustering/cluster_update.cpp
	$(CC) $(FLAGS) ../clustering/cluster_update.cpp $(LIBS)

silhouette.o: ../evaluation/silhouette.cpp
	$(CC) $(FLAGS) ../evaluation/silhouette.cpp $(LIBS)

user_data_item.o: ../common/data_manipulation/user_data_item.cpp
	$(CC) $(FLAGS) ../common/data_manipulation/user_data_item.cpp

sentiment.o: ../recomendation/sentiment.cpp
	$(CC) $(FLAGS) ../recomendation/sentiment.cpp

recomendation_helpers.o: ../recomendation/recomendation_helpers.cpp
	$(CC) $(FLAGS) ../recomendation/recomendation_helpers.cpp

# clean house
clean:
	rm -f $(OBJS) $(OUT)