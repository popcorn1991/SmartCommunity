#ifndef FIXJUDGEWIDGET_H
#define FIXJUDGEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>

namespace Ui {
class fixJudgeWidget;
}

class fixJudgeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit fixJudgeWidget(int staff_id,QWidget *parent = nullptr);
    ~fixJudgeWidget();
    void refreshTable();
    void onApprove();
    void onReject();
    QTableWidget* table;
    QPushButton* btnApprove;
    QPushButton* btnReject;
    QComboBox* comboBox;



private:
    Ui::fixJudgeWidget *ui;
    int ms_id;
};

#endif // FIXJUDGEWIDGET_H
