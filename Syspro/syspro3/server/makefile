#
# In order to execute this "Makefile" just type "make"
#

OBJS = main.o error_handler.o string_handler.o server_settings.o server.o\
cyc_buffer.o con_list.o file_manage.o

SOURCE = main.c error_handler.c string_handler.c server_settings.c server.c\
cyc_buffer.c con_list.c file_manage.c

HEADER = error_handler.h string_handler.h server_settings.h server.h\
cyc_buffer.h con_list.h file_manage.h

OUT	= dropbox_server
CC = gcc -g
FLAGS = -c -Wall 
LIBS = -lssl -lcrypto
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

con_list.o: ../common/data_structures/con_list/con_list.c
	$(CC) $(FLAGS) ../common/data_structures/con_list/con_list.c

cyc_buffer.o: ../common/data_structures/cyc_buffer/cyc_buffer.c
	$(CC) $(FLAGS) ../common/data_structures/cyc_buffer/cyc_buffer.c

file_manage.o: ../common/file_manage/file_manage.c
	$(CC) $(FLAGS) ../common/file_manage/file_manage.c

server_settings.o: server_settings/server_settings.c
	$(CC) $(FLAGS) server_settings/server_settings.c

server.o: server/server.c
	$(CC) $(FLAGS) server/server.c



# clean house
clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)