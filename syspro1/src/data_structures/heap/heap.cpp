#include "../../../include/data_structures/heap/heap.hpp"

Heap::Heap(int size, CommandString *command_string)
{
    this->size = size + 1;
    this->heap_table = new SearchResult *[this->size];
    this->command_string = command_string;
    for (int i = 0; i < this->size; i++)
    {
        this->heap_table[i] = NULL;
    }
    this->current_size = 0;
}
Heap::~Heap()
{
}

void Heap::add(SearchResult *search_result)
{

    if (current_size == 0)
    {
        this->heap_table[0] = search_result;
        this->current_size++;
        return;
    }

    this->current_size++;
    int k = this->current_size;
    this->heap_table[k - 1] = search_result;

    while (this->heap_table[k / 2 - 1]->get_score() > this->heap_table[k - 1]->get_score())
    {
        this->swap(&this->heap_table[k / 2 - 1], &this->heap_table[k - 1]);
        k = k / 2 + 1;
    }

    if (this->current_size == this->size)
    {
        SearchResult *item_to_delete = this->heap_table[0];
        this->heap_table[0] = this->heap_table[this->current_size - 1];
        this->current_size--;
        int i = 0;
        int l, r, min;
        do
        {
            l = 2 * i;
            r = 2 * i + 1;

            if (l <= this->current_size && this->heap_table[l]->get_score() < this->heap_table[i]->get_score())
            {
                min = l;
            }
            else
            {
                min = i;
            }

            if (r < this->current_size && this->heap_table[r]->get_score() < this->heap_table[min]->get_score())
            {
                min = r;
            }

            if (i != min)
            {
                this->swap(&this->heap_table[i], &this->heap_table[min]);
                i = min;
            }
            else
            {
                break;
            }

        } while (i < this->current_size);
        delete (item_to_delete);
    }
}

SearchResult *Heap::get_max_after_reheap()
{
    SearchResult *item_to_return = this->heap_table[0];
    this->heap_table[0] = this->heap_table[this->current_size - 1];
    this->current_size--;
    int i = 0;
    int l, r, max;
    do
    {
        l = 2 * i;
        r = 2 * i + 1;

        if (l <= this->current_size && this->heap_table[l]->get_score() > this->heap_table[i]->get_score())
        {
            max = l;
        }
        else
        {
            max = i;
        }

        if (r <= this->current_size && this->heap_table[r]->get_score() > this->heap_table[max]->get_score())
        {
            max = r;
        }

        if (i != max)
        {
            this->swap(&this->heap_table[i], &this->heap_table[max]);
            i = max;
        }
        else
        {
            break;
        }

    } while (i < this->current_size);
    return item_to_return;
}

void Heap::max_heapify()
{
    for (int i = (this->current_size / 2); i >= 1; i--)
    {
        reheap(i - 1);
    }
}

void Heap::reheap(int i)
{
    int l = 2 * i;
    int r = 2 * i + 1;
    int max;
    SearchResult *sr1 = this->heap_table[l];
    SearchResult *sr2 = this->heap_table[r];
    if (this->heap_table[l] == NULL || this->heap_table[r] == NULL)
    {
        return;
    }
    if (l <= this->current_size && this->heap_table[l]->get_score() > this->heap_table[i]->get_score())
    {
        max = l;
    }
    else
    {
        max = i;
    }
    if (r <= this->current_size && this->heap_table[r]->get_score() > this->heap_table[max]->get_score())
    {
        max = r;
    }
    if (i != max)
    {
        this->swap(&this->heap_table[i], &this->heap_table[max]);
        this->reheap(max);
    }
}

void Heap::swap(SearchResult **a, SearchResult **b)
{
    SearchResult *temp = *a;
    *a = *b;
    *b = temp;
    return;
}

void Heap::print_results()
{
    SearchResult *result_to_print;
    this->max_heapify();
    while (this->current_size != 0)
    {
        result_to_print = this->get_max_after_reheap();
        result_to_print->print(this->command_string);
        //delete (result_to_print);
    }
}

bool Heap::id_exists(int id)
{
    for (int i = 0; i < this->current_size; i++)
    {
        if (this->heap_table[i]->get_id() == id)
        {
            return true;
        }
    }
    return false;
}