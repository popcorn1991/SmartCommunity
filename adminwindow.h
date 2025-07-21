#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H
#include "staffmanagewidget.h"
#include <QMainWindow>
#include<QStackedWidget>
#include "staffsuperwidget.h"
#include "leavejudgewidget.h"
#include "leaveapprovewidget.h"
#include "C:\Users\zhang\Desktop\oppo\opip\PayCheckManageWidget.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\pone.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\pthree.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\pfour.h"
#include "C:\Users\zhang\Desktop\oppo\untitled1\pfive.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class adminwindow;
}
QT_END_NAMESPACE

//++class StaffManageWidget;
//++class LeaveApproveWidget;
//++class AttendanceWidget;
class adminwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit adminwindow(int staff_Id,int username,QWidget *parent = nullptr);
    ~adminwindow();
    void showStaffManage();
    void showLeaveManage();
    void showOut();
    void showPay();
   void showStaff();
   void showAtt();
   void showSet();
   void showLeave();
   void show0();


    QStackedWidget *stackedWidget;
    staffSuperWidget* staffWidget;
    leaveJudgeWidget* leaveWidget;
    LeaveApproveWidget* outWidget;
    PayCheckWidget *payer;
    PTHREE *p3;

    PONE *poner;
    PFOUR* pfourer;
    PFIVE* pfiver;







private:
    void setupMenus();
    int m_userId;
    int ms_id;
    Ui::adminwindow *ui;
};

#endif // ADMINWINDOW_H
