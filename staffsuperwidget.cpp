#include "staffsuperwidget.h"
#include "ui_staffsuperwidget.h"
#include "data.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <QInputDialog>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>




staffSuperWidget::staffSuperWidget(QWidget *parent) :
    QWidget(parent),

    ui(new Ui::staffSuperWidget)
{
    QString themeColor = "#2d8cfc";
    QString themeLight = "#cce8ff";

    table = new QTableWidget(this);
    ui->setupUi(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"ID","用户名","姓名","部门","岗位","状态"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->setStyleSheet(QString(R"(
            QTableWidget { border: none; background: #f8f8f8; padding: 12px; }
            QTableWidget::item { background: %1; color:white; font-weight:bold; font-size:14px; }
            QTableWidget::item:selected { background: %2; }
        )").arg(themeColor, themeLight));
    btnAdd = new QPushButton("新增", this);
    btnEdit = new QPushButton("编辑", this);
    btnDelete = new QPushButton("删除", this);
    btnS = new QPushButton("查询", this);
    edit =new QLineEdit();
    QLabel *queryLabel = new QLabel("请输入姓名：", this);
    QHBoxLayout *queryLayout = new QHBoxLayout;
        queryLayout->addWidget(queryLabel);
        queryLayout->addWidget(edit);

    QString gradientStyle =QString( R"(
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %1, stop:1 %2);
                color: white;
                border-radius: 8px;
                min-width: 88px;
                min-height: 50px;
                font-size: 30px;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %3, stop:1 %4);
            }
            QPushButton:pressed {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %5, stop:1 %6);
            }
        )").arg(
            themeColor,
            themeColor.left(7) + "e8",  // 深一点的主题色
            themeColor.left(7) + "f5",  // 浅一点的主题色
            themeColor.left(7) + "e8",
            themeColor.left(7) + "d8",  // 按下状态
            themeColor.left(7) + "b6"   // 按下更深
        );
    table->setStyleSheet(table->styleSheet() + QString(R"(
        QTableWidget::item:hover {
            background: %1;
            color: white;
        }
    )").arg(themeColor.left(7) + "f5"));

    //--------
    btnAdd->setStyleSheet(gradientStyle);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);

    shadow->setColor(QColor(0, 0, 0, 100));
    shadow->setBlurRadius(10);
    shadow->setOffset(2, 2);
    btnAdd->setGraphicsEffect(shadow);
    btnEdit->setStyleSheet(gradientStyle);
    QGraphicsDropShadowEffect *editShadow = new QGraphicsDropShadowEffect(this);
    editShadow->setBlurRadius(10);
    editShadow->setOffset(2, 2);
    btnEdit->setGraphicsEffect(editShadow);



    btnDelete->setStyleSheet(gradientStyle);
    QGraphicsDropShadowEffect *deleteShadow = new QGraphicsDropShadowEffect(this);
    deleteShadow->setBlurRadius(10);
    deleteShadow->setOffset(2, 2);
    btnDelete->setGraphicsEffect(deleteShadow);


    //-----------
    btnAdd->setStyleSheet(gradientStyle);

    QString btnStyle = "QPushButton { background-color: #2d8cf8; color: white; border-radius: 6px;min-width: 80px;min-height: 50px;font-size: 30px;}"
        "QPushButton:hover ( background-color: #1a73e8;}";
    btnAdd->setStyleSheet(btnStyle);
    btnEdit->setStyleSheet(btnStyle.replace("#2d8cf8", "#42b983").replace("#1a73e8", "#13a456")); // 绿色
    btnDelete->setStyleSheet(btnStyle.replace("#2d8cf8", "#f56c6c").replace("#1a73e8", "#d93025"));
    btnS->setStyleSheet(btnStyle);
    auto btnLayout = new QVBoxLayout;



    btnLayout->addStretch();
    btnLayout->addWidget(btnAdd);
    btnLayout->addSpacing(10);
    btnLayout->addWidget(btnEdit);
    btnLayout->addSpacing(10);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();

    btnLayout->addWidget(btnS);
    btnLayout->addWidget(edit);
    btnLayout->addLayout(queryLayout);

    QLabel* title = new QLabel("物业工作人员信息维护", this);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setPointSize(16);
    font.setBold(true);
    title->setFont(font);
    title->setStyleSheet("color: #2d8cf0; margin: 10px;");

    auto* mainLayout = new QVBoxLayout(this);

    //----------------------------
    mainLayout->setSpacing(20);  // 控件间间距
    mainLayout->setContentsMargins(24, 24, 24, 24);  // 界面与窗口边缘的边距，营造呼吸感

    // 给表格增加内边距，让内容不贴边
    table->setStyleSheet(R"(
        QTableWidget { border: none; background: #f8f8f8; padding: 12px; }
        QTableWidget::item { background: #2d8cfc; color:white; font-weight:bold; font-size:14px; }
        QTableWidget::item:selected { background: #cce8ff; }
    )");

    //-----------------------------------
    mainLayout->addWidget(title);
    mainLayout->addLayout(btnLayout);
    mainLayout->addWidget(table);

    connect(btnAdd, &QPushButton::clicked, this, &staffSuperWidget::onAdd);
    connect(btnEdit, &QPushButton::clicked, this, &staffSuperWidget::onEdit);
    connect(btnDelete, &QPushButton::clicked, this, &staffSuperWidget::onDelete);
    connect(btnS, &QPushButton::clicked, this, &staffSuperWidget::onS);

    //connect(btnDelete, &QPushButton::clicked, this, &StaffManageWidget::onDelete);
     refreshTable();
     qDebug() << "表头列数:" << table->horizontalHeader()->count();



}
void staffSuperWidget::refreshTable()
{
    table->setRowCount(0);
    Data db;
    db.openDB("smart_upc.db");
    QSqlQuery query=db.query("SELECT test.id,test.username,test.name,test.department,test.position,test.status "
               " FROM test WHERE test.role=1");
    qDebug() << "查询字段数:" << query.record().count();
    int row=0;
    while(query.next())
    {


        table->insertRow(row);
        for(int col=0;col<6;++col)
        {
            QString val=query.value(col).toString();
            if(col==5)
            {
                if(val=="1")val="在职";
                else if(val=="0")val="离职";
                else val ="未知";


            }
            table->setItem(row,col,new QTableWidgetItem(val));
        }
        ++row;
    }
    table->resizeRowsToContents();
}

void staffSuperWidget::onAdd()
{
    bool ok;
    QString username = QInputDialog::getText(this,"新增工作人员","用户名：",QLineEdit::Normal,"",&ok);
    if(!ok||username.isEmpty()) return;
    QString password = QInputDialog::getText(this,"新增工作人员","密码：",QLineEdit::Normal,"",&ok);
    if(!ok||password.isEmpty()) return;
    QString name = QInputDialog::getText(this,"新增工作人员","姓名：",QLineEdit::Normal,"",&ok);
    if(!ok) return;
    QString department = QInputDialog::getText(this,"新增工作人员","部门：",QLineEdit::Normal,"",&ok);
    if(!ok) return;
    QString position = QInputDialog::getText(this,"新增工作人员","岗位：",QLineEdit::Normal,"",&ok);
    if(!ok) return;

    Data db;
    db.openDB("smart_upc.db");
    QSqlQuery q;
    q.prepare("INSERT INTO test(username,password,role,name,department,position,status)"
              "VALUES(?,?,1,?,?,?,1)");

    q.addBindValue(username);
    q.addBindValue(password);
    q.addBindValue(name);
    q.addBindValue(department);
    q.addBindValue(position);
    if (q.exec()) {
            QMessageBox::information(this,"成功","已添加该员工！");
        } else {
            // 执行失败时，打印详细错误信息（含执行的 SQL、数据库返回的错误描述）
            qDebug() << "插入失败，执行的 SQL：" << q.executedQuery();
            qDebug() << "错误信息：" << q.lastError().text();
        }
    refreshTable();


}
void staffSuperWidget::onEdit()
{
    int row=table->currentRow();
    if(row<0) return;
    int id=table->item(row,0)->text().toInt();
    QString name=QInputDialog::getText(this,"编辑姓名","姓名",QLineEdit::Normal,table->item(row,2)->text());
    QString department=QInputDialog::getText(this,"编辑部门","部门",QLineEdit::Normal,table->item(row,3)->text());
    QString position=QInputDialog::getText(this,"编辑岗位","岗位",QLineEdit::Normal,table->item(row,4)->text());
    Data db;
    db.openDB("smart_upc.db");
    QSqlQuery q1;
    q1.prepare("UPDATE test SET name=? WHERE id=?");
    q1.addBindValue(name);
    q1.addBindValue(id);

    QSqlQuery q2;
    q2.prepare("UPDATE test SET department=?,position=? WHERE id=?");
    q2.addBindValue(department);
    q2.addBindValue(position);
    q2.addBindValue(id);

    if (!q1.exec()) {
       QMessageBox::information(this,"成功","已更改该员工信息！");
    }
    if (!q2.exec()) {
        qDebug() << "q2 exec error: " << q2.lastError().text();
    }


    refreshTable();

}


void staffSuperWidget::onDelete()
{
    qDebug()<<"11";
    int row=table->currentRow();
    if(row<0) return;
    int user_id=table->item(row,0)->text().toInt();
    Data db;
    db.openDB("smart_upc.db");
     QSqlQuery q1;
    q1.prepare("DELETE FROM test  WHERE id=?");
    q1.addBindValue(user_id);
    if (q1.exec()) {
       QMessageBox::information(this,"成功","已删除该员工信息！");
    }

    refreshTable();


}
void staffSuperWidget::onS()
{
   QString s=edit->text();
   Data db;
   db.openDB("smart_upc.db");
    QSqlQuery q1;
   q1.prepare("SELECT test.id,test.username,test.name,test.department,test.position,test.status FROM test  WHERE name=?");
   q1.addBindValue(s);
   table->setRowCount(0);
   int row=0;
   q1.exec();
   while(q1.next())
   {


       table->insertRow(row);
       for(int col=0;col<6;++col)
       {
           QString val=q1.value(col).toString();
           if(col==5)
           {
               if(val=="1")val="在职";
               else if(val=="0")val="离职";
               else val ="未知";


           }
           table->setItem(row,col,new QTableWidgetItem(val));
       }
       ++row;
   }
   table->resizeRowsToContents();


}

staffSuperWidget::~staffSuperWidget()
{
    delete ui;
}
