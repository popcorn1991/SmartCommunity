#ifndef OWNERWINDOW_H
#define OWNERWINDOW_H
#include "fixwidget.h"
#include "anncwidget.h"
#include <QMainWindow>
#include <QStackedWidget>
#include "ownerfixcommentwidget.h"
#include "C:\Users\zhang\Desktop\oppo\ParkingManageWidget.h"
#include "C:\Users\zhang\Desktop\oppo\opip\paymentwidget.h"
#include "fixseewidget.h"



namespace Ui {
class OwnerWindow;
}

class OwnerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit OwnerWindow(int userId,QWidget *parent = nullptr);
    ~OwnerWindow();
    QStackedWidget* stackedWidget;
    fixWidget* fixer;
    ownerFixCommentWidget* fixCommenter;
    ParkingManageWidget *car;
    anncWidget* annc;
    PaymentManageWidget* pay;
    FixSeeWidget* see;


    void setupMenus();
private slots:
    void showFixManage();
    void showFixC();
    void showCar();
    void showPay();
    void showSee();

private:
    Ui::OwnerWindow *ui;
    int m_userId;
};

#endif // OWNERWINDOW_H
