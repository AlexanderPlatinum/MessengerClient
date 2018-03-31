#include "UsersForm.h"
#include "ui_UsersForm.h"

UsersForm::UsersForm( QJsonArray data, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsersForm)
{
    ui->setupUi(this);

    setWindowTitle( "Messenger Client | Users list" );

    QList<QString> users;
    int count = 0;

    foreach ( const QJsonValue &value, data )
    {

        QJsonObject obj = value.toObject();

        users << obj["first_name"].toString() + " " + obj["last_name"].toString();

        this->positionListToUser[count] = obj["id"].toString();
        count++;
    }

    ui->listWidget->addItems( users );

    connect( ui->AddButton, SIGNAL( clicked(bool) ), this, SLOT( AddUserHandle() ) );
}

void UsersForm::AddUserHandle()
{

    int index = this->ui->listWidget->currentRow();

    emit this->isUserSelect( this->positionListToUser[index] );
    this->close();
}

UsersForm::~UsersForm()
{
    delete ui;
}
