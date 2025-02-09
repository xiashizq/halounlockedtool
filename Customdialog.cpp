
#include "Customdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>

CustomDialog::CustomDialog(const QString &message, QWidget *parent) :
    QDialog(parent),
    continueUnlock(false)
{
    setupUI(message);
}

void CustomDialog::setupUI(const QString &message) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QLabel *label = new QLabel(message, this);
    QPushButton *continueButton = new QPushButton("继续解锁", this);
    QPushButton *cancelButton = new QPushButton("取消", this);

    // 设置字体
    QFont font("Microsoft YaHei UI", 9);
    label->setFont(font);
    continueButton->setFont(font);
    cancelButton->setFont(font);

    // 设置按钮为左右布局
    buttonLayout->addWidget(continueButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addWidget(label);
    mainLayout->addLayout(buttonLayout);

    // 固定窗口大小
    setFixedSize(300, 90); // 根据需要调整宽度和高度

    connect(continueButton, &QPushButton::clicked, this, &CustomDialog::onContinueClicked);
    connect(cancelButton, &QPushButton::clicked, this, &CustomDialog::onCancelClicked);

    // 确保没有帮助按钮
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
}

bool CustomDialog::showCustomDialog(const QString &message, QWidget *parent) {
    CustomDialog dialog(message, parent);
    int ret = dialog.exec();
    return ret == QDialog::Accepted;
}

void CustomDialog::onContinueClicked() {
    continueUnlock = true;
    accept();
}

void CustomDialog::onCancelClicked() {
    continueUnlock = false;
    reject();
}
