#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CanInit(is_Custom_Config, string_CanInterface, is_Use_Can_Fd, data_Bitrate);

    connect(ui->radioBoxCustomConfig, &QRadioButton::toggled, this, &MainWindow::initCustomConfig);
    connect(ui->comboBoxCanName, &QComboBox::currentTextChanged, this, &MainWindow::setInterfacename);
    connect(ui->comboBoxCanDataBitrate, &QComboBox::currentTextChanged, this, &MainWindow::setDataBitrate);
    connect(ui->radioButtonUseCanFD, &QRadioButton::toggled, this, &MainWindow::isUseCanFD);

    connect(ui->buttonSend, &QPushButton::clicked, this, &MainWindow::sendCanMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initCustomConfig(void)
{
    is_Custom_Config = !is_Custom_Config;
    if(is_Custom_Config)
    {
        CanInit(is_Custom_Config, string_CanInterface, is_Use_Can_Fd, data_Bitrate);
    }
}

void MainWindow::setInterfacename(void)
{
    //Read interfacce name input from combo box
    string_CanInterface = ui->comboBoxCanName->currentText();

    CanInit(is_Custom_Config, string_CanInterface, is_Use_Can_Fd, data_Bitrate);
}

void MainWindow::setDataBitrate(void)
{
    //Read Data Bitrate from combo box
    if(ui->comboBoxCanDataBitrate->currentIndex() == 1)
    {
        data_Bitrate = 500000;
    }else if (ui->comboBoxCanDataBitrate->currentIndex() == 2)
    {
        data_Bitrate = 1000000;
    }

    CanInit(is_Custom_Config, string_CanInterface, is_Use_Can_Fd, data_Bitrate);
}

void MainWindow::isUseCanFD(void)
{
    is_Use_Can_Fd = !is_Use_Can_Fd;
    CanInit(is_Custom_Config, string_CanInterface, is_Use_Can_Fd, data_Bitrate);
}

void MainWindow::CanInit(const bool &is_Custom_Config, const QString &string_CanInterface,
                         const bool &is_Use_Can_Fd, const quint64 &data_Bitrate)
{
    ui->textReceiveIdMs->setReadOnly(true);
    ui->textReceivePayload->setReadOnly(true);
    if(!is_Custom_Config)
    {
        //unable custom config
        ui->comboBoxCanDataBitrate->setDisabled(true);
        ui->comboBoxCanName->setDisabled(true);
        ui->radioButtonUseCanFD->setDisabled(true);
    }
    else
    {
        ui->comboBoxCanDataBitrate->setDisabled(false);
        ui->comboBoxCanName->setDisabled(false);
        ui->radioButtonUseCanFD->setDisabled(false);
    }
    //open socket can
    QString errorString;
    canBusDevice = QCanBus::instance()->createDevice("socketcan", string_CanInterface, &errorString);
    if (!canBusDevice) {
        return;
    }

    //Init filter RX CAN
    QList<QCanBusDevice::Filter> filter_list;
    QCanBusDevice::Filter filter;
    // filter 1
    filter.frameId = 0x87u;
    filter.frameIdMask = 0x7FFu;
    filter.format = QCanBusDevice::Filter::MatchBaseFormat;
    filter.type = QCanBusFrame::DataFrame;
    filter_list.push_back(filter);
    // filter 2
    filter.frameId = 0x98u;
    filter_list.push_back(filter);
    // filter 3
    filter.frameId = 0x113u;
    filter_list.push_back(filter);

    //apply filter
    if(!is_Use_Can_Fd)
    {
        canBusDevice->setConfigurationParameter(QCanBusDevice::RawFilterKey, QVariant::fromValue(filter_list));
    }else
    {
        canBusDevice->setConfigurationParameter(QCanBusDevice::CanFdKey, QVariant::fromValue(filter_list));
    }

    //Connect device
    if (!canBusDevice->connectDevice()) {
        printf("Connect device error\n");
        return;
    }

    //Clear output or input buffer
    canBusDevice->clear(QCanBusDevice::AllDirections);

    //Handler after received data
    connect(canBusDevice, &QCanBusDevice::framesReceived, this, &MainWindow::onCanReceive);
}

void MainWindow::onCanReceive()
{
    canBusFrame_rx = canBusDevice->readFrame();
    can_id_rcv = canBusFrame_rx.frameId();
    can_payload_rcv = QString(canBusFrame_rx.payload().toHex());

    ui->textReceiveIdMs->setText("0x" + QString::number(can_id_rcv, 16));
    ui->textReceivePayload->setText("0x" + can_payload_rcv.toUpper());
}

void MainWindow::sendCanMessage(void)
{
    //Read CAN ID and payload was typed
    bool ok;
    //Convert Qstring to hex int
    can_Id_Send = ui->textSendIdMs->toPlainText().toInt(&ok, 16);
    //Convert Qstring to int hex to Qbyte array
    can_Payload_Send = QByteArray::fromHex(ui->textSendPayload->toPlainText().toLatin1());
    canBusFrame_tx.setFrameId(can_Id_Send);
    canBusFrame_tx.setPayload(can_Payload_Send);

    canBusDevice->writeFrame(canBusFrame_tx);
}


