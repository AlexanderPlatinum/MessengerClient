#include "Protocol.h"

Protocol::Protocol()
{
    this->socket = new QTcpSocket();
    SetUpConnection();
}

void Protocol::SetUpConnection()
{
    connect( this->socket, SIGNAL(readyRead()), this, SLOT(ReciveAndExecute()) );

    this->socket->connectToHost( "localhost", 7777 );
}

void Protocol::ReciveAndExecute()
{
    int executeFunction = 0;
    QString data = "";

    switch (1)
    {
        default:
        {
            this->callBacks[executeFunction](data);
        }
        break;
    }
}

void Protocol::SendMessage( QString data, void ( *function )( QString ) )
{
    this->callBacks[this->currentMessage] = function;
    this->currentMessage++;
}
