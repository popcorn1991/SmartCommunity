#include "pthree.h"
#include "ui_pthree.h"
#include <qdebug.h>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QTableView>
#include "data.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QLineEdit>
#include <QString>


PTHREE::PTHREE(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PTHREE)
{
    ui->setupUi(this);
    this->setStyleSheet(R"(
            /* 主窗口样式 */
            QMainWindow {
                background-color: #F5F7FA;
            }



            /* 按钮样式 */
            QPushButton#pushButton_8,
            QPushButton#pushButton_3,
            QPushButton#pushButton_2 {
                background-color: #66B3FF;
                color: white;
                border: none;
                border-radius: 6px;
                padding: 8px 16px;
                font-size: 20px;
            }

            QPushButton#pushButton_8:hover,
            QPushButton#pushButton_3:hover,
            QPushButton#pushButton_2:hover {
                background-color: #3399FF;
            }

            QPushButton#pushButton_8:pressed,
            QPushButton#pushButton_3:pressed,
            QPushButton#pushButton_2:pressed {
                background-color: #007FFF;
            }

            /* 全选复选框 */
            QCheckBox#checkBox {
                font-size: 14px;
                color: #333333;
                spacing: 5px;
            }

            QCheckBox#checkBox::indicator {
                width: 18px;
                height: 18px;
            }

            QCheckBox#checkBox::indicator:checked {
                image: url(:/icons/check.svg); /* 如果有图标资源 */
            }

            /* 表格样式 */
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
        )");
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //建立并打开数据库
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE","5");
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
}

PTHREE::~PTHREE()
{
    delete ui;
}


void PTHREE::on_pushButton_2_clicked()
{
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(ui->tableView->model());
       if (model) {
           QString searchText = ui->lineEdit->text(); // 假设界面上有一个QLineEdit用于输入查询条件
           model->setFilter(QString("用户名 LIKE '%%1%'").arg(searchText)); // 假设按用户名查询，根据实际表结构调整
           model->select();
       }
}

void PTHREE::on_pushButton_3_clicked()
{
    QSqlTableModel *model = qobject_cast<QSqlTableModel*>(ui->tableView->model());
        if (model) {
            QModelIndexList selectedIndexes = ui->tableView->selectionModel()->selectedIndexes();
            if (!selectedIndexes.isEmpty()) {
                QModelIndex index = selectedIndexes.first();
                int row = index.row();
                model->removeRow(row);
                model->submitAll(); // 提交更改到数据库
            }
        }
}
