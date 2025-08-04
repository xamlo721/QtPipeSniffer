#ifndef PIPESNIFFER_H
#define PIPESNIFFER_H

#include <QApplication>
#include "PipeSocket.h"

class PipeSniffer : public QApplication {

    Q_OBJECT

    private:
        PipeSocket m_pipeSocket_src;
        PipeSocket m_pipeSocket_dst;

        bool connectFlag = false;

    public:
        PipeSniffer(int argc, char *argv[]);


    private:
        void onPipeReadData(const QByteArray &data, bool isSrcSide);


        /**
         * @brief splitHexByte - метод разделения 16-тиричной строки по байтам
         * @param str - 16-тиричная строка
         * @return Разделенная по байтам 16-тиричная строка
         */
        QString splitHexByte(QString &str) const;

    signals:
        void signalOnStatusHandled(const QString text);

        void signalManageButtonState(const bool enable);

        void signalManageButtonText(const QString text);

        void signalOnDataRecieved(const QString & data);

    public slots:
        void slotOnPipeConnect(QString pipeName_src, QString pipeName_dst);

    private slots:

        /**
         * @brief slotOnManageButtonDisableTimeout - слот для обработки таймаута выключения кнопки управления соединением
         */
        void slotOnManageButtonDisableTimeout();

        void slotOnSrcPipeReadData(const QByteArray &data);
        void slotOnDstPipeReadData(const QByteArray &data);


};

#endif // PIPESNIFFER_H
