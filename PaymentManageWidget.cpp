#include "PaymentManageWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSqlQuery>
#include <QMessageBox>
#include <QHeaderView>
#include <QDate>
#include<QSqlError>

PaymentManageWidget::PaymentManageWidget(int ownerId, Data *db, QWidget *parent)
    : QWidget(parent),
      m_ownerId(ownerId),
      m_db(db)
{

    m_recordsTable = new QTableWidget(0, 5, this);
    m_recordsTable->setHorizontalHeaderLabels({"缴费ID", "费用类型", "金额", "缴费时间", "状态"});
    m_recordsTable->horizontalHeader()->setStretchLastSection(true);
    m_recordsTable->setColumnWidth(0, 80);
    m_recordsTable->setColumnWidth(1, 120);
    m_recordsTable->setColumnWidth(2, 80);
    m_recordsTable->setColumnWidth(3, 150);


    m_feeTypeCombo = new QComboBox();
    m_feeTypeCombo->addItem("物业费", 0);
    m_feeTypeCombo->addItem("停车费", 1);

    m_startDateEdit = new QDateEdit(QDate::currentDate().addMonths(-1));
    m_startDateEdit->setCalendarPopup(true);
    m_startDateEdit->setDisplayFormat("yyyy-MM-dd");

    m_endDateEdit = new QDateEdit(QDate::currentDate());
    m_endDateEdit->setCalendarPopup(true);
    m_endDateEdit->setDisplayFormat("yyyy-MM-dd");

    m_amountEdit = new QLineEdit();
    m_amountEdit->setValidator(new QDoubleValidator(0, 99999, 2, this));
    m_totalEdit = new QLineEdit();
    m_totalEdit->setReadOnly(true);
    m_totalEdit->setStyleSheet("background-color: #f0f0f0;");

    m_payBtn = new QPushButton("确认缴费", this);
    m_payBtn->setEnabled(false);


    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("费用类型：", m_feeTypeCombo);
    formLayout->addRow("缴费周期：", m_startDateEdit);
    formLayout->addRow("至：", m_endDateEdit);
    formLayout->addRow("金额：", m_amountEdit);
    formLayout->addRow("总计：", m_totalEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *refreshBtn = new QPushButton("刷新记录", this);
    btnLayout->addStretch();
    btnLayout->addWidget(refreshBtn);
    btnLayout->addWidget(m_payBtn);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_recordsTable);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);


    connect(m_payBtn, &QPushButton::clicked, this, &PaymentManageWidget::submitPayment);
    connect(refreshBtn, &QPushButton::clicked, this, &PaymentManageWidget::refreshPaymentRecords);
    connect(m_amountEdit, &QLineEdit::textChanged, this, &PaymentManageWidget::calculateTotalAmount);


    refreshPaymentRecords();
}


void PaymentManageWidget::refreshPaymentRecords()
{
    m_recordsTable->setRowCount(0);


    QString sql = QString("SELECT id, type, amount, pay_time, status "
                         "FROM Payments WHERE owner_id = %1 "
                         "ORDER BY pay_time DESC")
                         .arg(m_ownerId);
    QSqlQuery query = m_db->query(sql);

    while (query.next()) {
        int row = m_recordsTable->rowCount();
        m_recordsTable->insertRow(row);


        m_recordsTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));


        QString typeText = query.value(1).toInt() == 0 ? "物业费" : "停车费";
        m_recordsTable->setItem(row, 1, new QTableWidgetItem(typeText));


        m_recordsTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));


        m_recordsTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));


        QString statusText = query.value(4).toInt() == 0 ? "未支付" : "已支付";
        m_recordsTable->setItem(row, 4, new QTableWidgetItem(statusText));
    }
}


void PaymentManageWidget::submitPayment()
{
    double amount = m_amountEdit->text().toDouble();
    if (amount <= 0) {
        QMessageBox::warning(this, "错误", "请输入有效的缴费金额");
        return;
    }

    int feeType = m_feeTypeCombo->currentData().toInt();
    QString startDate = m_startDateEdit->date().toString("yyyy-MM-dd");
    QString endDate = m_endDateEdit->date().toString("yyyy-MM-dd");


    QString sql = QString("INSERT INTO Payments "
                         "(owner_id, type, amount, pay_time, status, remark) "
                         "VALUES (%1, %2, %3, datetime('now'), 1, '缴费周期：%4-%5')")
                         .arg(m_ownerId)
                         .arg(feeType)
                         .arg(amount)
                         .arg(startDate)
                         .arg(endDate);

    QSqlQuery query = m_db->query(sql);
    if (query.isActive()) {
        QMessageBox::information(this, "成功", "缴费成功！");
        emit paymentFinished(true, "缴费成功");
        refreshPaymentRecords();
        m_amountEdit->clear();
    } else {
        QMessageBox::critical(this, "失败", "缴费失败：" + query.lastError().text());
        emit paymentFinished(false, "缴费失败：" + query.lastError().text());
    }
}


void PaymentManageWidget::calculateTotalAmount()
{
    double amount = m_amountEdit->text().toDouble();
    m_totalEdit->setText(QString::number(amount, 'f', 2));
    m_payBtn->setEnabled(amount > 0);
}
