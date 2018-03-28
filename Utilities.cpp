#include "Utilities.h"

void Utilities::ShowError( QString message )
{
    QMessageBox *msgBox = new QMessageBox();

    msgBox->setWindowTitle( "Произошла ошибка!" );
    msgBox->setText( message );

    msgBox->exec();
}

QByteArray Utilities::MakeQuery( QString command, QJsonObject params )
{
    QJsonObject *object = new QJsonObject();

    object->insert( "command", command );
    object->insert( "params", QJsonValue( params ) );

    QJsonDocument *document = new QJsonDocument( *object );
    return document->toJson();
}
