#include "bldcinterface.h"

namespace {
void stepTowards(double &value, double goal, double step) {
    if (value < goal) {
        if ((value + step) < goal) {
            value += step;
        } else {
            value = goal;
        }
    } else if (value > goal) {
        if ((value - step) > goal) {
            value -= step;
        } else {
            value = goal;
        }
    }
}
}

BLDCInterface::BLDCInterface(QObject *parent) :
    QObject(parent)
{
    m_mcconf = new McConfiguration(this);
    m_appconf = new AppConfiguration(this);

    refreshSerialDevices();
    set_udpIp("192.168.1.118");

    // Compatible firmwares
    mFwVersionReceived = false;
    mFwRetries = 0;
    mCompatibleFws.append(qMakePair(2, 15));

    QString supportedFWs;
    for (int i = 0;i < mCompatibleFws.size();i++) {
        QString tmp;
        tmp.sprintf("%d.%d", mCompatibleFws.at(i).first, mCompatibleFws.at(i).second);
        if (i < (mCompatibleFws.size() - 1)) {
            tmp.append("\n");
        }
        supportedFWs.append(tmp);
    }
    update_firmwareSupported(supportedFWs);

    mSerialPort = new QSerialPort(this);

    mTimer = new QTimer(this);
    mTimer->setInterval(20);
    mTimer->start();

    mSampleInt = 0;
    mDoReplot = false;
    mDoRescale = true;
    mDoFilterReplot = true;
    mPacketInterface = new PacketInterface(this);
    mDetectRes.updated = false;

    connect(mSerialPort, SIGNAL(readyRead()),
            this, SLOT(serialDataAvailable()));
    connect(mSerialPort, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(serialPortError(QSerialPort::SerialPortError)));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));

    connect(mPacketInterface, SIGNAL(dataToSend(QByteArray&)),
            this, SLOT(packetDataToSend(QByteArray&)));
    connect(mPacketInterface, SIGNAL(fwVersionReceived(int,int)),
            this, SLOT(fwVersionReceived(int,int)));
    connect(mPacketInterface, SIGNAL(ackReceived(QString)),
            this, SIGNAL(ackReceived(QString)));
    connect(mPacketInterface, SIGNAL(valuesReceived(MC_VALUES)),
            this, SIGNAL(mcValuesReceived(MC_VALUES)));
    connect(mPacketInterface, SIGNAL(printReceived(QString)),
            this, SLOT(printReceived(QString)));
    connect(mPacketInterface, SIGNAL(samplesReceived(QByteArray)),
            this, SLOT(samplesReceived(QByteArray)));
    connect(mPacketInterface, SIGNAL(rotorPosReceived(double)),
            this, SIGNAL(rotorPosReceived(double)));
    connect(mPacketInterface, SIGNAL(experimentSamplesReceived(QVector<double>)),
            this, SLOT(experimentSamplesReceived(QVector<double>)));
    connect(mPacketInterface, SIGNAL(mcconfReceived(mc_configuration)),
            this, SLOT(mcconfReceived(mc_configuration)));
    connect(mPacketInterface, SIGNAL(motorParamReceived(double,double,QVector<int>,int)),
            this, SLOT(motorParamReceived(double,double,QVector<int>,int)));
    connect(mPacketInterface, SIGNAL(motorRLReceived(double,double)),
            this, SLOT(motorRLReceived(double,double)));
    connect(mPacketInterface, SIGNAL(motorLinkageReceived(double)),
            this, SLOT(motorLinkageReceived(double)));
    connect(mPacketInterface, SIGNAL(encoderParamReceived(double,double,bool)),
            this, SLOT(encoderParamReceived(double,double,bool)));
    connect(mPacketInterface, SIGNAL(focHallTableReceived(QVector<int>,int)),
            this, SLOT(focHallTableReceived(QVector<int>,int)));
    connect(mPacketInterface, SIGNAL(appconfReceived(app_configuration)),
            this, SLOT(appconfReceived(app_configuration)));
    connect(mPacketInterface, SIGNAL(decodedPpmReceived(double,double)),
            this, SLOT(decodedPpmReceived(double,double)));
    connect(mPacketInterface, SIGNAL(decodedAdcReceived(double,double,double,double)),
            this, SLOT(decodedAdcReceived(double,double,double,double)));
    connect(mPacketInterface, SIGNAL(decodedChukReceived(double)),
            this, SLOT(decodedChukReceived(double)));

    mSerialization = new Serialization(this);

}

void BLDCInterface::serialDataAvailable()
{
    while (mSerialPort->bytesAvailable() > 0) {
        QByteArray data = mSerialPort->readAll();
        mPacketInterface->processData(data);
    }
}

void BLDCInterface::serialPortError(QSerialPort::SerialPortError error)

{
    QString message;
    switch (error) {
    case QSerialPort::NoError:
        break;
    case QSerialPort::DeviceNotFoundError:
        message = tr("Device not found");
        break;
    case QSerialPort::OpenError:
        message = tr("Can't open device");
        break;
    case QSerialPort::NotOpenError:
        message = tr("Not open error");
        break;
    case QSerialPort::ResourceError:
        message = tr("Port disconnected");
        break;
    case QSerialPort::UnknownError:
        message = tr("Unknown error");
        break;
    default:
        message = "Serial port error: " + QString::number(error);
        break;
    }

    if(!message.isEmpty()) {
        emit statusInfoChanged(message, false);

        if(mSerialPort->isOpen()) {
            mSerialPort->close();
        }
    }
}

void BLDCInterface::timerSlot()
{
//@@ToDo: implement
}

void BLDCInterface::packetDataToSend(QByteArray &data)
{
    if (mSerialPort->isOpen()) {
        mSerialPort->write(data);
    }
}

void BLDCInterface::fwVersionReceived(int major, int minor)

{
    QPair<int, int> highest_supported = *std::max_element(mCompatibleFws.begin(), mCompatibleFws.end());
    QPair<int, int> fw_connected = qMakePair(major, minor);

    bool wasReceived = mFwVersionReceived;

    if (major < 0) {
        mFwVersionReceived = false;
        mFwRetries = 0;
        disconnect();
        emit msgCritical( "Error", "The firmware on the connected VESC is too old. Please"
                            " update it using a programmer.");
        update_firmwareVersion("Old Firmware");
    } else if (fw_connected > highest_supported) {
        mFwVersionReceived = true;
        mPacketInterface->setLimitedMode(true);
        if (!wasReceived) {
            emit msgwarning("Warning", "The connected VESC has newer firmware than this version of"
                                                " BLDC Tool supports. It is recommended that you update BLDC "
                                                " Tool to the latest version. Alternatively, the firmware on"
                                                " the connected VESC can be downgraded in the firmware tab."
                                                " Until then, limited communication mode will be used where"
                                                " only the firmware can be changed.");
        }
    } else if (!mCompatibleFws.contains(fw_connected)) {
        if (fw_connected >= qMakePair(1, 1)) {
            mFwVersionReceived = true;
            mPacketInterface->setLimitedMode(true);
            if (!wasReceived) {
                emit msgwarning("Warning", "The connected VESC has too old firmware. Since the"
                                                    " connected VESC has firmware with bootloader support, it can be"
                                                    " updated from the Firmware tab."
                                                    " Until then, limited communication mode will be used where only the"
                                                    " firmware can be changed.");
            }
        } else {
            mFwVersionReceived = false;
            mFwRetries = 0;
            disconnect();
            if (!wasReceived) {
                emit msgCritical( "Error", "The firmware on the connected VESC is too old. Please"
                                                   " update it using a programmer.");
            }
        }
    } else {
        mFwVersionReceived = true;
        if (fw_connected < highest_supported) {
            if (!wasReceived) {
                emit msgwarning("Warning", "The connected VESC has compatible, but old"
                                                    " firmware. It is recommended that you update it.");
            }
        }

        QString fwStr;
        mPacketInterface->setLimitedMode(false);
        fwStr.sprintf("VESC Firmware Version %d.%d", major, minor);
        emit statusInfoChanged(fwStr, true);
    }

    if (major >= 0) {
        QString fwText;
        fwText.sprintf("%d.%d", major, minor);
        update_firmwareVersion(fwText);
    }
}

void BLDCInterface::samplesReceived(QByteArray data)
{
    for (int i = 0;i < data.size();i++) {
        if (mSampleInt == 0 || mSampleInt == 1) {
            tmpCurr1Array.append(data[i]);
        } else if (mSampleInt == 2 || mSampleInt == 3) {
            tmpCurr2Array.append(data[i]);
        } else if (mSampleInt == 4 || mSampleInt == 5) {
            tmpPh1Array.append(data[i]);
        } else if (mSampleInt == 6 || mSampleInt == 7) {
            tmpPh2Array.append(data[i]);
        } else if (mSampleInt == 8 || mSampleInt == 9) {
            tmpPh3Array.append(data[i]);
        } else if (mSampleInt == 10 || mSampleInt == 11) {
            tmpVZeroArray.append(data[i]);
        } else if (mSampleInt == 12) {
            tmpStatusArray.append(data[i]);
        } else if (mSampleInt == 13 || mSampleInt == 14) {
            tmpCurrTotArray.append(data[i]);
        } else if (mSampleInt == 15 || mSampleInt == 16) {
            tmpFSwArray.append(data[i]);
        }

        mSampleInt++;
        if (mSampleInt == 17) {
            mSampleInt = 0;
        }

        if (tmpCurr1Array.size() == (m_sampleNum * 2)) {
            curr1Array = tmpCurr1Array;
            curr2Array = tmpCurr2Array;
            ph1Array = tmpPh1Array;
            ph2Array = tmpPh2Array;
            ph3Array = tmpPh3Array;
            vZeroArray = tmpVZeroArray;
            statusArray = tmpStatusArray;
            currTotArray = tmpCurrTotArray;
            fSwArray = tmpFSwArray;
            mDoReplot = true;
            mDoFilterReplot = true;
            mDoRescale = true;
        }
    }
}
void BLDCInterface::mcconfReceived(mc_configuration &mcconf)
{
    m_mcconf->setData(mcconf);
    emit statusInfoChanged("MCCONF Received", true);
}

void BLDCInterface::motorParamReceived(double cycle_int_limit, double bemf_coupling_k, QVector<int> hall_table, int hall_res)
{
    if (cycle_int_limit < 0.01 && bemf_coupling_k < 0.01) {
        emit statusInfoChanged("Bad Detection Result Received", false);
        update_mcconfDetectResultBrowser("Detection failed.");
        return;
    }
    emit statusInfoChanged("Detection Result Received", true);

    mDetectRes.updated = true;
    mDetectRes.cycle_int_limit = cycle_int_limit;
    mDetectRes.bemf_coupling_k = bemf_coupling_k;
    mDetectRes.hall_table = hall_table;
    mDetectRes.hall_res = hall_res;

    QString hall_str;
    if (hall_res == 0) {
        hall_str.sprintf("Detected hall sensor table:\n"
                         "%i, %i, %i, %i, %i, %i, %i, %i\n",
                         hall_table.at(0), hall_table.at(1),
                         hall_table.at(2), hall_table.at(3),
                         hall_table.at(4), hall_table.at(5),
                         hall_table.at(6), hall_table.at(7));
    } else if (hall_res == -1) {
        hall_str.sprintf("Hall sensor detection failed:\n"
                         "%i, %i, %i, %i, %i, %i, %i, %i\n",
                         hall_table.at(0), hall_table.at(1),
                         hall_table.at(2), hall_table.at(3),
                         hall_table.at(4), hall_table.at(5),
                         hall_table.at(6), hall_table.at(7));
    } else if (hall_res == -2) {
        hall_str.sprintf("WS2811 enabled. Hall sensors cannot be used.\n");
    } else if (hall_res == -3) {
        hall_str.sprintf("Encoder enabled. Hall sensors cannot be used.\n");
    } else {
        hall_str.sprintf("Unknown hall error: %d\n", hall_res);
    }

    update_mcconfDetectResultBrowser(QString().sprintf("Detection results:\n"
                                                             "Integrator limit: %.2f\n"
                                                             "BEMF Coupling: %.2f\n\n"
                                                             "%s",
                                                             cycle_int_limit,
                                                             bemf_coupling_k,
                                                             hall_str.toLocal8Bit().data()));
}


void BLDCInterface::motorRLReceived(double r, double l)
{
    if (r < 1e-9 && l < 1e-9) {
        emit statusInfoChanged("Bad Detection Result Received", false);
    } else {
        emit statusInfoChanged("Detection Result Received", true);
        set_mcconfFocDetectL(l);
        set_mcconfFocDetectR(r);
        mcconfFocCalcCC();
    }
}


void BLDCInterface::motorLinkageReceived(double flux_linkage)
{
    if (flux_linkage < 1e-9) {
        emit statusInfoChanged("Bad Detection Result Received", false);
    } else {
        emit statusInfoChanged("Detection Result Received", true);
        set_mcconfFocDetectLinkage(flux_linkage);
    }
}

void BLDCInterface::encoderParamReceived(double offset, double ratio, bool inverted)
{
    if (offset > 1000.0) {
        emit statusInfoChanged("Encoder not enabled in firmware", false);
        emit msgCritical("Error", "Encoder support is not enabled in the current firmware. Please \n"
                                           "upload firmware with encodcer support and try again.");
    } else {
        emit statusInfoChanged("Encoder Result Received", true);
        set_mcconfFocMeasureEncoderOffset(offset);
        set_mcconfFocMeasureEncoderRatio(ratio);
        set_mcconfFocMeasureEncoderInverted(inverted);
    }
}


void BLDCInterface::focHallTableReceived(QVector<int> hall_table, int res)
{
    if (res != 0) {
        emit statusInfoChanged("Bad Detection Result Received", false);
    } else {
        emit statusInfoChanged("Hall Result Received", true);
        QList<double> table;
        foreach (double val, hall_table)
            table.append(val);
        set_mcconfFocMeasureHallTable(table);
    }
}


void BLDCInterface::appconfReceived(app_configuration appconf){
    m_appconf->setData(appconf);
    emit statusInfoChanged("APPCONF Received", true);
}

void BLDCInterface::decodedPpmReceived(double ppm_value, double ppm_last_len)
{
    update_appconfDecodedPpm((ppm_value + 1.0) * 500.0);
    update_appconfPpmPulsewidth(ppm_last_len);
}

void BLDCInterface::decodedAdcReceived(double adc_value, double adc_voltage, double adc_value2, double adc_voltage2)
{
    update_appconfAdcDecoded(adc_value * 1000.0);
    update_appconfAdcVoltage(adc_voltage);

    update_appconfAdcDecoded2(adc_value2 * 1000.0);
    update_appconfAdcVoltage2(adc_voltage2);
}

void BLDCInterface::decodedChukReceived(double chuk_value)
{
    update_appconfDecodedChuk((chuk_value + 1.0) * 500.0);
}





void BLDCInterface::mcconfFocCalcCC()
{
    double r = get_mcconfFocDetectR();
    double l = get_mcconfFocDetectL();

    if (r < 1e-10) {
        emit msgCritical("Error", "R is 0. Please measure it first.");
        return;
    }

    if (l < 1e-10) {
        emit msgCritical( "Error", "L is 0. Please measure it first.");
        return;
    }

    l /= 1e6;
    double tc = m_mcconfFocCalcCCTc;
    double bw = 1.0 / (tc * 1e-6);
    double kp = l * bw;
    double ki = kp * (r / l);

    set_mcconfFocCalcKp(kp);
    set_mcconfFocCalcKi(ki);
}

void BLDCInterface::refreshSerialDevices()
{
    m_serialPortList.clear();

    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    foreach(const QSerialPortInfo &port, ports) {
        QString name = port.portName();
        int index = m_serialPortList.count();
        // put STMicroelectronics device first in list and add prefix
        if(port.manufacturer() == "STMicroelectronics") {
            name.insert(0, "VESC - ");
            index = 0;
        }
        m_serialPortList.insert(index, new SerialPort(name, port.systemLocation()));
    }

}

void BLDCInterface::disconnect()
{
    if (mSerialPort->isOpen()) {
        mSerialPort->close();
    }

    if (mPacketInterface->isUdpConnected()) {
        mPacketInterface->stopUdpConnection();
    }

    mFwVersionReceived = false;
    mFwRetries = 0;
}
