#include "leavejudgewidget.h"
#include "ui_leavejudgewidget.h"
#include "data.h"
#include <QTableView>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDebug>
#include <QTableWidget>
#include <QMessageBox>
#include <QDateTime>
#include <QHBoxLayout>
#include <QSqlError>
#include <QGroupBox>



leaveJudgeWidget::leaveJudgeWidget(int staff_Id,QWidget *parent) :

    QWidget(parent),
    ms_Id(staff_Id),
    ui(new Ui::leaveJudgeWidget)

{
    ui->setupUi(this);


    qDebug() << "LeaveApproveWidget is running";
        table = new QTableWidget(this);
        setupStyle();
        table->setColumnCount(8);
        table->setHorizontalHeaderLabels(QStringList() << "ID" << "员工ID" << "开始日期" << "结束日期" << "原因" << "状态" << "审批人ID" << "审批时间");

        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);

        table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        // 如果想“适配内容后，剩余空间平均分配”，再加这行（可选）
        table->horizontalHeader()->setStretchLastSection(true);

        // 新增：让行高也自动适配内容（如果单元格内容高度不一，很有用）
        table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

        btnApprove = new QPushButton("批准", this);
        btnReject = new QPushButton("拒绝", this);

        auto *btnLayout = new QHBoxLayout;
        btnLayout->addWidget(btnApprove);
        btnLayout->addWidget(btnReject);
        btnLayout->addStretch();

        auto *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(table);
        mainLayout->addLayout(btnLayout);

        connect(btnApprove, &QPushButton::clicked, this, &leaveJudgeWidget::onApprove);
        connect(btnReject, &QPushButton::clicked, this, &leaveJudgeWidget::onReject);
        refreshTable();
        qDebug() << "可用数据库驱动：" << QSqlDatabase::drivers();


}

leaveJudgeWidget::~leaveJudgeWidget()
{
    delete ui;
}
void leaveJudgeWidget::refreshTable()
{
    table->setRowCount(0);
    Data db;
    db.openDB("smart_upc.db");
    QSqlQuery query=db.query("SELECT id,staff_id,start_date,end_date,reason,status,approver_id,approve_time FROM leaves ORDER BY status,id DESC");
    int row =0;
    while(query.next())
    {
        qDebug()<<"sdsd";
        table->insertRow(row);
        for(int col=0;col<8;++col)
        {
            QString val =query.value(col).toString();
            if(col==5)
            {
                int status = query.value(col).toInt();
                if(status==0) val ="待审批";
                else if(status == 1) val="已批准";
                else if(status == 2) val="已拒绝";

            }
            table->setItem(row,col,new QTableWidgetItem(val));
        }
        ++row;
    }

}
void leaveJudgeWidget::onApprove()
{
    int row =table->currentRow();
    if(row< 0)return ;
    int leave_id =table->item(row,0)->text().toInt();
    int status =table->item(row,5)->text().toInt();
    if(status!=0)
    {
        QMessageBox::information(this,"提示","该申请已审批");
        return;
    }
    int approver_id =ms_Id;
    QString approve_time =QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    Data db;
    db.openDB("smart_upc.db");
    QSqlQuery q;
    q.prepare("UPDATE leaves SET status=1,approver_id=?,approve_time=? WHERE id=?");
   q.addBindValue(approver_id);
    q.addBindValue(approve_time);
    q.addBindValue( leave_id);
    qDebug() << "o:"<<approver_id;
    qDebug() << "p：" <<approve_time ;  // 看绑定了几个参数
    qDebug() << "l：" << leave_id;     // 看具体绑定了什么
    if (q.exec()) {
        QMessageBox::information(this, "成功", "已批准该请假申请！");
        refreshTable();
    } else {
        // 获取并弹出详细错误信息
        QSqlError error = q.lastError();
        qDebug() << "完整错误信息：" << error.databaseText();
        qDebug() << "驱动错误信息：" << error.driverText();
        QString errorMsg = QString("操作失败！错误信息：%1，错误类型：%2")
                           .arg(error.text())
                           .arg(error.type());
        qDebug() << "UPDATE 执行失败：" << q.lastError().text();
        QMessageBox::critical(this, "失败", errorMsg);
    }



}

    void leaveJudgeWidget::onReject()
    {
        int row =table->currentRow();
        if(row< 0)return ;
        int leave_id =table->item(row,0)->text().toInt();
        int status =table->item(row,5)->text().toInt();
        if(status!=0)
        {
            QMessageBox::information(this,"提示","该申请已审批");
            return;
        }
        int approver_id =ms_Id;
        QString approve_time =QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

        Data db;
        db.openDB("smart_upc.db");
        QSqlQuery q;
        q.prepare("UPDATE leaves SET status=2,approver_id=?,approve_time=? WHERE id=?");


        q.addBindValue(approver_id);
        q.addBindValue(approve_time);
        q.addBindValue(leave_id);


        if(q.exec())
        {
            QMessageBox::information(this,"成功","已拒绝该请假申请！");

        if(q.exec())
        {
            QMessageBox::information(this,"成功","已拒绝该请假申请！");
                 refreshTable();

        }
        else {
            {
                QMessageBox::information(this,"失败","操作失败!");
            }
        }



}
    }
    void leaveJudgeWidget::setupStyle() {
        QString styleSheet = R"(
                /* 1. 窗口背景：保持浅灰渐变 */
                QWidget#leaveJudgeWidget {
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                               stop:0 #F5F7FA, stop:1 #ECEEF2);
                    font-size: 30px; /* 全局基础字体，可按需调整 */
                }

                /* 2. 表格容器：卡片化 + 更大字体 */
                QTableWidget {
                    background-color: #FFFFFF;
                    border-radius: 12px;
                    border: none;
                    box-shadow: 0 4px 12px rgba(0,0,0,0.03);
                    padding: 16px; /* 增大内边距，让内容不拥挤 */

                    font-family: "Microsoft YaHei", sans-serif;
                    font-size: 30px; /* 表格字体加大 */
                    color: #4B5563;
                }

                /* 3. 表头：调整字体和间距 */
                QTableWidget::horizontalHeader::section {
                    border: none;
                    padding: 12px 8px;
                    font-weight: 600;
                    font-size: 30px; /* 表头字体更大 */
                    color: #6B7280;
                    text-align: center;
                }

                /* 4. 表格行：增大行高适配字体 */
                QTableWidget::item {
                    border-bottom: 1px solid #F1F2F6;
                    padding: 14px 8px; /* 增大行内边距 */
                }

                /* 5. 按钮：加大字体 + 优化布局 */
                QPushButton {
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                               stop:0 #66B2FF, stop:1 #4A90E2);
                    color: #FFFFFF;
                    border-radius: 8px;
                    border: none;
                    padding: 12px 24px; /* 加大按钮尺寸 */
                    font-size: 30px; /* 按钮字体加大 */
                    margin: 0 12px; /* 按钮间间距更大 */
                }

                QPushButton:hover {
                    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                               stop:0 #54A3FF, stop:1 #3F87D9);
                }
            )";

            this->setObjectName("leaveJudgeWidget");
            this->setStyleSheet(styleSheet);

            // 表格适配优化（不变）
            table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            table->horizontalHeader()->setStretchLastSection(true);
            table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

            // 按钮布局调整：让按钮居右显示
            // 找到按钮布局（假设是 btnLayout），添加拉伸让按钮靠右
            if (QHBoxLayout *btnLayout = qobject_cast<QHBoxLayout*>(btnLayout)) {
                btnLayout->addStretch(); // 在按钮左侧加拉伸，按钮会靠右
            }
    }
