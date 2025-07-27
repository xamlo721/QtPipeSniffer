#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {

    Q_OBJECT

    private:

        Ui::MainWindow *ui;

    public:

        MainWindow(QWidget *parent = nullptr);

        ~MainWindow();

        void displaySnifferData(const QByteArray &data);

        void setConnectStatus(bool status);

    private slots:

        void slotOnConnectButtonPressed();

        void slotOnClearButtonPressed();

    signals:

        void signalOnPipeConnectRequest(QString pipeName);

};
#endif // MAINWINDOW_H
