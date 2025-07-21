#ifndef PARKINGMANAGEWIDGET_H
#define PARKINGMANAGEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include "data.h"

class ParkingManageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ParkingManageWidget(int ownerId, Data *db, QWidget *parent = nullptr);

signals:
    void applyFinished(bool success, const QString &message);


private slots:
    void refreshParkingStatus();
    void submitParkingApply();
    void onCarportComboChanged();
    void  refreshApplicationHistory();

private:
    int m_ownerId;
    Data *m_db;


    QTableWidget *m_statusTable;
    QComboBox *m_carportCombo;
    QLineEdit *m_reasonEdit;
    QPushButton *m_applyBtn;
    QTableWidget *m_tableHistory;
    QLineEdit *m_carIdEdit;


};

#endif // PARKINGMANAGEWIDGET_H
