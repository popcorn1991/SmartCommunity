#ifndef STAFFSUPERWIDGET_H
#define STAFFSUPERWIDGET_H
#include <QTableWidget>
#include <QWidget>
#include <QPushButton>

namespace Ui {
class staffSuperWidget;
}

class staffSuperWidget : public QWidget
{
    Q_OBJECT

public:
    explicit staffSuperWidget(QWidget *parent = nullptr);
    ~staffSuperWidget();
    QTableWidget* table;
    QPushButton* btnAdd;
    QPushButton* btnEdit;
    QPushButton* btnDelete;
    QPushButton* btnS;
    QLineEdit* edit;
    void refreshTable();
    void onAdd();
    void onEdit();
    void onDelete();
    void onS();


private:
    Ui::staffSuperWidget *ui;
};

#endif // STAFFSUPERWIDGET_H
