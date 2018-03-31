#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->InitializeScenes();
    this->SetUpNetworking();

    this->loginForm->show();

    setWindowTitle( "Messenger Client" );

    connect( ui->SendButton, SIGNAL(clicked()), this, SLOT(SendMessageBtn()));
    connect( ui->ConversationList, &QListWidget::itemClicked, this, &MainWindow::SelectConversation );
    connect( ui->addUserBtn, SIGNAL(clicked(bool)), this, SLOT(OpenUsersListHandle()) );

    this->timer = new QTimer();
    this->timer->setInterval( 512 );

    connect( this->timer, SIGNAL( timeout() ), this, SLOT( LoadMessages() ) );

    this->timer->start();
}

void MainWindow::OpenUsersListHandle()
{

    QJsonObject object;

    object.insert( "token", this->token );
    object.insert( "seqId", QString::number( this->seqId ) );

    this->toRender[this->seqId] = "OPEN_USERS";
    this->seqId++;

    QByteArray data = Utilities::MakeQuery( "GET_USERS", object );
    this->socket->write( data );
}

void MainWindow::OpenUsersListExec( QJsonArray data )
{
    this->usersForm = new UsersForm( data );

    connect( this->usersForm, SIGNAL( isUserSelect(QString)), this, SLOT( CreateConvesation(QString) ) );

    this->usersForm->show();

}

void MainWindow::CreateConvesation( QString user_id )
{

    QJsonObject object;

    object.insert( "token", this->token );
    object.insert( "friend_id", user_id );
    object.insert( "seqId", QString::number( this->seqId ) );

    this->toRender[this->seqId] = "PREPAIR_TO_UPDATE_CONV";
    this->seqId++;

    QByteArray data = Utilities::MakeQuery( "CREATE_CONVERSATION", object );
    this->socket->write( data );
}

void MainWindow::SetUpNetworking()
{
    this->socket = new QTcpSocket();
    this->socket->connectToHost( "localhost", 7777 );

    connect( this->socket, SIGNAL(connected()), this, SLOT(ConnectToHost()) );
}

void MainWindow::ConnectToHost()
{
    connect( this->socket, SIGNAL(readyRead()), this, SLOT(ReadyReadInformation()) );
    connect( this->socket, SIGNAL(disconnected()), this, SLOT(DisconectFromHost()) );
}

void MainWindow::ReadyReadInformation()
{
    while( this->socket->bytesAvailable() > 0 )
    {
        QByteArray buffer = this->socket->readAll();
        this->ExecuteResponse( buffer );
    }
}

void MainWindow::DisconectFromHost()
{
    this->socket->close();
}

void MainWindow::InitializeScenes()
{
    this->loginForm = new LoginForm();
    this->registerForm = new RegisterForm();

    connect( this->loginForm, SIGNAL(isAuthorize(QString, QString)), this, SLOT( ReadyToLogin(QString, QString) ) );
    connect( this->loginForm, SIGNAL(openRegisterForm()) , this, SLOT( OpenRegisterForm() ) );

    connect( this->registerForm, SIGNAL(isRegister(QString,QString,QString,QString)), this, SLOT( ReadyToRegister(QString,QString,QString,QString) ) );
}

void MainWindow::OpenRegisterForm()
{
    this->registerForm->show();
}

void MainWindow::ReadyToRegister(QString first_name, QString last_name, QString email, QString password)
{
    QJsonObject object;

    object.insert( "first_name", first_name );
    object.insert( "last_name", last_name );
    object.insert( "email", email );
    object.insert( "password", password );
    object.insert( "seqId", QString::number( this->seqId ) );

    QByteArray data = Utilities::MakeQuery( "REGISTER_USER", object );

    this->toRender[seqId] = "REGISTER_ACTION";
    seqId++;

    this->socket->write( data );

}

void MainWindow::ReciveToken( QString token, QString user_id, QString first_name, QString last_name )
{
    if ( token.size() == 0 )
    {
        Utilities::ShowError( "You are entered wrong password or email!" );
        this->loginForm->show();
        this->close();
        return;
    }

    this->token = token;

    this->user_id = user_id;
    this->first_name = first_name;
    this->last_name = last_name;

    this->LoadConversations();
}

void MainWindow::LoadConversations()
{
    QJsonObject object;

    object.insert( "token", this->token );
    object.insert( "seqId", QString::number( this->seqId ) );

    QByteArray query = Utilities::MakeQuery( "GET_CONVERSATIONS", object );

    this->toRender[seqId] = "RENDER_CONVERSATION_LIST";
    seqId++;

    this->socket->write( query );
}

void MainWindow::ReadyToLogin( QString email, QString password )
{

    this->show();

    QJsonObject object;

    object.insert( "email", email );
    object.insert( "password", password );
    object.insert( "seqId", QString::number( this->seqId ) );

    QByteArray data = Utilities::MakeQuery( "LOGIN_USER", object );
    this->socket->write( data );

    this->toRender[this->seqId] = "RECIVE_TOKEN";
    this->seqId++;

}

void MainWindow::RenderConversationList( QJsonArray array )
{
    QList<QString> conversations;

    int index = 0;
    ui->ConversationList->clear();
    this->positionListToConv.clear();

    foreach ( const QJsonValue &value, array )
    {
        QJsonObject obj = value.toObject();

        if ( obj["u_one_first_name"].toString() == this->first_name &&
             obj["u_one_last_name"].toString() == this->last_name )
        {
            conversations << obj["u_two_first_name"].toString() + " " + obj["u_two_last_name"].toString();

            this->currentConvFirstName = obj["u_two_first_name"].toString();
            this->currentConvLastName  = obj["u_two_last_name"].toString();
        }
        else
        {
            conversations << obj["u_one_first_name"].toString() + " " + obj["u_one_last_name"].toString();

            this->currentConvFirstName = obj["u_one_first_name"].toString();
            this->currentConvLastName  = obj["u_one_last_name"].toString();
        }

        this->positionListToConv[index] = obj["conversation_id"].toString();
        index++;
    }

    ui->ConversationList->addItems( conversations );
}

void MainWindow::RenderMessagesList( QJsonArray array )
{

    ui->textBrowser->clear();

    foreach ( const QJsonValue &value, array )
    {
        QJsonObject obj = value.toObject();

        QString full_name;

        if ( obj["author_id"].toString() == this->user_id )
        {
            full_name = this->first_name + " " + this->last_name;
        }
        else
        {
            full_name = this->currentConvFirstName + " " + this->currentConvLastName;
        }

        QString htmlText = "<p><b>" + full_name + ": </b><span>" + obj["msg_text"].toString() + "</span></p>";

        ui->textBrowser->append( htmlText );
    }
}

void MainWindow::ExecuteResponse( QByteArray data )
{

    QJsonDocument document = QJsonDocument::fromJson( data );
    QJsonObject obj = document.object();


    int respSeqId = obj["seqId"].toString().toInt();
    QString executionFunction = this->toRender[ respSeqId ];

    if ( executionFunction == "RENDER_CONVERSATION_LIST" )
    {
       this->RenderConversationList( obj["response"].toArray() );
    }
    if ( executionFunction == "RENDER_MESSAGES" )
    {
        this->RenderMessagesList( obj["response"].toArray() );
    }

    if ( executionFunction == "RECIVE_TOKEN" )
    {
        this->ReciveToken( obj["token"].toString(), obj["user_id"].toString(), obj["first_name"].toString(), obj["last_name"].toString() );
    }

    if ( executionFunction == "REGISTER_ACTION" && obj["msg"].toString() == "OK"  )
    {
        Utilities::ShowError( "You are reginster now, please login!" );
        this->registerForm->close();
        this->loginForm->show();
    }

    if ( executionFunction == "OPEN_USERS" )
    {
        this->OpenUsersListExec( obj["response"].toArray() );
    }

    if ( executionFunction == "PREPAIR_TO_UPDATE_CONV" )
    {
        this->LoadConversations();
    }

    if ( obj["msg"].toString() != "OK" )
    {
        return;
    }

}

void MainWindow::SendMessageBtn()
{
    QString message;

    message = ui->MessageEdit->text();
    ui->MessageEdit->setText( "" );

    if ( message.size() == 0 )
    {
        return;
    }

    QJsonObject object;

    object.insert( "token", this->token );
    object.insert( "conversation_id", QString::number( this->currentConversationId ) );
    object.insert( "message", message );
    object.insert( "seqId", QString::number( this->seqId ) );

    QByteArray data = Utilities::MakeQuery( "SEND_MESSAGE", object );

    this->socket->write( data );
    this->seqId++;

    QString htmlText = "<p align=\"right\"><b>" + this->first_name + " " + this->last_name + ": </b><span>" + message + "</span></p>";
    ui->textBrowser->append( htmlText );

}

void MainWindow::SelectConversation()
{
    int index = this->ui->ConversationList->currentRow();
    this->currentConversationId = this->positionListToConv[index].toInt();

    this->QueryMessages();
}

void MainWindow::LoadMessages()
{

    if ( this->currentConversationId != 0)
    {
         this->QueryMessages();
    }

}

void MainWindow::QueryMessages()
{

    QJsonObject object;

    object.insert( "token", this->token );
    object.insert( "conversation_id", QString::number( this->currentConversationId ) );
    object.insert( "seqId", QString::number( this->seqId ) );

    this->toRender[this->seqId] = "RENDER_MESSAGES";
    this->seqId++;

    QByteArray data = Utilities::MakeQuery( "GET_MESSAGES", object );

    this->socket->write( data );
}

MainWindow::~MainWindow()
{
    delete ui;
}
