#pragma once

#include <QMainWindow>

#include "Types.h"
#include "LoginForm.h"
#include "Protocol.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;

    SceneTypes currentScene;
    Protocol *protocol;

    LoginForm *loginForm;

    void InitializeScenes();
    void RenderCurrentScene();

public:

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();


};

