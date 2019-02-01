#
# In order to execute this "Makefile" just type "make"
#

OBJS = main_mpi.o ../common/constants.o ../common/common.o ../common/mpi_computations.o
OUT = run
CC = mpicc -g
FLAGS = -c -pedantic -ansi  -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@

# create/compile the individual files >>separately<< 
constants.o: ../common/constants.c
	$(CC) $(FLAGS) ../common/constants.c

common.o: ../common/common.c
	$(CC) $(FLAGS) ../common/common.c

computations.o: ../common/mpi_computations.c
	$(CC) $(FLAGS) ../common/mpi_computations.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)