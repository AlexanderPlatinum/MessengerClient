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

    connect( this->loginForm, &LoginForm::isAuthorize, this, &MainWindow::ReadyToLogin );
}

void MainWindow::ReadyToLogin()
{
    this->show();
    this->socket->write( QString("{ \"command\": \"GET_CONVERSATIONS\", \"params\" : { \"token\" : \"QRJATYDIMOFUGCLQURCLQECDRHVQSUWBVNTMRYKTDLWIX\", \"seqId\" : \"" + QString::number( this->seqId ) +  "\" } }" ).toUtf8() );
    this->toRender[seqId] = "RENDER_CONVERSATION_LIST";
    seqId++;
}

void MainWindow::RenderConversationList( QJsonArray array )
{
    QList<QString> conversations;

    int index = 0;
    this->positionListToConv.clear();

    foreach ( const QJsonValue &value, array )
    {
        QJsonObject obj = value.toObject();

        conversations << obj["u_two_first_name"].toString() + obj["u_two_last_name"].toString();
        this->positionListToConv[index] = obj["conversation_id"].toString();
        index++;
    }

    ui->ConversationList->addItems( conversations );
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

void MainWindow::SelectConversation()
{
    int index = this->ui->ConversationList->currentRow();
    Utilities::ShowError( this->positionListToConv[index] );
}

MainWindow::~MainWindow()
{
    delete ui;
}
