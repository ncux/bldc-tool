/*
    Copyright 2012-2014 Benjamin Vedder	benjamin@vedder.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QByteArray>
#include <QSerialPort>

#include "qcustomplot.h"
#include "packetinterface.h"
#include "serialization.h"
#include "datatypes.h"
#include "bldcinterface.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool eventFilter(QObject *object, QEvent *e);

private slots:
    void setMcconfGui(const mc_configuration &mcconf);
    void UpdatePlots();
    void showStatusInfo(QString info, bool isGood);
    void mcValuesReceived(MC_VALUES values);
    void samplesReceived(QByteArray data);
    void rotorPosReceived(double pos);
    void experimentSamplesReceived(QVector<double> samples);
    void setFocHallTable(QList<int> hall_table);
    void appconfReceived(app_configuration appconf);
    void decodedPpmReceived(double ppm_value, double ppm_last_len);
    void decodedAdcReceived(double adc_value, double adc_voltage, double adc_value2, double adc_voltage2);
    void decodedChukReceived(double chuk_value);

    void on_serialConnectButton_clicked();
    void on_udpConnectButton_clicked();
    void on_disconnectButton_clicked();
    void on_getDataButton_clicked();
    void on_getDataStartButton_clicked();
    void on_dutyButton_clicked();
    void on_rpmButton_clicked();
    void on_currentButton_clicked();
    void on_offButton_clicked();
    void on_offBrakeButton_clicked();
    void on_replotButton_clicked();
    void on_rescaleButton_clicked();
    void on_horizontalZoomBox_clicked();
    void on_verticalZoomBox_clicked();
    void on_filterLogScaleBox_clicked(bool checked);
    void on_filterLogScaleBox2_clicked(bool checked);
    void on_detectButton_clicked();
    void on_clearTerminalButton_clicked();
    void on_sendTerminalButton_clicked();
    void on_stopDetectButton_clicked();
    void on_experimentClearSamplesButton_clicked();
    void on_experimentSaveSamplesButton_clicked();
    void on_mcconfReadButton_clicked();
    void on_mcconfReadDefaultButton_clicked();
    void on_mcconfWriteButton_clicked();
    void on_currentBrakeButton_clicked();
    void on_mcconfLoadXmlButton_clicked();
    void on_mcconfSaveXmlButton_clicked();
    void on_mcconfDetectMotorParamButton_clicked();
    void on_appconfReadButton_clicked();
    void on_appconfWriteButton_clicked();
    void on_appconfRebootButton_clicked();
    void on_appconfReadDefaultButton_clicked();
    void on_posCtrlButton_clicked();
    void on_firmwareChooseButton_clicked();
    void on_firmwareUploadButton_clicked();
    void on_firmwareVersionReadButton_clicked();
    void on_firmwareCancelButton_clicked();
    void on_servoOutputSlider_valueChanged(int value);
    void on_mcconfFocObserverGainCalcButton_clicked();
    void on_mcconfFocMeasureRLButton_clicked();
    void on_mcconfFocMeasureLinkageButton_clicked();
    void on_mcconfFocCalcCCButton_clicked();
    void on_mcconfFocApplyRLLambdaButton_clicked();
    void on_mcconfFocCalcCCApplyButton_clicked();
    void on_mcconfDetectApplyButton_clicked();
    void on_mcconfFocMeasureEncoderButton_clicked();
    void on_mcconfFocMeasureEncoderApplyButton_clicked();
    void on_detectEncoderButton_clicked();
    void on_detectPidPosButton_clicked();
    void on_detectPidPosErrorButton_clicked();
    void on_detectEncoderObserverErrorButton_clicked();
    void on_detectObserverButton_clicked();
    void on_mcconfFocMeasureHallButton_clicked();
    void on_mcconfFocMeasureHallApplyButton_clicked();
    void on_refreshButton_clicked();
    void on_bleScanButton_clicked();
    void on_bleConnectButton_clicked();

private:
    BLDCInterface *m_bldcInterface;
    Ui::MainWindow *ui;
    QLabel *mStatusLabel;
    int mStatusInfoTime;
    QList<QPair<int, int> > mCompatibleFws;
    int mSampleInt;
    QByteArray curr1Array;
    QByteArray curr2Array;
    QByteArray ph1Array;
    QByteArray ph2Array;
    QByteArray ph3Array;
    QByteArray vZeroArray;
    QByteArray statusArray;
    QByteArray currTotArray;
    QByteArray fSwArray;
    QByteArray tmpCurr1Array;
    QByteArray tmpCurr2Array;
    QByteArray tmpPh1Array;
    QByteArray tmpPh2Array;
    QByteArray tmpPh3Array;
    QByteArray tmpVZeroArray;
    QByteArray tmpStatusArray;
    QByteArray tmpCurrTotArray;
    QByteArray tmpFSwArray;

    QVector<double> tempMos1Vec;
    QVector<double> tempMos2Vec;
    QVector<double> tempMos3Vec;
    QVector<double> tempMos4Vec;
    QVector<double> tempMos5Vec;
    QVector<double> tempMos6Vec;
    QVector<double> tempPcbVec;
    QVector<double> currInVec;
    QVector<double> currMotorVec;
    QVector<double> dutyVec;
    QVector<double> rpmVec;
    QVector<double> voltInVec;
    QVector<double> positionVec;

    bool mRealtimeGraphsAdded;
    bool mDoReplot;
    bool mDoReplotPos;
    bool mDoRescale;
    bool mDoFilterReplot;
    PacketInterface *mPacketInterface;
    bool mMcconfLoaded;
    bool mAppconfLoaded;

    QVector<QVector<double> > mExperimentSamples;

    mc_configuration getMcconfGui();
    void appendDoubleAndTrunc(QVector<double> *vec, double num, int maxSize);
    void clearBuffers();
    void saveExperimentSamplesToFile(QString path);

};

#endif // MAINWINDOW_H
