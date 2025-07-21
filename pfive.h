#ifndef PFIVE_H
#define PFIVE_H

#include <QMainWindow>

namespace Ui {
class PFIVE;
}

class PFIVE : public QMainWindow
{
    Q_OBJECT

public:
    explicit PFIVE(QWidget *parent = nullptr);
    ~PFIVE();
private slots:
    void on_pushButton_clicked();

private:
    Ui::PFIVE *ui;
};

#endif // PFIVE_H


