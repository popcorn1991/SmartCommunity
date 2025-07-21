#pragma once
#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <Data.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class Login;
}
QT_END_NAMESPACE
class Login : public QDialog
{
    Q_OBJECT


public:
    Data db;
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    signals:
    void loginSuccess(int userRole,int userId,int staff_Id);

private slots:



    void onLoginClicked();
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_checkBox_3_clicked(bool checked);

    void on_pushButton_hl_3_clicked();

    void on_pushButton_qz_3_clicked();

    void on_pushButton_ql_3_clicked();

    void on_pushButton_fh_3_clicked();

private:
    Ui::Login *ui;
    int styleIndex;
    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginBtn;
    QPushButton *m_registerBtn;
};

#endif // LOGIN_H





