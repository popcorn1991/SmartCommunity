#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "staffmanagewidget.h"
#include "staffsuperwidget.h"
#include "leaveapprovewidget.h"
#include "leavejudgewidget.h"
#include "C:\Users\zhang\Desktop\oppo\opip\PayCheckManageWidget.h"
#include <QStackedWidget>
#include "data.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\pone.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\pthree.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\pfour.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\pfive.h"


#include <QDebug>






adminwindow::adminwindow(int staff_Id,int userId,QWidget *parent) :
    QMainWindow(parent),

    m_userId(userId),
    ms_id(staff_Id),
    ui(new Ui::adminwindow)
{
    Data* db=new Data();
    db->openDB("smart_upc.db");
    qDebug()<<"ggg";
    ui->setupUi(this);
    stackedWidget = new QStackedWidget(this);
    staffWidget = new staffSuperWidget(this);
    leaveWidget = new leaveJudgeWidget(ms_id,this);
    p3=new PTHREE(this);
    payer =new PayCheckWidget(this);
    poner =new PONE(this);
    pfourer =new PFOUR(this);
    pfiver =new PFIVE(this);









    stackedWidget->addWidget(staffWidget);
     stackedWidget->addWidget(leaveWidget);
     stackedWidget->addWidget(payer);
     stackedWidget->addWidget(poner);
     stackedWidget->addWidget(p3);
     stackedWidget->addWidget(pfourer);
     stackedWidget->addWidget(pfiver);








    setCentralWidget(stackedWidget);


    stackedWidget->setCurrentWidget(staffWidget); // 直接用已知添加的 widget 测试
    qDebug() << "Current widget after set:" << stackedWidget->currentWidget();
    setupMenus();

}

adminwindow::~adminwindow()
{
    delete ui;
}
void adminwindow::setupMenus()
{
    QMenuBar*mB=ui->menuBar;
    mB->clear();
    QMenu* staffMenu=mB->addMenu("人事管理");//addMenu()用于在菜单里添加一个动作（即菜单项），并返回该动作的指针
    QAction* actStaffManage =staffMenu->addAction("物业工作人员维护");



    QAction* actLeave =staffMenu->addAction("请销假审批");
    QAction* actAttendance=staffMenu->addAction("出勤记录管理");
    QMenu* staffMenu2=mB->addMenu("支付管理");
     QAction* actPay =staffMenu2->addAction("支付查询");
     QMenu* staffMenu3=mB->addMenu("系统设置");//addMenu()用于在菜单里添加一个动作（即菜单项），并返回该动作的指针
     QAction* actSet =staffMenu3->addAction("系统设置");
     QAction* act0=staffMenu3->addAction("系统初始化");










    connect(actStaffManage,&QAction::triggered,this,&adminwindow::showStaffManage);
    connect(actPay,&QAction::triggered,this,&adminwindow::showPay);

    connect(actPay,&QAction::triggered,this,&adminwindow::showPay);
    connect(actAttendance,&QAction::triggered,this,&adminwindow::showAtt);
    connect(actLeave,&QAction::triggered,this,&adminwindow::showLeave);
    connect(actSet,&QAction::triggered,this,&adminwindow::showSet);
    connect(act0,&QAction::triggered,this,&adminwindow::show0);






}
void adminwindow::showStaffManage()
{
    qDebug()<<"a";
   stackedWidget->setCurrentWidget(staffWidget);
   if (stackedWidget) {
           stackedWidget->setCurrentWidget(staffWidget);
           qDebug() << "当前页面索引:" << stackedWidget->currentIndex();
           qDebug() << "当前页面对象:" << stackedWidget->currentWidget();
       } else {
           qDebug() << "错误：stackedWidget 为空！";
       }
   setWindowTitle("管理人员管理系统-物业工作人员维护");
}
void adminwindow::showLeaveManage()
{
    qDebug()<<"a";
   stackedWidget->setCurrentWidget(leaveWidget);
   setWindowTitle("管理人员管理系统-请销假审批");

}
void adminwindow::showOut()
{
    stackedWidget->setCurrentWidget(outWidget);
    setWindowTitle("管理人员管理系统-请销假审批");

}
void adminwindow::showPay()
{
    stackedWidget->setCurrentWidget(payer);
        setWindowTitle("管理人员管理系统-支付查询");
}
void adminwindow::showStaff()
{
    stackedWidget->setCurrentWidget(poner);
        setWindowTitle("管理人员管理系统-物业工作人员查看");
}
void adminwindow::showAtt()
{
    stackedWidget->setCurrentWidget(p3);
        setWindowTitle("管理人员管理系统-出勤记录管理");
}
void adminwindow::showSet()
{
    stackedWidget->setCurrentWidget(pfourer);
        setWindowTitle("管理人员管理系统-系统设置");
}

void adminwindow::show0()
{
    stackedWidget->setCurrentWidget(pfiver);
        setWindowTitle("管理人员管理系统-系统初始化");
}
void adminwindow::showLeave()
{
    stackedWidget->setCurrentWidget(leaveWidget);
        setWindowTitle("管理人员管理系统-请销假审批");
}



