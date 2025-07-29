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

    public slots:

        void displaySnifferData(const QString & data);

        void setConnectStatus(const QString text);

        void setManageButtonState(const bool state);

        void setManageButtonText(const QString text);

    private slots:

        void slotOnConnectButtonPressed();

        void slotOnClearButtonPressed();

    signals:

        void signalOnPipeConnectRequest(QString pipeName_src, QString pipeName_dst);

};
#endif // MAINWINDOW_H
