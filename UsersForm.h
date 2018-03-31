#pragma once

#include <QWidget>
#include <QJsonArray>
#include <QList>
#include <QJsonObject>
#include <map>
#include <QString>

namespace Ui {
class UsersForm;
}

class UsersForm : public QWidget
{
    Q_OBJECT

public:
    explicit UsersForm( QJsonArray data, QWidget *parent = 0 );
    ~UsersForm();

private:
    Ui::UsersForm *ui;
    std::map<int, QString> positionListToUser;

signals:
    void isUserSelect( QString user_id );

private slots:
    void AddUserHandle();

};
