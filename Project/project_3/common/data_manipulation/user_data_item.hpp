#ifndef USER_DATA_ITEM_HPP
#define USER_DATA_ITEM_HPP

#include <string>
#include <vector>

using namespace std;

class UserDataItem
{
private:
public:
  UserDataItem();
  UserDataItem(int user_id, int tweet_id);
  UserDataItem(const UserDataItem &user_data_item);
  UserDataItem(UserDataItem &&user_data_item);

  ~UserDataItem();

  vector<string> *tweet_data;
  int user_id;
  int tweet_id;
  double sentiment;
};

#endif