#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

    class MainWindow : public QMainWindow
{
    Q_OBJECT

        public:
                 MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    bool is_Custom_Config = false;
    bool is_Use_Can_Fd = false;

    quint32 can_Id_Send;
    QByteArray can_Payload_Send;
    quint32 can_id_rcv;
    QString can_payload_rcv;
    QCanBusFrame canBusFrame_tx;
    QCanBusFrame canBusFrame_rx;
    QCanBusDevice *canBusDevice;
    QString string_CanInterface = "can0";
    quint64 data_Bitrate = 500000;

    void CanInit(const bool &is_Custom_Config, const QString &string_CanInterface,
                         const bool &is_Use_Can_Fd, const quint64 &data_Bitrate);
    void isUseCanFD(void);
    void setInterfacename(void);
    void setDataBitrate(void);
    void initCustomConfig(void);
    void onCanReceive(void);
    void sendCanMessage(void);
};
#endif // MAINWINDOW_H
