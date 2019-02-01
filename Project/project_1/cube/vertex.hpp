#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <string>
#include <iostream>
#include <vector>
#include "../common/data_manipulation/data_item.hpp"

using namespace std;

template <class T>
class Vertex
{
  public:
    vector<DataItem<T> *> *bucket;
    string id;
    Vertex(string id)
        : bucket(new vector<DataItem<T> *>()), id(id)
    {
    }

    ~Vertex()
    {
        delete bucket;
    }
};

#endif