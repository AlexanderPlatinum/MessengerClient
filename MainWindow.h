#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <map>

#include "Types.h"
#include "LoginForm.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    int seqId = 1;
    std::map<int, QString> toRender;
    std::map<int, QString> positionListToConv;

    QString token;
    int currentConversationId = 0;

    Ui::MainWindow *ui;
    LoginForm *loginForm;
    QTcpSocket *socket;

    void InitializeScenes();
    void SetUpNetworking();
    void RenderCurrentScene();

    void ExecuteResponse( QByteArray buffer );

    void RenderConversationList( QJsonArray array );
    void RenderMessagesList( QJsonArray array );

    void ReciveToken( QString token );

private slots:
     void SendMessageBtn ();

     void ConnectToHost();
     void DisconectFromHost();
     void ReadyReadInformation();

     void ReadyToLogin( QString email, QString password );

     void SelectConversation();

public:

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();


};

