#ifndef STAFFPAGE13_H
#define STAFFPAGE13_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>

namespace Ui {
class StaffPage13;
}

class StaffPage13 : public QMainWindow
{
    Q_OBJECT

public:
    explicit StaffPage13(QWidget *parent = nullptr);
    ~StaffPage13();



private slots:
    void on_btn_checkIn_clicked();
    void on_refreshTable_clicked();

private:
    Ui::StaffPage13 *ui;
    QSqlTableModel * model;
};

#endif
