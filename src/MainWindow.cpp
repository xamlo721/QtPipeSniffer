#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QObject::connect(this->ui->PipeConnectPushButton, &QPushButton::pressed, this, &MainWindow::slotOnConnectButtonPressed);
    QObject::connect(this->ui->ClearPushButton, &QPushButton::pressed, this, &MainWindow::slotOnClearButtonPressed);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::displaySnifferData(const QByteArray &data) {
    this->ui->PipeSnifferEextEdit->append(QString(data));
}

void MainWindow::setConnectStatus(bool status) {
    this->ui->PipeStatusLabel->setText(status ? "Connected" : "Failed");
}

void MainWindow::slotOnConnectButtonPressed() {
    QString pipeName = this->ui->PipeNameLineEdit->text();

    emit signalOnPipeConnectRequest(pipeName);
}

void MainWindow::slotOnClearButtonPressed() {
    this->ui->PipeSnifferEextEdit->clear();
}

