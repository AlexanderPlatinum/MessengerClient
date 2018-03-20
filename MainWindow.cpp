#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->protocol = new Protocol();
    this->InitializeScenes();

    setWindowTitle( "Messanger Client" );

    connect( ui->SendButton, SIGNAL(clicked()), this, SLOT(SendMessageBtn()) );
}

void MainWindow::InitializeScenes()
{

    this->loginForm = new LoginForm();
    connect( this->loginForm, &LoginForm::isAuthorize, this, &MainWindow::show );

    currentScene = SceneTypes::LoginScene;
    RenderCurrentScene();
    RenderConversationList();
}

void MainWindow::RenderCurrentScene()
{
    this->close();

    switch ( currentScene )
    {
        case SceneTypes::LoginScene:
        {
            this->loginForm->show();
        }
        break;
    }
}

void MainWindow::SendMessageBtn()
{
    QString message;
    message = ui->MessageEdit->text();

    if ( message.size() == 0 )
    {
        return;
    }

    ui->MessageEdit->setText( "" );
}

void MainWindow::RenderConversationList ()
{
    QList<QString> conversations;
    conversations << "Vasiliy Pupkin" << "Ivan Petrov";
    ui->ConversationList->addItems( conversations );
}

MainWindow::~MainWindow()
{
    delete ui;
}
