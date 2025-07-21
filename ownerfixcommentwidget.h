#ifndef OWNERFIXCOMMENTWIDGET_H
#define OWNERFIXCOMMENTWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>


namespace Ui {
class ownerFixCommentWidget;
}

class ownerFixCommentWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ownerFixCommentWidget(int id,QWidget *parent = nullptr);
    ~ownerFixCommentWidget();

private:
    Ui::ownerFixCommentWidget *ui;
    QTableWidget* table;
    QPushButton* btnSubmit;
    QLineEdit* scoreEdit;
    QLineEdit* commentEdit;
    QLabel* scoreLabel;
    QLabel* commentLabel;
    void refreshTable();
    void onSubmit();
    int m_id;
};

#endif // OWNERFIXCOMMENTWIDGET_H
