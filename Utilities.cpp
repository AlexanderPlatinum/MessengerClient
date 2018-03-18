#include "Utilities.h"

void Utilities::ShowError( QString message )
{
    QMessageBox *msgBox = new QMessageBox();

    msgBox->setWindowTitle( "Произошла ошибка!" );
    msgBox->setText( message );

    msgBox->exec();
}
