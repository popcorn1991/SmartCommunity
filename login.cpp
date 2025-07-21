#include "login.h"
#include "ui_login.h"
#include "data.h"
#include "QDebug"
#include "QMessageBox"
#include "QSqlQuery"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QLineEdit>
#include <QString>
#include <QSqlDatabase>
#include <QDialog>
#include <QPushButton>


Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)

{
    ui->setupUi(this);


    if(!db.openDB("smart_upc.db"))
    {
        QMessageBox::critical(this,"错误","数据库打开失败！");


    }
    connect(ui->pushButton_5,&QPushButton::clicked,this,&Login::onLoginClicked);
    connect(ui->pushButton_6,&QPushButton::clicked,this,&Login::close);

    setStyleSheet("background-color:#288bf0;color:black;");
    ui->label_photo_3->setPixmap(QPixmap("qh1.png"));
    ui->pushButton_5->setStyleSheet("background-color: #fcd840; color: white;");
     ui->pushButton_6->setStyleSheet("background-color: #fcd840; color: white;");
    ui->label_photo_3->setScaledContents(true);
    ui->lineEdit_6->setEchoMode(QLineEdit::Password);
    m_usernameEdit = ui->lineEdit_5;
    m_passwordEdit = ui->lineEdit_6;
    m_loginBtn = ui->pushButton_5;
    ui->lineEdit_5->setEchoMode(QLineEdit::Normal);
    ui->lineEdit_6->setEchoMode(QLineEdit::Password);







        //connect(m_loginBtn,&QPushButton::clicked,this,&Login::on_pushButton_clicked);

    //connect(ui->pushButton_5,&QPushButton::clicked,this,&Login::on_pushButton_clicked);

}

Login::~Login()
{
    delete ui;
}


void Login::onLoginClicked()
{
    QString username=ui->lineEdit_5->text().trimmed();
    QString password=ui->lineEdit_6->text().trimmed();
    qDebug()<<username;
    qDebug()<<password;

QSqlQuery query =db.getUserByUsername(username);
    if(username.isEmpty()||password.isEmpty())
    {
        QMessageBox::warning(this,"提示","请输入用户名和密码");

    }
    else
    {


    if(query.next())
    {
        QString dbPassword =query.value("password").toString();
        if(password!=dbPassword)
        {
            QMessageBox::warning(this,"登录失败","密码错误！");
            return;
        }
        int userRole = query.value("role").toInt();
        int userId = query.value("id").toInt();
        int staff_Id=query.value("staff_id").toInt();
        emit loginSuccess(userRole,userId,staff_Id);
        accept();
    }
    else {

       QMessageBox::warning(this,"登录失败","用户不存在，请先注册！");
       return;
    }
    }


}

void Login::on_pushButton_5_clicked()
{
    QString username = m_usernameEdit->text().trimmed();
    QString password = m_passwordEdit->text();


    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this,"提示","请输入用户名和密码！");
        return;
    }


    //查询数据库，使用QSqlQuery数据集
    QSqlQuery query = db.getUserByUsername(username);
    //qDebug()<<"11111111111111111111111111111";
    if(query.next()){
        QString dbPassword = query.value("password").toString();
        //qDebug()<<"222222222222";
        if(dbPassword!=password){
            QMessageBox::warning(this,"登录失败","密码错误！");
            return;
        }
        int userId=query.value("id").toInt();
        int userRole = query.value("role").toInt();
        int staff_Id=query.value("staff_id").toInt();

        emit loginSuccess(userRole,userId,staff_Id);
        //qDebug()<<role<<"20250710";
        accept();
    }else {
       QMessageBox::warning(this,"登录失败","用户不存在！");
       return;
    }

}




//显示密码
void Login::on_checkBox_3_clicked(bool checked)
{
    if(checked)
    {
        ui->lineEdit_6->setEchoMode(QLineEdit::Normal);
    }
    else {
        ui->lineEdit_6->setEchoMode(QLineEdit::Password);
    }
}

void Login::on_pushButton_fh_3_clicked()
{


    setStyleSheet("background-color: rgb(253, 221, 231); color: black;");
                ui->label_photo_3->setPixmap(QPixmap("qh2.png")); // 图片2
   ui->pushButton_5->setStyleSheet("background-color: #fc7c91; color: white;");
    ui->pushButton_6->setStyleSheet("background-color: #fc7c91; color: white;");

}

void Login::on_pushButton_hl_3_clicked()
{
    setStyleSheet("background-color: #288bf0; color:black;"); // 背景蓝，字体黄
    ui->label_photo_3->setPixmap(QPixmap("qh1.png")); // 图片1
      ui->pushButton_5->setStyleSheet("background-color: #fcd840; color: white;");
     ui->pushButton_6->setStyleSheet("background-color: #fcd840; color: white;");
}


void Login::on_pushButton_qz_3_clicked()
{
    setStyleSheet("background-color: rgb(165, 185, 255); color: black;"); // 背景绿，字体红
    ui->label_photo_3->setPixmap(QPixmap("qh4.png")); // 图片4
    ui->pushButton_5->setStyleSheet("background-color: #7f98fd; color: white;");
      ui->pushButton_6->setStyleSheet("background-color: #7f98fd; color:white;");
}

void Login::on_pushButton_ql_3_clicked()
{
    setStyleSheet("background-color: rgb(201, 252, 244); color: black;");
    ui->label_photo_3->setPixmap(QPixmap("qh3.png"));
     ui->pushButton_5->setStyleSheet("background-color: rgb(138, 238, 235); color: white;");
     ui->pushButton_6->setStyleSheet("background-color: rgb(138, 238, 235); color: white;");
}







void Login::on_pushButton_6_clicked()
{
    this->hide();
}
