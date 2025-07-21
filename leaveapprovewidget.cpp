#include "leaveapprovewidget.h"
#include "ui_leaveapprovewidget.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>

#include "data.h"
#include "login.h"

LeaveApproveWidget::LeaveApproveWidget(int staff_id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LeaveApproveWidget),
    m_staffId(staff_id)

{


    ui->setupUi(this);
    connect(ui->btnSubmit, &QPushButton::clicked, this, &LeaveApproveWidget::onSubmit);
    QString styleSheet = R"(
            /* 1. 窗口背景 */
            QWidget#LeaveApproveWidget {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                           stop:0 #F5F7FA, stop:1 #ECEEF2);
            }

            /* 2. 表格样式（对应 tableHistory） */
            QTableWidget#tableHistory {
                background-color: #FFFFFF;
                border-radius: 12px;
                border: none;
                box-shadow: 0 4px 12px rgba(0,0,0,0.03);
                padding: 12px;
                font-family: "Microsoft YaHei", sans-serif;
                font-size: 30px;
                color: #4B5563;
            }

            /* 3. 表头样式 */
            QTableWidget#tableHistory QHeaderView::section {
                background-color: #F9FAFB;
                border-radius: 8px 8px 0 0;
                border: none;
                padding: 10px 8px;
                font-weight: 600;
                font-size: 30px;
                color: #6B7280;
                text-align: center;
            }

            /* 4. 表格行样式 */
            QTableWidget#tableHistory::item {
                border-bottom: 1px solid #F1F2F6;
                padding: 12px 8px;
            }

            QTableWidget#tableHistory::item:selected {
                background-color: #EEF4FF;
                color: #3B82F6;
            }

            /* 5. 提交按钮样式（对应 btnSubmit） */
            QPushButton#btnSubmit {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                           stop:0 #66B2FF, stop:1 #4A90E2);
                color: #FFFFFF;
                border-radius: 8px;
                border: none;
                padding: 10px 20px;
                font-size: 30px;
                font-weight: 500;
            }

            QPushButton#btnSubmit:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                           stop:0 #54A3FF, stop:1 #3F87D9);
            }

            /* 6. 日期选择控件样式（对应 editStart 和 editEnd） */
            QDateTimeEdit#editStart, QDateTimeEdit#editEnd {
                background-color: #FFFFFF;
                border: 1px solid #E0E0E0;
                border-radius: 6px;
                padding: 6px 8px;
                font-size: 30px;
                color: #4B5563;
            }

            QDateTimeEdit#editStart:hover, QDateTimeEdit#editEnd:hover {
                border-color: #C0C0C0;
            }

            /* 7. 输入框样式（对应 editReason） */
            QLineEdit#editReason {
                background-color: #FFFFFF;
                border: 1px solid #E0E0E0;
                border-radius: 6px;
                padding: 6px 8px;
                font-size: 30px;
                color: #4B5563;
            }

            QLineEdit#editReason:hover {
                border-color: #C0C0C0;
            }
        )";

        // 应用样式到窗口
        this->setObjectName("LeaveApproveWidget");
        this->setStyleSheet(styleSheet);

        // 表格自适应优化
        ui->tableHistory->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        ui->tableHistory->horizontalHeader()->setStretchLastSection(true);
        ui->tableHistory->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    refreshTable();

}

LeaveApproveWidget::~LeaveApproveWidget()
{
    delete ui;
}
void LeaveApproveWidget::onSubmit()
{
    qDebug()<<"submit";

    QString start = ui->editStart->date().toString("yyyy-MM-dd");
    qDebug()<<start;


    QString end = ui->editEnd->date().toString("yyyy-MM-dd");
    qDebug()<<end;

    QString reason = ui->editReason->toPlainText().trimmed();
    qDebug()<<reason;

    if (reason.isEmpty()) {
        QMessageBox::warning(this, "提示", "请填写请假原因！");
        return;
    }


    Data db;

    db.openDB("smart_upc.db");

    QSqlQuery q;
    q.prepare("INSERT INTO leaves (staff_id, start_date, end_date, reason, status) VALUES (?, ?, ?, ?, 0)");

    q.addBindValue(m_staffId);
    q.addBindValue(start);
    q.addBindValue(end);
    q.addBindValue(reason);
    q.exec();




    QMessageBox::information(this, "成功", "请假申请已提交，等待审批！");

    ui->editReason->clear();

    refreshTable();
}

void LeaveApproveWidget::refreshTable()
{

    ui->tableHistory->setRowCount(0);
    ui->tableHistory->setHorizontalHeaderLabels(QStringList()<<"ID"<<"开始日期"<<"结束日期"<<"原因"<<"状态");
    Data db;
    db.openDB("smart_upc.db");
    qDebug()<<m_staffId;
    qDebug()<<"kkk";

    QSqlQuery query;
    query.prepare("SELECT id, start_date, end_date, reason, status FROM leaves WHERE staff_id=? ORDER BY id DESC");
    query.addBindValue(m_staffId);

    qDebug() << "绑定参数：" << query.boundValues();

   query.exec();



    int row = 0;


    while (query.next()) {
        ui->tableHistory->insertRow(row);
        qDebug()<<"j";
        for(int col =0;col<5;++col)
        {
            QString val =query.value(col).toString();
            if(col==4)
            {
                if(val=="0") val="待审批";
                else if(val=="1") val="已批准";
                else if(val=="2") val="已拒绝";
                else val ="未知("+val+")";


            }
            ui->tableHistory->setItem(row,col,new QTableWidgetItem(val));

        }

        row++;
    }
}
//void LeaveApproveWidget::refreshTable()
//{
//    ui->tableHistory->setRowCount(0);
//    Data db;
//    db.openDB("smart_upc.db");
//    QSqlQuery query=db.query("SELECT* FROM leaves");
//    qDebug() << "查询字段数:" << query.record().count();
//    int row=0;
//    while(query.next())
//    {


//        ui->tableHistory->insertRow(row);
//        for(int col=0;col<6;++col)
//        {
//            QString val=query.value(col).toString();
//            if(col==5)
//            {
//                if(val=="1")val="已审批";
//                else if(val=="0")val="";
//                else val ="未知";


//            }
//            ui->tableHistory->setItem(row,col,new QTableWidgetItem(val));
//        }
//        ++row;
//    }
//    ui->tableHistory->resizeRowsToContents();
//}
