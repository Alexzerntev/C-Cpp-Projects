#
# In order to execute this "Makefile" just type "make"
#

OBJS = main.o error_handler.o string_handler.o settings.o file_manager.o \
client_list.o helpers.o log_handler.o reader.o writer.o child_list.o crypto.o

SOURCE = main.c error_handler.c string_handler.c settings.c file_manager.c \
client_list.c helpers.c log_handler.c reader.c writer.c child_list.c crypto.h

HEADER = error_handler.h string_handler.h settings.h file_manager.h client_list.h \
helpers.h log_handler.h reader.h writer.h child_list.h crypto.h
OUT	= mirror_client
CC = gcc -g
FLAGS = -c -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@

# create/compile the individual files >>separately<< 
main.o: main.c
	$(CC) $(FLAGS) main.c

error_handler.o: error_handler/error_handler.c
	$(CC) $(FLAGS) error_handler/error_handler.c

string_handler.o: string/string_handler.c
	$(CC) $(FLAGS) string/string_handler.c

settings.o: model/settings/settings.c
	$(CC) $(FLAGS) model/settings/settings.c

file_manager.o: io/file_manager/file_manager.c
	$(CC) $(FLAGS) io/file_manager/file_manager.c

client_list.o: data_structures/client_list/client_list.c
	$(CC) $(FLAGS) data_structures/client_list/client_list.c

child_list.o: data_structures/child_list/child_list.c
	$(CC) $(FLAGS) data_structures/child_list/child_list.c

helpers.o: helpers/helpers.c
	$(CC) $(FLAGS) helpers/helpers.c

log_handler.o: log_handler/log_handler.c
	$(CC) $(FLAGS) log_handler/log_handler.c

reader.o: process/reader/reader.c
	$(CC) $(FLAGS) process/reader/reader.c

writer.o: process/writer/writer.c
	$(CC) $(FLAGS) process/writer/writer.c

crypto.o: crypto/crypto.c
	$(CC) $(FLAGS) crypto/crypto.c


# clean house
clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)