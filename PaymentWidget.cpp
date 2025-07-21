#include "PaymentWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QSqlQuery>
#include <QMessageBox>
#include <QHeaderView>
#include <QDate>
#include <QSqlError>
#include<QLabel>

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


    m_filterCombo = new QComboBox(this);
    m_filterCombo->addItems({"全部订单", "待缴费", "已支付"});
    m_filterCombo->setCurrentIndex(0);


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

    m_payBtn = new QPushButton("主动缴费", this);
    m_payBtn->setEnabled(false);

    m_payOrderBtn = new QPushButton("支付选中订单", this);
    m_payOrderBtn->setEnabled(false);


    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("筛选：", this));
    filterLayout->addWidget(m_filterCombo);
    filterLayout->addStretch();

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
    btnLayout->addWidget(m_payOrderBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(m_recordsTable);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(btnLayout);

    setLayout(mainLayout);


    connect(m_payBtn, &QPushButton::clicked, this, &PaymentManageWidget::submitManualPayment);
    connect(refreshBtn, &QPushButton::clicked, this, &PaymentManageWidget::refreshAllPayments);
    connect(m_amountEdit, &QLineEdit::textChanged, this, &PaymentManageWidget::calculateTotal);
    connect(m_payOrderBtn, &QPushButton::clicked, this, &PaymentManageWidget::payPendingOrder);
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PaymentManageWidget::filterPayments);
    connect(m_recordsTable, &QTableWidget::itemSelectionChanged, [this]() {

        bool hasPendingSelection = false;
        QList<QTableWidgetItem*> selectedItems = m_recordsTable->selectedItems();
        for (QTableWidgetItem *item : selectedItems) {
            if (item->column() == 4 && item->text() == "待缴费") {
                hasPendingSelection = true;
                break;
            }
        }
        m_payOrderBtn->setEnabled(hasPendingSelection);
    });

    QString styleSheet = R"(
           /* 窗口背景 */
           QWidget#PaymentManageWidget {
               background-color: #f5f7fa;
           }

           /* 表格整体样式 */
           QTableWidget {
               background-color: white;
               border-radius: 8px;
               border: none;
               gridline-color: #e0e0e0;
               font-family: "Microsoft YaHei", sans-serif;
               font-size: 30px;
               color: #333;
           }

           /* 表头样式 */
           QHeaderView::section {
               background-color: #f0f2f5;
               padding: 8px;
               border: none;
               border-bottom: 1px solid #e0e0e0;
               font-weight: bold;
               font-size: 30px;
               color: #555;
           }

           /* 表格单元格样式 */
           QTableWidget::item {
               padding: 8px;
               border: none;
           }

           /* 选中行样式 */
           QTableWidget::item:selected {
               background-color: #e0efff;
               color: #0056b3;
           }

           /* 组合框样式 */
           QComboBox {
               background-color: white;
               border: 1px solid #e0e0e0;
               border-radius: 4px;
               padding: 6px 8px;
               font-size: 30px;
               color: #333;
           }
           QComboBox:hover {
               border-color: #c0c0c0;
           }

           /* 日期编辑框样式 */
           QDateEdit {
               background-color: white;
               border: 1px solid #e0e0e0;
               border-radius: 4px;
               padding: 6px 8px;
               font-size: 30px;
               color: #333;
           }
           QDateEdit:hover {
               border-color: #c0c0c0;
           }

           /* 输入框样式 */
           QLineEdit {
               background-color: white;
               border: 1px solid #e0e0e0;
               border-radius: 4px;
               padding: 6px 8px;
               font-size: 30px;
               color: #333;
           }
           QLineEdit:hover {
               border-color: #c0c0c0;
           }
           QLineEdit:focus {
               border-color: #66b2ff;
               outline: none;
           }

           /* 按钮样式 */
           QPushButton {
               background-color: #66b2ff;
               color: white;
               border-radius: 4px;
               padding: 8px 16px;
               font-weight: bold;
               border: none;
               margin: 0 5px;
           }
           QPushButton:hover {
               background-color: #4da6ff;
           }
           QPushButton:pressed {
               background-color: #3990ff;
           }

           /* 标签样式 */
           QLabel {
               font-size: 30px;
               color: #555;
               padding: 5px 0;
           }
       )";
       // 设置 objectName，让样式表能匹配到当前窗口
       this->setObjectName("PaymentManageWidget");
       this->setStyleSheet(styleSheet);
    m_recordsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_recordsTable->horizontalHeader()->setStretchLastSection(true);
    m_recordsTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    refreshAllPayments();
}


void PaymentManageWidget::refreshAllPayments()
{
    m_recordsTable->setRowCount(0);


    QString statusFilter = "";
    int filterIndex = m_filterCombo->currentIndex();
    if (filterIndex == 1) statusFilter = " AND status = 0";
    if (filterIndex == 2) statusFilter = " AND status = 1";

    QString sql = QString("SELECT id, type, amount, pay_time, status "
                         "FROM Payments WHERE owner_id = %1 %2 "
                         "ORDER BY pay_time DESC")
                         .arg(m_ownerId)
                         .arg(statusFilter);

    QSqlQuery query = m_db->query(sql);

    while (query.next()) {
        int row = m_recordsTable->rowCount();
        m_recordsTable->insertRow(row);

        m_recordsTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));

        QString typeText = query.value(1).toString();
        m_recordsTable->setItem(row, 1, new QTableWidgetItem(typeText));

        m_recordsTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));

        m_recordsTable->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));

        QString statusText = query.value(4).toInt() == 0 ? "待缴费" : "已支付";
        QTableWidgetItem *statusItem = new QTableWidgetItem(statusText);

        // 为不同状态设置不同颜色
        if (statusText == "待缴费") {
            statusItem->setBackgroundColor(QColor(255, 230, 230)); // 浅红色
        } else {
            statusItem->setBackgroundColor(QColor(230, 255, 230)); // 浅绿色
        }

        m_recordsTable->setItem(row, 4, statusItem);
    }
}

// ========== 主动缴费（原有功能） ==========
void PaymentManageWidget::submitManualPayment()
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
                         "VALUES (%1, %2, %3, datetime('now'), 1, '业主主动缴费：%4-%5')")
                         .arg(m_ownerId)
                         .arg(feeType)
                         .arg(amount)
                         .arg(startDate)
                         .arg(endDate);

    QSqlQuery query = m_db->query(sql);
    if (query.isActive()) {
        QMessageBox::information(this, "成功", "缴费成功！");
        emit paymentFinished(true, "缴费成功");
        refreshAllPayments();
        m_amountEdit->clear();
    } else {
        QMessageBox::critical(this, "失败", "缴费失败：" + query.lastError().text());
        emit paymentFinished(false, "缴费失败：" + query.lastError().text());
    }
}


void PaymentManageWidget::payPendingOrder()
{
    QList<QTableWidgetItem*> selectedItems = m_recordsTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择要支付的订单！");
        return;
    }

    int row = m_recordsTable->currentRow();
    int paymentId = m_recordsTable->item(row, 0)->text().toInt();
    QString statusText = m_recordsTable->item(row, 4)->text();

    if (statusText != "待缴费") {
        QMessageBox::warning(this, "提示", "只能支付状态为『待缴费』的订单！");
        return;
    }


    QString amount = m_recordsTable->item(row, 2)->text();
    QString type = m_recordsTable->item(row, 1)->text();
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "确认支付",
        QString("确定要支付以下订单吗？\n\n费用类型：%1\n金额：%2元")
        .arg(type).arg(amount),
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply != QMessageBox::Yes) return;

    QString sql = QString("UPDATE Payments "
                         "SET status = 1, pay_time = datetime('now') "
                         "WHERE id = %1 AND owner_id = %2")
                         .arg(paymentId)
                         .arg(m_ownerId);

    QSqlQuery query = m_db->query(sql);
    if (query.isActive()) {
        QMessageBox::information(this, "成功", "支付成功！");
        emit paymentFinished(true, "支付成功");
        refreshAllPayments();
    } else {
        QMessageBox::critical(this, "失败", "支付失败：" + query.lastError().text());
        emit paymentFinished(false, "支付失败：" + query.lastError().text());
    }
}


void PaymentManageWidget::calculateTotal()
{
    double amount = m_amountEdit->text().toDouble();
    m_totalEdit->setText(QString::number(amount, 'f', 2));
    m_payBtn->setEnabled(amount > 0);
}


void PaymentManageWidget::filterPayments(int index)
{
    Q_UNUSED(index);
    refreshAllPayments();
}
