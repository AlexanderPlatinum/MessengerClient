#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->InitializeScenes();
}

void MainWindow::InitializeScenes()
{
    currentScene = SceneTypes::LoginScene;
    RenderCurrentScene();
}

void MainWindow::RenderCurrentScene()
{
    QWidget *widget;

    switch ( currentScene )
    {
        case SceneTypes::LoginScene:
        {
            widget = new LoginForm();
        }
        break;
    }

    this->setCentralWidget( widget );
}

MainWindow::~MainWindow()
{
    delete ui;
}
