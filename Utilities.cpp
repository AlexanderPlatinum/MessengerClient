#include "Utilities.h"

void Utilities::ShowError( QString message )
{
    QMessageBox *msgBox = new QMessageBox();
    msgBox->setText( message );
    msgBox->exec();
}
