#pragma once

#include <QWidget>

namespace Ui {
class RegisterForm;
}

class RegisterForm : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterForm(QWidget *parent = 0);
    ~RegisterForm();

private:
    Ui::RegisterForm *ui;

signals:
    void isRegister( QString first_name, QString last_name, QString email, QString password );

private slots:
    void registerBtn();

};
