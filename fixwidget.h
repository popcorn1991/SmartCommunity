#ifndef FIXWIDGET_H
#define FIXWIDGET_H

#include <QWidget>

namespace Ui {
class fixWidget;
}

class fixWidget : public QWidget
{
    Q_OBJECT

public:
    explicit fixWidget(int id,QWidget *parent = nullptr);
    ~fixWidget();
    private slots:
    void onSubmit();

private:
    Ui::fixWidget *ui;
    int m_id;
};

#endif // FIXWIDGET_H
