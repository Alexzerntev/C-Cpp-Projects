#
# In order to execute this "Makefile" just type "make"
#

OBJS = main.o functions/file_managment.o functions/message_managment.o functions/intercom_managment.o functions/shared_managment.o data_structures/list.o
OUT = run
CC = gcc -g
FLAGS = -c -pedantic -ansi  -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@

# create/compile the individual files >>separately<< 
file_managment.o: functions/file_managment.c
	$(CC) $(FLAGS) functions/file_managment.c

message_managment.o: functions/message_managment.c
	$(CC) $(FLAGS) functions/message_managment.c

intercom_managment.o: functions/intercom_managment.c
	$(CC) $(FLAGS) functions/intercom_managment.c

shared_managment.o: functions/shared_managment.c
	$(CC) $(FLAGS) functions/shared_managment.c

list.o: data_structures/list.c
	$(CC) $(FLAGS) data_structures/list.c


# clean house
clean:
	rm -f $(OBJS) $(OUT)
	rm -f files/log/*

clean_without_log:
	rm -f $(OBJS) $(OUT)