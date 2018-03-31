#include "RegisterForm.h"
#include "ui_RegisterForm.h"

RegisterForm::RegisterForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegisterForm)
{
    ui->setupUi(this);

    setWindowTitle( "Messenger Client | Registration" );

    connect( ui->registerBtn, SIGNAL( clicked(bool) ), this, SLOT( registerBtn() ) );
}

void RegisterForm::registerBtn()
{
    QString first_name = ui->firstNameEdit->text();
    QString last_name  = ui->lastNameEdit->text();
    QString email      = ui->emailEdit->text();
    QString password   = ui->PasswordEdit->text();

    if ( first_name.size() == 0 || last_name.size() == 0 )
    {
        return;
    }

    if ( email.size() < 5 )
    {
        return;
    }

    if ( password.size()  < 6 )
    {
        return;
    }

    emit this->isRegister( first_name, last_name, email, password );
}

RegisterForm::~RegisterForm()
{
    delete ui;
}
