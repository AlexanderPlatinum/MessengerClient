#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->protocol = new Protocol();
    this->InitializeScenes();

    setWindowTitle( "Messanger Client" );
}

void MainWindow::InitializeScenes()
{

    this->loginForm = new LoginForm();
    connect( this->loginForm, &LoginForm::isAuthorize, this, &MainWindow::show );

    currentScene = SceneTypes::LoginScene;
    RenderCurrentScene();
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

MainWindow::~MainWindow()
{
    delete ui;
}
