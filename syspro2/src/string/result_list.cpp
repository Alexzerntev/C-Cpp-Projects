#include "../../include/string/result_list.hpp"

ResultList::ResultList()
{
    this->answered = false;
    this->alarm_answer_snapshot = true;
    this->next_path = NULL;
    this->next_result = NULL;
    this->pid = pid;
}

ResultList::~ResultList()
{
}

void ResultList::clean()
{
    this->answered = false;
    this->alarm_answer_snapshot = false;
    ResultString *temp = this->next_result;
    if (temp != NULL)
    {
        delete (temp);
    }
    this->next_result = NULL;
}

void ResultList::add_path(char *str)
{
    this->next_path = new PathListNode(str, this->next_path);
    if (this->next_path == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
}

void ResultList::add_result(char *str, orderType result_type)
{
    this->result_type = result_type;
    this->next_result = new ResultString(str, this->next_result, result_type);
    if (this->next_result == NULL)
    {
        printf("errno = % d \ n ", errno);
        perror("malloc");
    }
}

void ResultList::set_pid(int pid)
{
    this->pid = pid;
}

int ResultList::get_pid()
{
    return this->pid;
}

void ResultList::print_results()
{
    ResultString *temp = this->next_result;
    while (temp != NULL)
    {
        printf("\n The result is : %s \n", temp->string);
        temp = temp->next;
    }
}
