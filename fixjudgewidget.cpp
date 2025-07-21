#include "fixjudgewidget.h"
#include "ui_fixjudgewidget.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QAbstractItemView>
#include <QDebug>
#include <QTableWidget>
#include <QMessageBox>
#include <QDateTime>
#include <QHBoxLayout>
#include <QComboBox>
#include "data.h"

fixJudgeWidget::fixJudgeWidget(int staff_id,QWidget *parent) :

    QWidget(parent),

    ui(new Ui::fixJudgeWidget),
     ms_id(staff_id)
{
    ui->setupUi(this);


    table = new QTableWidget(this);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels(QStringList() <<"编号"<<"提交人ID" <<"标题" << "内容" << "状态" << "审批人ID" << "审批时间");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    btnApprove = new QPushButton("已开始", this);

    btnReject = new QPushButton("已完毕", this);
    auto *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(btnApprove);

    btnLayout->addWidget(btnReject);
    btnLayout->addStretch();

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(table);
    mainLayout->addLayout(btnLayout);

    connect(btnApprove, &QPushButton::clicked, this, &fixJudgeWidget::onApprove);
    connect(btnReject, &QPushButton::clicked, this, &fixJudgeWidget::onReject);

    comboBox = new QComboBox(this);
    Data db;
    db.openDB("smart_upc.db");
     QSqlQuery query=db.query("SELECT name FROM test WHERE role = 1 ");
     query.exec();
     while(query.next())
     {
         QString val =query.value(0).toString();
         comboBox->addItem(val);

     }
     QString styleSheet = R"(
            /* 窗口背景 */
            QWidget#fixJudgeWidget {
                background-color: #f5f7fa;
             font-size: 30px;
            }

            /* 表格整体样式 */
            QTableWidget {
                background-color: white;
                border-radius: 8px;
                border: none;
                gridline-color: #e0e0e0;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 30px;
                color: #333;
            }

            /* 表头样式 */
            QHeaderView::section {
                background-color: #f0f2f5;
                padding: 8px;
                border: none;
                border-bottom: 1px solid #e0e0e0;
                font-weight: bold;
                font-size: 30px;
                color: #555;
            }

            /* 表格单元格样式 */
            QTableWidget::item {
                padding: 8px;
                border: none;
            }

            /* 选中行样式 */
            QTableWidget::item:selected {
                background-color: #e0efff;
                color: #0056b3;
            }

            /* 按钮样式 */
            QPushButton {
                background-color: #66b2ff;
                color: white;
                border-radius: 4px;
                padding: 8px 16px;
                font-weight: bold;
                border: none;
                margin: 0 5px;
            }
            QPushButton:hover {
                background-color: #4da6ff;
            }
            QPushButton:pressed {
                background-color: #3990ff;
            }


            }
        )";
        // 设置 objectName，让样式表能匹配到当前窗口
        this->setObjectName("fixJudgeWidget");
        this->setStyleSheet(styleSheet);

     table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
     table->horizontalHeader()->setStretchLastSection(true);
     table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

     QFontMetrics fm(comboBox->font());
     int maxWidth = 0;
     for (int i = 0; i < comboBox->count(); ++i) {
         maxWidth = qMax(maxWidth, fm.horizontalAdvance(comboBox->itemText(i)));
     }
     // 设置宽度（+40 为下拉按钮留出空间）
     comboBox->setMinimumWidth(maxWidth + 40);
    refreshTable();
}




fixJudgeWidget::~fixJudgeWidget()
{
    delete ui;
}
void fixJudgeWidget::refreshTable()
{
    table->setRowCount(0);
    Data db;
    db.openDB("smart_upc.db");
    QSqlQuery query=db.query("SELECT id,o_id,title,text,status,staff_id,date FROM fix ORDER BY status,id DESC");
    query.exec();
    int row =0;
    while(query.next())
    {
        qDebug()<<"sdsd";
        table->insertRow(row);
        for(int col=0;col<7;++col)
        {
            QString val =query.value(col).toString();
            if(col==4)
            {
                int status = query.value(col).toInt();
                if(status==0) val ="未处理";
                else if(status == 1) val="正在处理";
                else if(status == 2) val="处理完毕";

            }
            table->setItem(row,col,new QTableWidgetItem(val));
        }
        ++row;
    }

}
void fixJudgeWidget::onApprove()
{
    int row =table->currentRow();
    if(row< 0)return ;

    int status =table->item(row,4)->text().toInt();
    int leave_id=table->item(row,0)->text().toInt();
    if(status==2)
    {
        QMessageBox::information(this,"提示","该维修请求已结束！");
        return;
    }
    int approver_id =ms_id;

    QString approve_time =QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    Data db;
    db.openDB("smart_upc.db");
    QSqlQuery q;
    q.prepare("UPDATE fix SET status=1,staff_id=?,date=? WHERE id=?");
    q.addBindValue(approver_id);
    q.addBindValue(approve_time);
    q.addBindValue(leave_id);
    q.exec();
    if(q.exec())
    {
        QMessageBox::information(this,"成功","已更改维修状态！");
             refreshTable();

    }
    else {
        {
            QMessageBox::information(this,"失败","操作失败!");
        }
    }
}
void fixJudgeWidget::onReject()
{
    int row =table->currentRow();
    if(row< 0)return ;
    QString approve_time =QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    qDebug()<<approve_time;

    int status =table->item(row,4)->text().toInt();
    qDebug()<<status;
    int leave_id=table->item(row,0)->text().toInt();
    if(status!=0)
    {
        QMessageBox::information(this,"提示","该维修请求已结束！");
        return;
    }
    int approver_id =ms_id;


    Data db;
    db.openDB("smart_upc.db");
    QSqlQuery q;
    q.prepare("UPDATE fix SET status=2,staff_id=?,date=? WHERE id=?");
    q.addBindValue(approver_id);
    q.addBindValue(approve_time);
    q.addBindValue(leave_id);
    q.exec();
    if(q.exec())
    {
        QMessageBox::information(this,"成功","已更改维修状态！");
             refreshTable();

    }
    else {
        {
            QMessageBox::information(this,"失败","操作失败!");
        }
    }
}
