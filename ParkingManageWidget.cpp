#include "C:\Users\zhang\Desktop\oppo\parkingmanagewidget.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSqlQuery>
#include <QMessageBox>
#include <QHeaderView>
#include <QSqlError>
#include <QGroupBox>

ParkingManageWidget::ParkingManageWidget(int ownerId, Data *db, QWidget *parent)
    : QWidget(parent),
      m_ownerId(ownerId),
      m_db(db)
{


    // 参考设计风格：浅色系、卡片化、简洁现代
    this->setStyleSheet(R"(
        /* 全局基础样式 */
        QWidget {
            font-family: "Microsoft YaHei", sans-serif;
            background-color: #F5F7FA; /* 浅灰背景，贴近参考设计 */
            color: #4B5563; /* 深灰文字 */
        }

        /* 主窗口布局优化（若需要） */
        QWidget#ownerFixCommentWidget {
            padding: 20px; /* 与边缘保持间距 */
        }

        /* 表格容器：卡片化设计 */
        QTableWidget {
            background-color: #FFFFFF; /* 白色卡片背景 */
            border: none;
            border-radius: 8px; /* 圆角卡片 */
            box-shadow: 0 2px 6px rgba(0, 0, 0, 0.05); /* 柔和阴影 */
            gridline-color: #E5E7EB; /* 表格分隔线浅灰 */
            alternate-background-color: #FAFAFA; /* 交替行浅灰 */
        }

        /* 表格表头（水平 + 垂直） */
        QTableWidget::horizontalHeader,
        QTableWidget::verticalHeader {
            background-color: #F9FAFB; /* 表头浅灰背景 */
            color: #6B7280; /* 表头文字色 */
            font-weight: 500; /* 加粗突出 */
            border: none;
        }

        /* 水平表头单元格 */
        QTableWidget::horizontalHeader::section {
            border-bottom: 1px solid #E5E7EB; /* 底部分隔线 */
            padding: 8px;
            text-align: center;
            border-radius: 0; /* 与表格整体圆角适配 */
        }

        /* 垂直表头（左侧序号列） */
        QTableWidget::verticalHeader::section {
            border-right: 1px solid #E5E7EB; /* 右侧分隔线 */
            padding: 8px;
            text-align: center;
            background-color: #F9FAFB; /* 与水平表头一致 */
        }

        /* 表格内容单元格 */
        QTableWidget::item {
            border: none;
            padding: 8px;
            color: #4B5563;
        }

        /* 单元格选中态（柔和反馈） */
        QTableWidget::item:selected {
            background-color: #E5F1FF; /* 浅蓝选中色，贴近参考设计的交互感 */
            color: #2B6CB0; /* 深一点的蓝色文字 */
        }

        /* 按钮：主操作按钮（参考右侧的"Go to Chapter"风格） */
        QPushButton {
            background-color: #3B82F6; /* 参考设计的主蓝色 */
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 8px 16px;
            font-weight: 500;
            cursor: pointer;
            transition: all 0.2s ease;
        }

        /* 按钮悬停态 */
        QPushButton:hover {
            background-color: #2563EB; /* 更深的蓝色 */
        }

        /* 按钮按下态 */
        QPushButton:pressed {
            background-color: #1D4ED8; /* 强调按下反馈 */
        }

        /* 输入框：参考搜索框/表单风格 */
        QLineEdit, QTextEdit {
            background-color: #FFFFFF;
            border: 1px solid #E5E7EB;
            border-radius: 6px;
            padding: 8px 12px;
            color: #4B5563;
            transition: border-color 0.2s ease;
        }

        /* 输入框聚焦态 */
        QLineEdit:focus, QTextEdit:focus {
            border-color: #3B82F6; /* 主蓝色高亮 */
            outline: none;
        }

        /* 标签：标题/说明文字 */
        QLabel {
            color: #4B5563;
            font-weight: 500; /* 标题加粗 */
        }

        /* 布局容器：模拟卡片内的分组（若有） */
        QGroupBox {
            background-color: #FFFFFF;
            border: 1px solid #E5E7EB;
            border-radius: 8px;
            margin-top: 16px;
            padding: 12px;
        }

        /* 分组框标题 */
        QGroupBox::title {
            color: #6B7280;
            font-weight: 500;
            padding: 0 8px; /* 与边框间距 */
        }

        /* 滚动条（柔和风格） */
        QScrollBar:vertical {
            background-color: #F1F2F6;
            width: 8px;
            border-radius: 4px;
        }

        QScrollBar::handle:vertical {
            background-color: #D1D5DB;
            border-radius: 4px;
            min-height: 20px;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0; /* 隐藏默认箭头 */
        }

        /* 消息框：适配整体风格 */
        QMessageBox {
            background-color: #FFFFFF;
            border: 1px solid #E5E7EB;
            border-radius: 8px;
        }

        QMessageBox QLabel {
            color: #4B5563;
        }

        QMessageBox QPushButton {
            background-color: #3B82F6;
            color: #FFFFFF;
            border: none;
            border-radius: 6px;
            padding: 6px 12px;
        }
    )");
    m_statusTable = new QTableWidget(0, 4, this);
    m_statusTable->setHorizontalHeaderLabels({"车位ID", "位置", "状态", "价格"});
    m_statusTable->horizontalHeader()->setStretchLastSection(true);
    m_statusTable->setColumnWidth(0, 80);
     m_statusTable->setMinimumHeight(200);  // 最小高度200像素，可根据需求改


    m_carportCombo = new QComboBox();
    m_reasonEdit = new QLineEdit();
    m_carIdEdit = new QLineEdit();
    m_reasonEdit->setPlaceholderText("请输入申请原因（如：日常使用）");
    m_carIdEdit->setPlaceholderText("请输入车牌号");
    m_applyBtn = new QPushButton("提交申请", this);
    m_applyBtn->setEnabled(false);



    m_tableHistory = new QTableWidget(0, 6, this);
    m_tableHistory->setHorizontalHeaderLabels({"申请ID", "车位ID", "位置", "申请时间", "状态","车牌号"});
    m_tableHistory->horizontalHeader()->setStretchLastSection(true);
    m_tableHistory->setColumnWidth(0, 80);
    m_tableHistory->setColumnWidth(1, 80);
    m_tableHistory->setColumnWidth(2, 120);
    m_tableHistory->setColumnWidth(3, 150);

    m_tableHistory->setColumnWidth(5,100);
    m_tableHistory->setMinimumHeight(200);


    QVBoxLayout *mainLayout = new QVBoxLayout(this);


    QGroupBox *historyGroupBox = new QGroupBox("申请历史", this);
    QVBoxLayout *historyLayout = new QVBoxLayout(historyGroupBox);
    historyLayout->addWidget(m_tableHistory);
    mainLayout->addWidget(historyGroupBox);

    QGroupBox *statusGroupBox = new QGroupBox("车位状态", this);
    QVBoxLayout *statusLayout = new QVBoxLayout(statusGroupBox);
    statusLayout->addWidget(m_statusTable);
    mainLayout->addWidget(statusGroupBox);


    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow("选择可用车位：", m_carportCombo);
    formLayout->addRow("申请原因：", m_reasonEdit);

    formLayout->addRow("车牌号：",m_carIdEdit);
    formLayout->addRow(m_applyBtn);
    mainLayout->addLayout(formLayout);

    setLayout(mainLayout);


    connect(m_applyBtn, &QPushButton::clicked, this, &ParkingManageWidget::submitParkingApply);
    connect(m_carportCombo, &QComboBox::currentTextChanged, this, &ParkingManageWidget::onCarportComboChanged);


    refreshParkingStatus();
    refreshApplicationHistory();
}

void ParkingManageWidget::refreshParkingStatus()
{
    m_statusTable->setRowCount(0);
    m_carportCombo->clear();


    QSqlQuery query = m_db->query("SELECT id, location, status, charge FROM Carports");
    while (query.next()) {
        int row = m_statusTable->rowCount();
        m_statusTable->insertRow(row);

        m_statusTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        m_statusTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));

        int status = query.value(2).toInt();
        QString statusText = (status == 0) ? "未占用" : "已占用";
        m_statusTable->setItem(row, 2, new QTableWidgetItem(statusText));


        QString chargeText = query.value(3).toString();
        m_statusTable->setItem(row, 3, new QTableWidgetItem(chargeText));

        if (status == 0) {

            m_carportCombo->addItem(
                QString("%1（ID: %2 / 价格: %3）").arg(query.value(1).toString()).arg(query.value(0).toString()).arg(chargeText),
                query.value(0).toInt()
            );
        }
    }

    m_applyBtn->setEnabled(m_carportCombo->count() > 0);
}

void ParkingManageWidget::submitParkingApply()
{
    int carportId = m_carportCombo->currentData().toInt();
    QString reason = m_reasonEdit->text().trimmed();
    QString carId = m_carIdEdit->text().trimmed();

    if (reason.isEmpty()) {
        QMessageBox::warning(this, "错误", "请填写申请原因");
        return;
    }
    if(carId.isEmpty()){
        QMessageBox::warning(this,"错误","请填写车牌号");
        return;
    }

    m_db->query("BEGIN TRANSACTION");

    QString updateSql = QString("UPDATE Carports "
                                "SET status = 1 "
                                "WHERE id = %1")
                            .arg(carportId);
    QSqlQuery updateQuery = m_db->query(updateSql);
    if (!updateQuery.isActive()) {
        m_db->query("ROLLBACK");
        QMessageBox::critical(this, "失败", "标记车位为待审批失败：" + updateQuery.lastError().text());
        return;
    }

    QString insertSql = QString("INSERT INTO ParkingApplications "
                                "(owner_id, carport_id, reason, status, apply_time,car_id) "
                                "VALUES (%1, %2, '%3', 0, datetime('now'),'%4')")
                            .arg(m_ownerId)
                            .arg(carportId)
                            .arg(reason)
                            .arg(carId);
    QSqlQuery insertQuery = m_db->query(insertSql);

    if (insertQuery.isActive()) {
        m_db->query("COMMIT");
        QMessageBox::information(this, "成功", "车位申请已提交，等待管理员审批");
        emit applyFinished(true, "申请提交成功（待审批）");
        refreshParkingStatus();
        refreshApplicationHistory();
        m_reasonEdit->clear();
        m_carIdEdit->clear();
    } else {
        m_db->query("ROLLBACK");
        QMessageBox::critical(this, "失败", "申请提交失败：" + insertQuery.lastError().text());
        emit applyFinished(false, "申请提交失败：" + insertQuery.lastError().text());
    }
}

void ParkingManageWidget::onCarportComboChanged()
{
    m_applyBtn->setEnabled(m_carportCombo->currentIndex() != -1);
}

void ParkingManageWidget::refreshApplicationHistory()
{
    m_tableHistory->setRowCount(0);

    QString sql = QString("SELECT pa.id, pa.carport_id, c.location, pa.apply_time, pa.status,pa.car_id "
                         "FROM ParkingApplications pa "
                         "JOIN Carports c ON pa.carport_id = c.id "
                         "WHERE pa.owner_id = %1 "
                         "ORDER BY pa.apply_time DESC")
                         .arg(m_ownerId);
    QSqlQuery query = m_db->query(sql);

    while (query.next()) {
        int row = m_tableHistory->rowCount();
        m_tableHistory->insertRow(row);

        m_tableHistory->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        m_tableHistory->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        m_tableHistory->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
        m_tableHistory->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));

        QString statusText;
        int status = query.value(4).toInt();
        switch (status) {
            case 0: statusText = "待审批"; break;
            case 1: statusText = "已批准"; break;
            case 2: statusText = "已拒绝"; break;
            default: statusText = "未知";
        }
        m_tableHistory->setItem(row, 4, new QTableWidgetItem(statusText));
        m_tableHistory->setItem(row,5,new QTableWidgetItem(query.value(5).toString()));
    }
}

