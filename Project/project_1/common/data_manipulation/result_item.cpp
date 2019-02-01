#include "./result_item.hpp"

ResultItem::ResultItem()
    : id(0), distance(0.0)
{
}

ResultItem::ResultItem(unsigned long int id, double distance)
    : id(id), distance(distance)
{
}

bool operator<(const ResultItem &r1, const ResultItem &r2)
{
    return r1.distance < r2.distance;
}

bool compareResult(ResultItem *a, ResultItem *b) { return (a->distance < b->distance); }