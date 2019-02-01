#ifndef RESULT_LIST_HPP
#define RESULT_LIST_HPP
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "./result_string.hpp"
#include "./path_list_node.hpp"

class ResultList
{
  private:
    int pid;

  public:
    ResultString *next_result;
    PathListNode *next_path;
    bool answered;
    bool alarm_answer_snapshot;
    orderType result_type;
    ResultList();
    ~ResultList();
    void clean();
    void add_path(char *str);
    void add_result(char *str, orderType type);
    void set_pid(int pid);
    int get_pid();
    void print_results();
};

#endif