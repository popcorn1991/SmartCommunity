#include "ownerfixcommentwidget.h"
#include "ui_ownerfixcommentwidget.h"
#include <QTableView>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDebug>
#include <QTableWidget>
#include <QMessageBox>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

#include "data.h"

ownerFixCommentWidget::ownerFixCommentWidget(int id,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ownerFixCommentWidget),
    m_id(id)
{
    ui->setupUi(this);

    table = new QTableWidget(this);
    table->setColumnCount(9);
    table->setHorizontalHeaderLabels(QStringList() << "编号" << "提交人ID" << "标题" << "内容" << "状态" << "审批人ID" << "审批时间" << "评价" << "评分");
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    btnSubmit = new QPushButton("提交", this);
    scoreEdit = new QLineEdit;
    commentEdit = new QLineEdit;
    scoreLabel=new QLabel("评分");
    commentLabel=new QLabel("评价");
    scoreEdit->setPlaceholderText("请输入评分(满分五分)");
    commentEdit->setPlaceholderText("请输入评价");



    auto *editLayout = new QHBoxLayout();
    editLayout->addWidget(commentLabel);
    editLayout->addWidget(commentEdit);
    editLayout->addWidget(scoreLabel);
    editLayout->addWidget(scoreEdit);



    auto *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(btnSubmit);

    btnLayout->addStretch();

    auto *mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(table);
    mainLayout->addLayout(editLayout);
    mainLayout->addLayout(btnLayout);



    connect(btnSubmit, &QPushButton::clicked, this, &ownerFixCommentWidget::onSubmit);
    QString styleSheet = R"(
            /* 窗口背景 */
            QWidget#ownerFixCommentWidget {
                background-color: #f5f7fa;
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

            /* 输入框样式 */
            QLineEdit {
                background-color: white;
                border: 1px solid #e0e0e0;
                border-radius: 4px;
                padding: 6px 8px;
                font-size: 30px;
                color: #333;
            }
            QLineEdit:hover {
                border-color: #c0c0c0;
            }
            QLineEdit:focus {
                border-color: #66b2ff;
                outline: none;
            }

            /* 标签样式 */
            QLabel {
                font-size: 30px;
                color: #555;
                padding: 5px 0;
            }
        )";
        // 设置 objectName，让样式表能匹配到当前窗口
        this->setObjectName("ownerFixCommentWidget");
        this->setStyleSheet(styleSheet);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  refreshTable();
}



ownerFixCommentWidget::~ownerFixCommentWidget()
{
    delete ui;
}
void ownerFixCommentWidget::onSubmit()
{

    int score =scoreEdit->text().toInt();
    qDebug()<<score;
    QString comment=commentEdit->text();
    qDebug()<<comment;
    Data db;

    db.openDB("smart_upc.db");
    int row =table->currentRow();
    int id =table->item(row,0)->text().toInt();
    QSqlQuery q;
    q.prepare("UPDATE fix SET score=?,comment=? WHERE id=?");
    // 插入成功后弹出提示框告知用户
    q.addBindValue(score);
    q.addBindValue(comment);
    q.addBindValue(id);


    if(q.exec())
    {
        QMessageBox::information(this,"成功","已成功评价！");
             refreshTable();

    }
    else {
        {
            QMessageBox::information(this,"失败","操作失败!");
        }
    }
    refreshTable();
}
void ownerFixCommentWidget::refreshTable()
{
    table->setRowCount(0);
    Data db;
    db.openDB("smart_upc.db");
    QSqlQuery query;
    query.prepare("SELECT id,o_id,title,text,status,staff_id,date,comment,score FROM fix WHERE status=2 AND o_id=? ORDER BY status,id DESC;");
    query.addBindValue(m_id);
    qDebug()<<m_id;
    query.exec();
    int row =0;
    while(query.next())
    {
        qDebug()<<"sdsd";
        table->insertRow(row);
        for(int col=0;col<9;++col)
        {
            QString val =query.value(col).toString();
            if(col==4)
            {
                int status = query.value(col).toInt();
                if(status==0) val ="未开始";
                else if(status == 1) val="进行中";
                else if(status == 2) val="处理完毕";

            }
            table->setItem(row,col,new QTableWidgetItem(val));
        }
        ++row;
    }

}
