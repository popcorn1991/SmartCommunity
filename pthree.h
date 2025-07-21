#ifndef PTHREE_H
#define PTHREE_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableView>
#include <string>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRelationalTableModel>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "data.h"


namespace Ui {
class PTHREE;
}

class PTHREE : public QMainWindow
{
    Q_OBJECT

public:
    explicit PTHREE(QWidget *parent = nullptr);
    ~PTHREE();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::PTHREE *ui;
    Data db;
};

#endif // PTHREE_H
