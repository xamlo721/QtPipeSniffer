#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QIcon>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/icons/pipe32.png"));

    QObject::connect(this->ui->PipeConnectPushButton, &QPushButton::pressed, this, &MainWindow::slotOnConnectButtonPressed);
    QObject::connect(this->ui->ClearPushButton, &QPushButton::pressed, this, &MainWindow::slotOnClearButtonPressed);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::displaySnifferData(const QString & data) {
    this->ui->PipeSnifferEextEdit->append(data);
}

void MainWindow::setConnectStatus(const QString text) {
    QString resText = "Pipe Status: ";
    resText += text;
    this->ui->PipeStatusLabel->setText(resText);
}

void MainWindow::setManageButtonState(const bool state) {
    state ? this->ui->PipeConnectPushButton->setEnabled(true) : this->ui->PipeConnectPushButton->setDisabled(true);
}

void MainWindow::setManageButtonText(const QString text) {
    this->ui->PipeConnectPushButton->setText(text);
}

void MainWindow::slotOnConnectButtonPressed() {
    QString pipeNameSrc = this->ui->PipeNameSrcLineEdit->text();
    QString pipeNameDst = this->ui->PipeNameDstLineEdit->text();

    emit signalOnPipeConnectRequest(pipeNameSrc, pipeNameDst);
}

void MainWindow::slotOnClearButtonPressed() {
    this->ui->PipeSnifferEextEdit->clear();
}

