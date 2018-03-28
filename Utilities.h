#pragma once

#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>

class Utilities
{
public:
    static void ShowError ( QString message );
    static QByteArray MakeQuery( QString command, QJsonObject params );
};
