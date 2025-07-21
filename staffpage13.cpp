#include "staffpage13.h"
#include "ui_staffpage13.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QDateTime>
#include <QDebug>
#include <QTableView>

StaffPage13::StaffPage13(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StaffPage13)
{
    ui->setupUi(this);
    QString styleSheet = R"(
            /* 1. 窗口整体样式 */
            QWidget#StaffPage13 {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                            stop:0 #F5F7FA, stop:1 #ECEEF2);
            }

            /* 2. 输入框样式（lineEdit 和 lineEdit_2） */
            QLineEdit#lineEdit, QLineEdit#lineEdit_2 {
                border: 1px solid #CCCCCC;
                border-radius: 6px;
                padding: 6px 10px;
                font-size: 14px;
                color: #333333;
                background-color: #FFFFFF;
            }
            QLineEdit#lineEdit:focus, QLineEdit#lineEdit_2:focus {
                border-color: #66B3FF;
                outline: none;
            }

            /* 3. 按钮样式（打卡和更新按钮） */
            QPushButton#btn_checkIn, QPushButton#refreshTable {
                background-color: #66B3FF;
                color: white;
                border: none;
                border-radius: 6px;
                padding: 8px 16px;
                font-size: 14px;
                cursor: pointer;
            }
            QPushButton#btn_checkIn:hover, QPushButton#refreshTable:hover {
                background-color: #3399FF;
            }
            QPushButton#btn_checkIn:pressed, QPushButton#refreshTable:pressed {
                background-color: #007FFF;
            }

            /* 4. 表格样式 */
            QTableView#tableView {
                background-color: #FFFFFF;
                alternate-background-color: #F9FAFB;
                gridline-color: #E5E7EB;
                border-radius: 8px;
                border: 1px solid #E5E7EB;
                selection-background-color: #DBEAFE;
                selection-color: #1E40AF;
            }
            QTableView#tableView QHeaderView::section {
                background-color: #F3F4F6;
                color: #1F2937;
                font-weight: bold;
                padding: 8px;
                border: none;
                border-bottom: 1px solid #E5E7EB;
            }
            QTableView#tableView QHeaderView::section:first {
                border-top-left-radius: 8px;
            }
            QTableView#tableView QHeaderView::section:last {
                border-top-right-radius: 8px;
            }
        )";

        // 设置当前窗口对象名，让样式表中 #StaffPage13 能精准匹配
        this->setObjectName("StaffPage13");
        // 应用样式表到当前窗口
        this->setStyleSheet(styleSheet);



    //建立并打开数据库
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","3");
            db.setDatabaseName("smart_upc.db");

        if(db.open()){
        // 创建QSqlTableModel并设置数据库和表名
        QSqlTableModel *model = new QSqlTableModel(this,db);
        model->setTable("Check2"); // 替换为实际的表名
        model->select();

        // 设置QTableView的模型
        ui->tableView->setModel(model);
        }else {
            QMessageBox::critical(this,"错误","数据库打开失败！");
        }
        ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        connect(ui->btn_checkIn,&QPushButton::clicked,this,&StaffPage13::on_btn_checkIn_clicked);
}

StaffPage13::~StaffPage13()
{
    delete ui;
}

void StaffPage13::on_btn_checkIn_clicked()
{
    QString Id=ui->lineEdit->text().trimmed();
    if(Id.isEmpty()){
        QMessageBox::warning(this,"提示","请输入Id!");
        return;
    }
    QDateTime currentTime = QDateTime::currentDateTime();
    QString checkTime = currentTime.toString("yyyy-MM-dd hh:mm:ss");
    QString username =ui->lineEdit_2->text();
    QSqlQuery query;

      QString sql = QString("INSERT INTO Check2 (id, 用户名,打卡时间) VALUES ('%1', '%2', '%3')")
                        .arg(Id).arg(username).arg(checkTime);
      if (!query.exec(sql)) {
          //QMessageBox::critical(this, "错误", "打卡失败：" + query.lastError().text());
          qDebug()<<"11111";
          return;
      }

      // 打卡成功提示
      QMessageBox::information(this, "成功", "打卡成功！时间：" + checkTime);
      qDebug()<<"22222";

      // 同步更新表格显示
      on_refreshTable_clicked();
  }




void StaffPage13::on_refreshTable_clicked()
{

    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(ui->tableView->model());
       if (model) {

           model->select();
       }

}
