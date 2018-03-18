#pragma once

#include <QString>

enum SceneTypes
{
    LoginScene
};

enum Requests
{
    GetMessages = 1,
    SendMessage,
    GetUserList,
    AddFriends,
    ConfirmFriendship,
    GetFriends
};

enum Reply
{
    Error = 1,
    MessagesList,
    GetTimeline,
    GetFriendsReply
};

