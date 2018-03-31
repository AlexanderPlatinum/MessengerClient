#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QTime>
#include <map>

#include "Types.h"
#include "LoginForm.h"
#include "RegisterForm.h"
#include "UsersForm.h"

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
    QString user_id;
    QString first_name;
    QString last_name;

    int currentConversationId = 0;
    QString currentConvFirstName;
    QString currentConvLastName;

    Ui::MainWindow *ui;

    LoginForm *loginForm;
    RegisterForm *registerForm;
    UsersForm *usersForm;

    QTcpSocket *socket;
    QTimer *timer;

    void InitializeScenes();
    void SetUpNetworking();
    void RenderCurrentScene();

    void ExecuteResponse( QByteArray buffer );

    void RenderConversationList( QJsonArray array );
    void RenderMessagesList( QJsonArray array );

    void ReciveToken( QString token, QString user_id, QString first_name, QString last_name );

    void QueryMessages();

private slots:

     void ConnectToHost();
     void DisconectFromHost();
     void ReadyReadInformation();

     void ReadyToLogin( QString email, QString password );
     void ReadyToRegister( QString first_name, QString last_name, QString email, QString password );
     void SelectConversation();
     void LoadMessages();
     void SendMessageBtn ();
     void OpenRegisterForm();
     void OpenUsersListHandle();
     void OpenUsersListExec( QJsonArray data );
     void CreateConvesation( QString user_id );
     void LoadConversations();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

};

