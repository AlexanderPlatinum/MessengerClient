#pragma once

#include <QMainWindow>

#include "SceneTypes.h"
#include "LoginForm.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    Ui::MainWindow *ui;

    SceneTypes currentScene;

    void InitializeScenes();
    void RenderCurrentScene();
};

