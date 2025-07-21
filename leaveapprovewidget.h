#ifndef LEAVEAPPROVEWIDGET_H
#define LEAVEAPPROVEWIDGET_H

#include <QWidget>

namespace Ui {
class LeaveApproveWidget;
}

class LeaveApproveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LeaveApproveWidget(int staffId,QWidget *parent = nullptr);
    ~LeaveApproveWidget();
    void refreshTable();
    void onSubmit();

private:
    Ui::LeaveApproveWidget *ui;
    int m_staffId;
};

#endif // LEAVEAPPROVEWIDGET_H
