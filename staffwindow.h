#ifndef STAFFWINDOW_H
#define STAFFWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "fixjudgewidget.h"
#include "leaveapprovewidget.h"
#include "rentwidget.h"
#include "C:\Users\zhang\Desktop\oppo\parkingapprovewidget.h"
#include "C:\Users\zhang\Desktop\oppo\PaymentManageWidget.h"
#include "C:\Users\zhang\Desktop\oppo\opip\OnlineChargeWidget.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\staffpage11.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\staffpage13.h"
#include "C:\Users\zhang\Desktop\oppo\chargewidget.h"
#include "annwidget.h"

#include "QVBoxLayout"

namespace Ui {
class StaffWindow;
}

class StaffWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StaffWindow(int staff_id,int userId,QWidget *parent = nullptr);
    ~StaffWindow();
    LeaveApproveWidget* outWidget;
private slots:
    void showOut();

private:
    Ui::StaffWindow *ui;
    int m_userId;
    int ms_userId;
    QStackedWidget* stackedWidget;
    fixJudgeWidget* fixJudger;
    ParkingApproveWidget* car;

    StaffPage11* attend;
    StaffPage13* card;
    annWidget* ann;
    ChargeWidget* ch;
    OnlineChargeWidget* pay;
    void setupMenus();
    void showFixJudgeManage();
    void showRent();
    void showCar();
    void showPay();
    void showAtt();
    void showCard();
    void showAnn();
    void showCh();


    rentWidget* renter;






    QWidget *sidebarWidget;
    QVBoxLayout *sidebarLayout;

};

#endif // STAFFWINDOW_H
