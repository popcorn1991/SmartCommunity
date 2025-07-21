#ifndef STAFFPAGE11_H
#define STAFFPAGE11_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableView>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QLineEdit>
#include <QPushButton>
#include "data.h"
#include <QWidget>
#include <QDebug>
#include <QSqlTableModel>
#include <string>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>

namespace Ui {
class StaffPage11;
}

class StaffPage11 : public QMainWindow
{
    Q_OBJECT

public:
    explicit StaffPage11(QWidget *parent = nullptr);
    ~StaffPage11();

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::StaffPage11 *ui;
//    Data db;

};

#endif






