#ifndef PAYMENTMANAGEWIDGET_H
#define PAYMENTMANAGEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include "data.h"

class PaymentManageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaymentManageWidget(int ownerId, Data *db, QWidget *parent = nullptr);

signals:
    void paymentFinished(bool success, const QString &message);
private slots:
    void refreshPaymentRecords();
    void submitPayment();
    void calculateTotalAmount();

private:
    int m_ownerId;
    Data *m_db;


    QTableWidget *m_recordsTable;
    QComboBox *m_feeTypeCombo;
    QDateEdit *m_startDateEdit;
    QDateEdit *m_endDateEdit;
    QLineEdit *m_amountEdit;
    QLineEdit *m_totalEdit;
    QPushButton *m_payBtn;

};

#endif // PAYMENTMANAGEWIDGET_H
