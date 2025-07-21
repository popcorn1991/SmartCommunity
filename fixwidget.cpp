#include "fixwidget.h"
#include "ui_fixwidget.h"
#include <QMessageBox>
#include "leaveapprovewidget.h"
#include "ui_leaveapprovewidget.h"

#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlRecord>
#include "data.h"
#include "login.h"

fixWidget::fixWidget(int id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::fixWidget),
    m_id(id)
{
    ui->setupUi(this);


    connect(ui->pushButton ,&QPushButton::clicked, this, &fixWidget::onSubmit);
    QString styleSheet = R"(
            /* 窗口背景：蓝色系渐变 */
            QWidget#fixWidget {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                           stop:0 #e3f2fd, stop:1 #bbdefb);
            }

            /* 标题“报修申请”标签样式（假设其 objectName 为 labelTitle ，需根据实际 ui 调整） */
            QLabel#labelTitle {
                font-size: 30px;
                font-weight: bold;
                color: #1976d2;
                padding: 10px 0;
            }

            /* 输入框（lineEdit）样式 */
            QLineEdit#lineEdit {
                background-color: white;
                border: 1px solid #bbdefb;
                border-radius: 6px;
                padding: 8px 10px;
                font-size: 30px;
                color: #333;
            }
            QLineEdit#lineEdit:hover {
                border-color: #90caf9;
            }
            QLineEdit#lineEdit:focus {
                border-color: #2196f3;
                outline: none;
            }

            /* 文本编辑框（textEdit）样式，若实际命名不同需修改选择器 */
            QTextEdit#textEdit {
                background-color: white;
                border: 1px solid #bbdefb;
                border-radius: 6px;
                padding: 8px 10px;
                font-size: 30px;
                color: #333;
            }
            QTextEdit#textEdit:hover {
                border-color: #90caf9;
            }
            QTextEdit#textEdit:focus {
                border-color: #2196f3;
                outline: none;
            }

            /* 提交按钮样式 */
            QPushButton#pushButton {
                background-color: #2196f3;
                color: white;
                border-radius: 6px;
                border: none;
                padding: 10px 20px;
                font-size: 30px;
                font-weight: 500;
            }
            QPushButton#pushButton:hover {
                background-color: #1976d2;
            }
            QPushButton#pushButton:pressed {
                background-color: #1565c0;
            }
        )";
        // 设置窗口 objectName ，使样式表能匹配
        this->setObjectName("fixWidget");
        this->setStyleSheet(styleSheet);
    ui->lineEdit->setPlaceholderText("请输入报修标题");
    ui->textEdit->setPlaceholderText("请输入报修描述");

}

fixWidget::~fixWidget()
{
    delete ui;
}
void fixWidget::onSubmit()
{


    QString title = ui->lineEdit->text();
    qDebug()<<title;

    QString text = ui->textEdit->toPlainText();
    qDebug()<<text;

   ui->lineEdit->clear();
   ui->textEdit->clear();
   if(title.isEmpty()&&text.isEmpty())
   {
       QMessageBox::warning(this, "提示", "请填写标题与内容！");
       return;
   }
    else if (title.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写标题！");
        return;
    }

    else if (text.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写内容！");
        return;
    }


    Data db;

    db.openDB("smart_upc.db");

    QSqlQuery q;
    q.prepare("INSERT INTO fix ( o_id, title, text, status) VALUES (?, ?, ?, 0)");

    qDebug()<<m_id;
    q.addBindValue(m_id);
    q.addBindValue(title);
    q.addBindValue(text);
    QMessageBox::information(this, "成功", "维修申请已提交，等待处理！");

    q.exec();
}
