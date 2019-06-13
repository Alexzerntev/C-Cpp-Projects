#
# In order to execute this "Makefile" just type "make"
#

OBJS = main.o error_handler.o string_handler.o client_settings.o client.o\
cyc_buffer.o file_manage.o con_list.o

SOURCE = main.c error_handler.c string_handler.c client_settings.c client.c\
cyc_buffer.c file_manage.c con_list.o

HEADER = error_handler.h string_handler.h client_settings.h client.h\
cyc_buffer.h file_manage.h con_list.o

OUT	= dropbox_client
CC = gcc -g
FLAGS = -c -Wall
LIBS = -lssl -lcrypto -lpthread
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $@

# create/compile the individual files >>separately<< 
main.o: main.c
	$(CC) $(FLAGS) main.c

error_handler.o: ../common/error_handler/error_handler.c
	$(CC) $(FLAGS) ../common/error_handler/error_handler.c

string_handler.o: ../common/string/string_handler.c
	$(CC) $(FLAGS) ../common/string/string_handler.c

cyc_buffer.o: ../common/data_structures/cyc_buffer/cyc_buffer.c
	$(CC) $(FLAGS) ../common/data_structures/cyc_buffer/cyc_buffer.c

con_list.o: ../common/data_structures/con_list/con_list.c
	$(CC) $(FLAGS) ../common/data_structures/con_list/con_list.c

file_manage.o: ../common/file_manage/file_manage.c
	$(CC) $(FLAGS) ../common/file_manage/file_manage.c

client_settings.o: client_settings/client_settings.c
	$(CC) $(FLAGS) client_settings/client_settings.c

client.o: client/client.c
	$(CC) $(FLAGS) client/client.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)