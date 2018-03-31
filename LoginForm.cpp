#include "LoginForm.h"
#include "ui_LoginForm.h"

LoginForm::LoginForm(QWidget *parent) : QWidget(parent), ui(new Ui::LoginForm)
{
    ui->setupUi(this);

    connect( ui->LoginButton, SIGNAL(clicked(bool)), this, SLOT( HandleLoginButton() ) );
    connect( ui->RegisterButton, SIGNAL( clicked(bool)), this, SLOT( HandleRegisterButton() ) );

    setWindowTitle( "Messenger Client | Вход" );
}

void LoginForm::HandleLoginButton()
{

    QString login;
    QString password;

    login = ui->LoginEdit->text();
    password = ui->PasswordEdit->text();

    if ( login.length() == 0 )
    {
        Utilities::ShowError( "Введите логин!" );
        return;
    }

    if ( password.length() == 0 )
    {
        Utilities::ShowError( "Введите пароль!" );
        return;
    }


    emit this->isAuthorize( login, password );
    this->close();

}

void LoginForm::HandleRegisterButton()
{
    emit this->openRegisterForm();
    this->close();
}

LoginForm::~LoginForm()
{
    delete ui;
}
