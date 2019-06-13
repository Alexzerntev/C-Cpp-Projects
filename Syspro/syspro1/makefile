#
# In order to execute this "Makefile" just type "make"
#

OBJS = main.o bitcoin_tree.o hashtable.o transaction_list.o error_handler.o \
io_manager.o user_interaction.o user.o settings.o string_handler.o date_time.o

SOURCE = main.c bitcoin_tree.c hashtable.c transaction_list.c error_handler.c \
io_manager.c user_interaction.c user.c settings.c string_handler.c date_time.o

HEADER = bitcoin_tree.h hashtable.h transaction_list.h error_handler.h io_manager.h \
user_interaction.h user.h settings.h string_handler.h date_time.o

OUT	= bitcoin
CC = gcc -g
FLAGS = -c -Wall
# -g option enables debugging mode 
# -c flag generates object code for separate files

$(OUT): $(OBJS)
	$(CC) $(OBJS) -o $@

# create/compile the individual files >>separately<< 
main.o: main.c
	$(CC) $(FLAGS) main.c

bitcoin_tree.o: data_structures/bitcoin_tree/bitcoin_tree.c
	$(CC) $(FLAGS) data_structures/bitcoin_tree/bitcoin_tree.c

hashtable.o: data_structures/hashtable/hashtable.c
	$(CC) $(FLAGS) data_structures/hashtable/hashtable.c

transaction_list.o: data_structures/transaction_list/transaction_list.c
	$(CC) $(FLAGS) data_structures/transaction_list/transaction_list.c

error_handler.o: error_handler/error_handler.c
	$(CC) $(FLAGS) error_handler/error_handler.c

io_manager.o: io/io_manager.c
	$(CC) $(FLAGS) io/io_manager.c

user_interaction.o: io/user_interaction.c
	$(CC) $(FLAGS) io/user_interaction.c

user.o: model/user/user.c
	$(CC) $(FLAGS) model/user/user.c

settings.o: model/settings/settings.c
	$(CC) $(FLAGS) model/settings/settings.c

string_handler.o: string/string_handler.c
	$(CC) $(FLAGS) string/string_handler.c

date_time.o: date_time/date_time.c
	$(CC) $(FLAGS) date_time/date_time.c

# clean house
clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)