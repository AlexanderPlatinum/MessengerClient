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

private slots:
    void HandleLoginButton ();

};

