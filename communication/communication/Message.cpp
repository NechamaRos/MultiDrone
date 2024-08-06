#include "Message.h"

vector<vector<vector<vector<uint8_t>>>> Message::getReminder(size_t, size_t, size_t, vector<uint8_t>&)
{
    return vector<vector<vector<vector<uint8_t>>>>();
}

vector<vector<vector<uint8_t>>> Message::getReminder(size_t, size_t, vector<uint8_t>&)
{
    return vector<vector<vector<uint8_t>>>();
}

vector<uint8_t> Message::getReminder(size_t, vector<uint8_t>&)
{
    return vector<uint8_t>();
}


Message::Message(const MessageType& type)
    : type(type)
{
}
