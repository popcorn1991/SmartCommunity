#include "fixseewidget.h"
#include "ui_fixseewidget.h"
#include "QHeaderView"
#include "QVBoxLayout"
#include "data.h"
#include <QSqlQuery>
#include <QPushButton>
#include <QDebug>

FixSeeWidget::FixSeeWidget(int m_id,QWidget *parent) :
    QWidget(parent),
    m_id(m_id),
    ui(new Ui::FixSeeWidget)
{
    ui->setupUi(this);
    table = new QTableWidget(this);
    table->setColumnCount(5);
    table->setHorizontalHeaderLabels(QStringList() << "编号" << "提交人ID" << "标题" << "内容" << "状态" );
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    btn = new QPushButton("刷新", this);


    auto *mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(table);
    mainLayout->addWidget(btn);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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
        this->setObjectName("ownerFixCommentWidget");
        this->setStyleSheet(styleSheet);
    connect(btn, &QPushButton::clicked, this, &FixSeeWidget::refreshTable);
    refreshTable();

}

FixSeeWidget::~FixSeeWidget()
{
    delete ui;
}
void FixSeeWidget::refreshTable()
{
    table->setRowCount(0);
    Data db;
    db.openDB("smart_upc.db");
    QSqlQuery query;
    query.prepare("SELECT id,o_id,title,text,status,staff_id FROM fix WHERE  o_id=? ORDER BY status,id DESC;");
    query.addBindValue(m_id);
    qDebug()<<m_id;

    query.exec();
    int row =0;
    while(query.next())
    {

        table->insertRow(row);
        for(int col=0;col<5;++col)
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

