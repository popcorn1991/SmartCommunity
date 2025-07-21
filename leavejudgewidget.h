#ifndef LEAVEJUDGEWIDGET_H
#define LEAVEJUDGEWIDGET_H


#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

namespace Ui {
class leaveJudgeWidget;
}

class leaveJudgeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit leaveJudgeWidget(int staff_Id,QWidget *parent = nullptr);

    ~leaveJudgeWidget();


private slots:
    void onApprove();
    void onReject();
    void refreshTable();
     void setupStyle();

private:
    private: QTableWidget *table;
    QPushButton* btnApprove;
    QPushButton* btnReject;
    int ms_Id;



    Ui::leaveJudgeWidget *ui;
};

#endif // LEAVEJUDGEWIDGET_H
