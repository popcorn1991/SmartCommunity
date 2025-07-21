#include "ownerwindow.h"
#include "ui_ownerwindow.h"
#include "fixwidget.h"
#include "ownerfixcommentwidget.h"
#include "C:\Users\zhang\Desktop\oppo\ParkingManageWidget.h"
#include "C:\Users\zhang\Desktop\oppo\opip\paymentwidget.h"
#include "fixseewidget.h"
#include <QStackedWidget>

#include <QDebug>

OwnerWindow::OwnerWindow(int userId,QWidget *parent) :

    QMainWindow(parent),

    ui(new Ui::OwnerWindow),
    m_userId(userId)
{
    Data* db=new Data();
    db->openDB("smart_upc.db");
    ui->setupUi(this);
    stackedWidget=new  QStackedWidget(this);
    fixer =new fixWidget(m_userId,this);
    fixCommenter =new ownerFixCommentWidget(m_userId,this);
    car =new ParkingManageWidget(m_userId,db,this);
    annc= new anncWidget(this);
    pay =new PaymentManageWidget(m_userId,db,this);
    see= new FixSeeWidget(m_userId,this);

    stackedWidget->addWidget(fixer);
    stackedWidget->addWidget(fixCommenter);
    stackedWidget->addWidget(car);
    stackedWidget->addWidget(annc);
    stackedWidget->addWidget(pay);
    stackedWidget->addWidget(see);




    setCentralWidget(stackedWidget);
    stackedWidget->setCurrentWidget(annc);

    setupMenus();

}

OwnerWindow::~OwnerWindow()
{
    delete ui;
}
void OwnerWindow::setupMenus()
{
    QMenuBar *mB=ui->menubar;
    mB->clear();
    QMenu* ownerMenu1 =mB->addMenu("故障申报");
    QAction* actFix=ownerMenu1->addAction("维修申报");
    QAction* actSee=ownerMenu1->addAction("维修追踪");
    QAction* actFixC=ownerMenu1->addAction("维修评价");
    QMenu* ownerMenu2 =mB->addMenu("车位管理");
    QAction* actcar=ownerMenu2->addAction("车位申请");
    QMenu* ownerMenu3 =mB->addMenu("缴费入口管理");
    QAction* payer=ownerMenu3->addAction("线上缴费");






    connect(actFix,&QAction::triggered,this,&OwnerWindow::showFixManage);
    connect(actFixC,&QAction::triggered,this,&OwnerWindow::showFixC);
    connect(actcar,&QAction::triggered,this,&OwnerWindow::showCar);
    connect(payer,&QAction::triggered,this,&OwnerWindow::showPay);
    connect(actSee,&QAction::triggered,this,&OwnerWindow::showSee);
    mB->setStyleSheet("QMenuBar{"
                      "background-color: #404040;"  // 菜单栏背景色，深灰色示例
                      "color: white;"  // 菜单栏文字颜色
                      "}"
                      "QMenuBar::item{"
                      "padding: 4px 16px;"  // 菜单项内边距，调整点击区域和间距
                      "}"
                      "QMenuBar::item:selected{"
                      "background-color: #606060;"  // 选中菜单项时的背景色
                      "}");
    // 遍历菜单，美化子菜单
    QList<QMenu *> menus = mB->findChildren<QMenu *>();
    for (QMenu *menu : menus) {
        menu->setStyleSheet("QMenu{"
                            "background-color: #F5F5F5;"  // 子菜单背景色，浅灰色示例
                            "color: #333333;"  // 子菜单文字颜色
                            "border: 1px solid #CCCCCC;"  // 子菜单边框
                            "}"
                            "QMenu::item{"
                            "padding: 4px 24px;"  // 子菜单项内边距
                            "}"
                            "QMenu::item:selected{"
                            "background-color: #E0E0E0;"  // 选中子菜单项时的背景色
                            "}");
    }
    this->setStyleSheet("OwnerWindow{"
                            "background-color: #ECECEC;"  // 窗口背景色，浅灰色示例
                            "}"
                            // 若窗口内有其他通用控件想统一美化，也可继续添加规则
                            "QLineEdit{"
                            "border: 1px solid #CCCCCC;"
                            "border-radius: 4px;"
                            "padding: 2px 4px;"
                            "}"
                            "QPushButton{"
                            "background-color: #F0F0F0;"
                            "border: 1px solid #CCCCCC;"
                            "border-radius: 4px;"
                            "padding: 4px 8px;"
                            "}"
                            "QPushButton:hover{"
                            "background-color: #E0E0E0;"
                            "}");

}
void OwnerWindow::showFixManage()
{
    stackedWidget->setCurrentWidget(fixer);
    setWindowTitle("业主管理系统-维修申报");
}
void OwnerWindow::showFixC()
{
     stackedWidget->setCurrentWidget(fixCommenter);
     setWindowTitle("业主管理系统-维修评价");
}
void OwnerWindow::showCar()
{

stackedWidget->setCurrentWidget(car);
setWindowTitle("业主管理系统-车位申请");
}
void OwnerWindow::showPay()
{
    stackedWidget->setCurrentWidget(pay);
    setWindowTitle("业主管理系统-线上缴费");
}
void OwnerWindow::showSee()
{
    stackedWidget->setCurrentWidget(see);
    setWindowTitle("业主管理系统-维修追踪");
}


