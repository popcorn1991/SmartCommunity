#ifndef PAYMENTMANAGEWIDGET_H
#define PAYMENTMANAGEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QDateEdit>
#include <QLineEdit>
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
    void refreshAllPayments();
    void submitManualPayment();
    void payPendingOrder();
    void calculateTotal();
    void filterPayments(int index);

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
    QPushButton *m_payOrderBtn;
    QComboBox *m_filterCombo;
};

#endif // PAYMENTMANAGEWIDGET_H
