#pragma once

#include <QWidget>

#include "Utilities.h"

namespace Ui
{
    class LoginForm;
}

class LoginForm : public QWidget
{
    Q_OBJECT

public:
    explicit LoginForm(QWidget *parent = 0);
    ~LoginForm();

private:
    Ui::LoginForm *ui;

signals:
    void isAuthorize( QString email, QString password );

private slots:
    void HandleLoginButton ();

};

