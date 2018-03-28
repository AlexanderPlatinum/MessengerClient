#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->InitializeScenes();
    this->SetUpNetworking();

    this->loginForm->show();

    setWindowTitle( "Messanger Client" );

    connect( ui->SendButton, SIGNAL(clicked()), this, SLOT(SendMessageBtn()));
    connect( ui->ConversationList, &QListWidget::itemClicked, this, &MainWindow::SelectConversation );
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

    connect( this->loginForm, SIGNAL(isAuthorize(QString, QString)), this, SLOT( ReadyToLogin(QString, QString) ) );
}

void MainWindow::ReciveToken( QString token )
{
    if ( token.size() == 0 )
    {
        Utilities::ShowError( "You are entered wrong password or email!" );
        this->loginForm->show();
        this->close();
        return;
    }

    this->token = token;

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
    this->positionListToConv.clear();

    foreach ( const QJsonValue &value, array )
    {
        QJsonObject obj = value.toObject();

        conversations << obj["u_two_first_name"].toString() + " " + obj["u_two_last_name"].toString();
        this->positionListToConv[index] = obj["conversation_id"].toString();
        index++;
    }

    ui->ConversationList->addItems( conversations );
}

void MainWindow::RenderMessagesList( QJsonArray array )
{
    foreach ( const QJsonValue &value, array )
    {
        QJsonObject obj = value.toObject();

        QString htmlText = "<p><b>" + obj["author_id"].toString() + ": </b><span>" + obj["msg_text"].toString() + "</span></p>";

        ui->textBrowser->append( htmlText );
    }
}

void MainWindow::ExecuteResponse( QByteArray data )
{

    QJsonDocument document = QJsonDocument::fromJson( data );
    QJsonObject obj = document.object();

    if ( obj["msg"].toString() != "OK" )
    {
        return;
    }

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
        this->ReciveToken( obj["token"].toString() );
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

    QString htmlText = "<p align=\"right\"><b>My: </b><span>" + message + "</span></p>";
    ui->textBrowser->append( htmlText );

}

void MainWindow::SelectConversation()
{
    int index = this->ui->ConversationList->currentRow();
    this->currentConversationId = this->positionListToConv[index].toInt();

    QJsonObject object;

    object.insert( "token", this->token );
    object.insert( "conversation_id", QString::number( this->currentConversationId ) );
    object.insert( "seqId", QString::number( this->seqId ) );

    this->toRender[this->seqId] = "RENDER_MESSAGES";
    this->seqId++;

    QByteArray data = Utilities::MakeQuery( "GET_MESSAGES", object );

    this->socket->write( data );

    ui->textBrowser->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}
