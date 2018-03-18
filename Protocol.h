#pragma once

#include <QTcpSocket>
#include <map>

class Protocol : QTcpSocket
{

private:

    QTcpSocket *socket;

    int currentMessage  = 0;
    std::map <int, void (*)(QString)> callBacks;

    void SetUpConnection();

public:
    Protocol();

    void SendMessage( QString data, void ( *function )( QString ) );

private slots:
    void ReciveAndExecute ();

};

