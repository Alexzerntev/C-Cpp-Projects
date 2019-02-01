#include "./user_data_item.hpp"

UserDataItem::UserDataItem(int _user_id, int _tweet_id)
{
    user_id = _user_id;
    tweet_id = _tweet_id;
}

UserDataItem::UserDataItem()
{
}

UserDataItem::~UserDataItem()
{
    delete tweet_data;
}

// Copy Constructor
UserDataItem::UserDataItem(const UserDataItem &user_data_item)
{
    tweet_id = user_data_item.tweet_id;
    tweet_data = new vector<string>();
    *tweet_data = *user_data_item.tweet_data;
}

// Move Constructor
UserDataItem::UserDataItem(UserDataItem &&user_data_item)
    : tweet_data(user_data_item.tweet_data)
{
    user_data_item.tweet_data = nullptr;
}