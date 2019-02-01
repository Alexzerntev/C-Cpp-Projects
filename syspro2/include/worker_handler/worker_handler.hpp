#ifndef WORKER_HAANDLER_HPP
#define WORKER_HAANDLER_HPP

#define BUFSIZE 500
#define MSGSIZE 500
#define NAMED_PIPE_FILE_NAME "/tmp/myfifo"
#define READ 0
#define WRITE 1

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctime>
#include "../string/map_list_node.hpp"
#include "../user_interaction.hpp"
#include "../string/order_string.hpp"
#include "../string/result_list.hpp"

class WorkerHandler
{
private:
  int num_workers;
  char *file_path;
  char *pipe_message_buffer;
  char *fifo_message_buffer;
  int **worker_pipes;
  int lines_count;
  int fifo_file_desc;
  int worker_self_file_desc;
  ResultList **auditor;
  MapListNode *self_maps;
  UserInteraction *user_interaction;

public:
  WorkerHandler(int num_workers, char *file_path);
  ~WorkerHandler();
  void initialize(UserInteraction *user_interaction);
  void find_lines_count();
  void distribute_file_paths();
  int order_action(char *order_string);
  void do_action();
  void send_result();
  void receive_result(int deadline);
  void initialize_worker(int i);
  static void alarm_handler(int signo);
  static void signal_handler(int signo);
};

char *get_enum_string(orderType type);

#endif