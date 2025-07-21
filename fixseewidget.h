#ifndef FIXSEEWIDGET_H
#define FIXSEEWIDGET_H

#include <QWidget>
#include "QTableWidget"
#include "data.h"
#include "QPushButton.h"
namespace Ui {
class FixSeeWidget;
}

class FixSeeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FixSeeWidget(int m_id,QWidget *parent = nullptr);
    ~FixSeeWidget();
    QTableWidget* table;
    void refreshTable();
    int m_id;
    QPushButton* btn;

private:
    Ui::FixSeeWidget *ui;
};

#endif // FIXSEEWIDGET_H
