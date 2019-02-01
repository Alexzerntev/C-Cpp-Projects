#ifndef F_FUNCTION_HPP
#define F_FUNCTION_HPP

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "../common/h_function.hpp"
#include "../common/random_generator.hpp"

using namespace std;

template <class T>
class FFunction
{
  public:
    vector<HFunction<T> *> *f;
    FFunction(vector<HFunction<T> *> *hi)
        : f(hi)
    {
    }

    FFunction(const FFunction &ff)
    {
        f = new vector<HFunction<T> *>();
        *f = *ff.f;
    }

    FFunction(FFunction &&ff)
        : f(ff.f)
    {
        ff.f = nullptr;
    }

    ~FFunction()
    {
        HFunction<T> *temp;
        while (f->size() > 0)
        {
            temp = f->at(0);
            f->erase(f->begin());
            delete temp;
        }
        delete f;
    }

    string apply_function(vector<T> *&p)
    {
        string result = "";
        int h_result;
        int random_binary = 0;
        vector<int>::iterator found_zeroes;
        vector<int>::iterator found_ones;
        RandomGenerator random_generator;
        for (unsigned int i = 0; i < f->size(); i++)
        {
            h_result = f->at(i)->apply_function(p);

            found_zeroes = find(zeroes_.begin(), zeroes_.end(), h_result);
            if (found_zeroes != zeroes_.end())
            {
                result = result + "0";
                continue;
            }

            found_ones = find(ones_.begin(), ones_.end(), h_result);
            if (found_ones != ones_.end())
            {
                result = result + "1";
                continue;
            }

            random_binary = random_generator.get_uniform_distribution_binary_random();

            if (random_binary == 0)
            {
                zeroes_.push_back(h_result);
                result = result + "0";
                continue;
            }
            else
            {
                ones_.push_back(h_result);
                result = result + "1";
                continue;
            }
        }
        return result;
    }

    MetricType get_type()
    {
        return type_;
    }

  private:
    MetricType type_;
    vector<int> ones_;
    vector<int> zeroes_;
};

#endif