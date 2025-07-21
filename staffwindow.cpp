#include "staffwindow.h"
#include "ui_staffwindow.h"
#include "fixjudgewidget.h"
#include "rentwidget.h"
#include "C:\Users\zhang\Desktop\oppo\parkingapprovewidget.h"
#include "C:\Users\zhang\Desktop\oppo\opip\OnlineChargeWidget.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\staffpage11.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\staffpage13.h"
#include "C:\Users\zhang\Desktop\oppo\opip\ChargeWidget(1).h"
#include <QStackedWidget>
#include "annwidget.h"
#include "QVBoxLayout"

StaffWindow::StaffWindow(int staff_id,int userId,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StaffWindow),
   m_userId(userId),
 ms_userId(staff_id)
{


    ui->setupUi(this);
    Data* db=new Data();
    db->openDB("smart_upc.db");
    stackedWidget =new QStackedWidget(this);
    fixJudger =new fixJudgeWidget(ms_userId,this);
    outWidget = new LeaveApproveWidget(ms_userId,this);
    renter =new rentWidget(this);
    car=new ParkingApproveWidget(this);
    pay =new OnlineChargeWidget(this);
    attend=new StaffPage11(this);
    card=new StaffPage13(this);
    ann=new annWidget(this);
    ch =new ChargeWidget(this);

    stackedWidget->addWidget(fixJudger);
    stackedWidget->addWidget(outWidget);
    stackedWidget->addWidget(renter);
    stackedWidget->addWidget(car);
    stackedWidget->addWidget(pay);
    stackedWidget->addWidget(attend);
    stackedWidget->addWidget(card);
    stackedWidget->addWidget(ann);
    stackedWidget->addWidget(ch);








    setCentralWidget(stackedWidget);

    setupMenus();






}

StaffWindow::~StaffWindow()
{


    delete ui;
}
void StaffWindow::setupMenus()
{
    QMenuBar *mB=ui->menubar;
    mB->clear();
    QMenu* staffMenu1 =mB->addMenu("故障处理");
    QAction* actFix=staffMenu1->addAction("维修申请处理");




    QMenu* staffMenu2 =mB->addMenu("房产管理");
    QAction* actRent=staffMenu2->addAction("房产管理");

    QMenu* staffMenu3 =mB->addMenu("车位管理");
    QAction* actCar=staffMenu3->addAction("车位管理");

    QMenu* staffMenu4 =mB->addMenu("支付管理");
    QAction* actPay=staffMenu4->addAction("缴费通知");
    QAction* actCh=staffMenu4->addAction("线下缴费");

    QMenu* staffMenu5 =mB->addMenu("人事管理");
    QAction* actAtt=staffMenu5->addAction("月度出勤情况");
    QAction* actCard=staffMenu5->addAction("打卡");

    QMenu* staffMenu6 =mB->addMenu("请假管理");
    QAction* actOut=staffMenu6->addAction("请假申请");

    QMenu* staffMenu7 =mB->addMenu("社区公告");
    QAction* actAnn =staffMenu7->addAction("社区公告发布");










    connect(actFix,&QAction::triggered,this,&StaffWindow::showFixJudgeManage);
    connect(actOut,&QAction::triggered,this,&StaffWindow::showOut);
    connect(actRent,&QAction::triggered,this,&StaffWindow::showRent);
    connect(actCar,&QAction::triggered,this,&StaffWindow::showCar);
    connect(actPay,&QAction::triggered,this,&StaffWindow::showPay);
    connect(actAtt,&QAction::triggered,this,&StaffWindow::showAtt);
    connect(actCard,&QAction::triggered,this,&StaffWindow::showCard);
    connect(actAnn,&QAction::triggered,this,&StaffWindow::showAnn);
    connect(actCh,&QAction::triggered,this,&StaffWindow::showCh);



}

void StaffWindow::showFixJudgeManage()
{
    stackedWidget->setCurrentWidget(fixJudger);
    setWindowTitle("工作人员管理系统-维修申请处理");

}
void StaffWindow::showOut()
{
    stackedWidget->setCurrentWidget(outWidget);
    setWindowTitle("工作人员管理系统-请假申请");
}
void StaffWindow::showRent()
{
    stackedWidget->setCurrentWidget(renter);
    setWindowTitle("工作人员管理系统-房产管理");
}
void StaffWindow::showCar()
{
    stackedWidget->setCurrentWidget(car);
    setWindowTitle("工作人员管理系统-车位管理");
}
void StaffWindow::showPay()
{
    stackedWidget->setCurrentWidget(pay);
    setWindowTitle("工作人员管理系统-缴费通知");
}
void StaffWindow::showAtt()
{
    stackedWidget->setCurrentWidget(attend);
    setWindowTitle("工作人员管理系统-月度出勤情况");
}
void StaffWindow::showCard()
{
    stackedWidget->setCurrentWidget(card);
    setWindowTitle("工作人员管理系统-打卡");
}
void StaffWindow::showAnn()
{
    stackedWidget->setCurrentWidget(ann);
    setWindowTitle("工作人员管理系统-社区公告发布");
}
void StaffWindow::showCh()
{
    stackedWidget->setCurrentWidget(ch);

    setWindowTitle("工作人员管理系统-线下缴费");
}


