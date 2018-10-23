#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QSerialPortInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    softNameFW = "C:/Program Files (x86)/Neul/UEUpdaterCLI/UEUpdater.exe";
    softNameSSB = "C:/Program Files (x86)/Neul/SsbLoader/SsbLoader.exe";
    programFW = "\"%1\"";
    programSSB = "\"%1\"";
    programFW = programFW.arg(softNameFW);
    programSSB = programSSB.arg(softNameSSB);

    this->setWindowTitle("量产烧录工具 V1.00 by mikey.liu");
    this->setWindowIcon(QIcon(":/style/title.png"));
    m_comboBox[0] = ui->comboBox_0;
    m_comboBox[1] = ui->comboBox_1;
    m_comboBox[2] = ui->comboBox_2;
    m_comboBox[3] = ui->comboBox_3;

    m_second[0] = 0;
    m_second[1] = 0;
    m_second[2] = 0;
    m_second[3] = 0;

    // 查询可用串口
    m_timer.setSingleShot(true);
    m_timer.setInterval(100);
    connect(&m_timer,&QTimer::timeout,this,&MainWindow::_slotQueryPortInfo,Qt::QueuedConnection);
    m_timer.start();

    // 实现简易秒表
    connect(&m_stopWatch[0],&QTimer::timeout,this,&MainWindow::_slotTime0);
    connect(&m_stopWatch[1],&QTimer::timeout,this,&MainWindow::_slotTime1);
    connect(&m_stopWatch[2],&QTimer::timeout,this,&MainWindow::_slotTime2);
    connect(&m_stopWatch[3],&QTimer::timeout,this,&MainWindow::_slotTime3);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// UpdateAll --in TK05_SW_V3.4-B018.fwpkg --port com3
void MainWindow::on_pushButton_start0_clicked()
{
    ui->pushButton_status_0->setText("连接中");
    if(ui->lineEdit_FW->text().isEmpty()) {
        QMessageBox::information(this,"提示","请先选择FW文件");
        return;
    }
    m_stopWatch[0].start(1000);
    argument<<"UpdateAll"<<"--in"<<fileNameFW<<"--port"<<ui->comboBox_0->currentText();
    connect(&process[0],&QProcess::readyReadStandardOutput,this,&MainWindow::_slotReadMsg_0);
    process[0].start(programFW,argument);
}
void MainWindow::on_pushButton_start1_clicked()
{
    ui->pushButton_status_1->setText("连接中");
    if(ui->lineEdit_FW->text().isEmpty()) {
        QMessageBox::information(this,"提示","请先选择FW文件");
        return;
    }
    m_stopWatch[1].start(1000);
    argument<<"UpdateAll"<<"--in"<<fileNameFW<<"--port"<<ui->comboBox_1->currentText();
    connect(&process[1],&QProcess::readyReadStandardOutput,this,&MainWindow::_slotReadMsg_1);
    process[1].start(programFW,argument);
}

void MainWindow::on_pushButton_start2_clicked()
{
    ui->pushButton_status_2->setText("连接中");
    if(ui->lineEdit_FW->text().isEmpty()) {
        QMessageBox::information(this,"提示","请先选择FW文件");
        return;
    }
    m_stopWatch[2].start(1000);
    argument<<"UpdateAll"<<"--in"<<fileNameFW<<"--port"<<ui->comboBox_2->currentText();
    connect(&process[2],&QProcess::readyReadStandardOutput,this,&MainWindow::_slotReadMsg_2);
    process[2].start(programFW,argument);
}

void MainWindow::on_pushButton_start3_clicked()
{
    ui->pushButton_status_3->setText("连接中");
    if(ui->lineEdit_FW->text().isEmpty()) {
        QMessageBox::information(this,"提示","请先选择FW文件");
        return;
    }
    m_stopWatch[3].start(1000);
    argument<<"UpdateAll"<<"--in"<<fileNameFW<<"--port"<<ui->comboBox_3->currentText();
    connect(&process[3],&QProcess::readyReadStandardOutput,this,&MainWindow::_slotReadMsg_3);
    process[3].start(programFW,argument);
}

void MainWindow::_slotReadMsg_0()
{
    ui->pushButton_status_0->setText("刷写中");
    QString receMsg = QString::fromLocal8Bit(process[0].readAllStandardOutput());
    ui->lineEdit_msg0->setText(receMsg);
    if(receMsg.contains("Application finished")){
        ui->pushButton_status_0->setText("刷写成功");
        m_stopWatch[0].stop();
        return;
    }
    myMesParsing(receMsg);
}

void MainWindow::_slotReadMsg_1()
{
    ui->pushButton_status_1->setText("刷写中");
    QString receMsg = QString::fromLocal8Bit(process[1].readAllStandardOutput());
    ui->lineEdit_msg1->setText(receMsg);
    if(receMsg.contains("Application finished")){
        ui->pushButton_status_1->setText("刷写成功");
        m_stopWatch[1].stop();
        return;
    }
    myMesParsing(receMsg);
}

void MainWindow::_slotReadMsg_2()
{
    ui->pushButton_status_2->setText("刷写中");
    QString receMsg = QString::fromLocal8Bit(process[1].readAllStandardOutput());
    ui->lineEdit_msg2->setText(receMsg);
    if(receMsg.contains("Application finished")){
        ui->pushButton_status_2->setText("刷写成功");
        m_stopWatch[2].stop();
        return;
    }
    myMesParsing(receMsg);
}

void MainWindow::_slotReadMsg_3()
{
    ui->pushButton_status_3->setText("刷写中");
    QString receMsg = QString::fromLocal8Bit(process[3].readAllStandardOutput());
    ui->lineEdit_msg3->setText(receMsg);
    if(receMsg.contains("Application finished")){
        ui->pushButton_status_3->setText("刷写成功");
        m_stopWatch[3].stop();
        return;
    }
    myMesParsing(receMsg);
}

void MainWindow::_slotTime0()
{
    m_second[0] += 1;
    ui->pushButton_time_0->setText(QString::number(m_second[0]));
}

void MainWindow::_slotTime1()
{
    m_second[1] += 1;
    ui->pushButton_time_1->setText(QString::number(m_second[1]));
}

void MainWindow::_slotTime2()
{
    m_second[2] += 1;
    ui->pushButton_time_2->setText(QString::number(m_second[2]));
}

void MainWindow::_slotTime3()
{
    m_second[3] += 1;
    ui->pushButton_time_3->setText(QString::number(m_second[3]));
}

void MainWindow::myMesParsing(QString msg)
{
    if(msg.isEmpty()){
        return;
    }
    progressNum = msg.split("%").at(0).toInt();
    ui->progressBar->setValue(progressNum);
}

void MainWindow::on_pushButton_SSB_clicked()
{
    fileNameSSB = QFileDialog::getOpenFileName(this,tr("Open file"),QDir::currentPath());
    ui->lineEdit_SSB->setText(fileNameSSB);
}

void MainWindow::on_pushButton_FW_clicked()
{
    fileNameFW = QFileDialog::getOpenFileName(this,tr("Open file"),QDir::currentPath());
    ui->lineEdit_FW->setText(fileNameFW);
}

void MainWindow::_slotQueryPortInfo()
{
    int nCurSize =0;
    QList<QSerialPortInfo>   now_portInfoList;
    const auto infos = QSerialPortInfo::availablePorts();
    QString portName[4];
    foreach(QSerialPortInfo info,infos)
    {
        if( (info.description() == "Silicon Labs CP210x USB to UART Bridge")
                ||(info.description() == "Prolific USB-to-Serial Comm Port")
                ||(info.description() == "USB-SERIAL CH340")
                ||(info.description() == "MTK USB Port")
                ||(info.description() == "MediaTek USB Port")
                ||(info.description() == "USB Modem Driver"))
        {
            now_portInfoList << info;
            portName [nCurSize++] = /*info.description()+*/info.portName();
        }

    }
    for(int k = 0;k < nCurSize;k++)
    {
        for(int j = 0;j < nCurSize - 1;j++)
        {
            QString port;
            if (portName[j+1].length() < portName[j].length() ||
                    (portName[j+1].length() == portName[j].length() && (portName[j+1] < portName[j])))
            {
                port = portName[j];
                portName[j] =  portName[j+1];
                portName[j+1] = port;
            }
        }
    }
   for(int i= 0;i<nCurSize;i++){
       m_comboBox[i]->clear();
       m_comboBox[i]->addItem(portName[i]);
   }
   m_timer.start();
}

void MainWindow::on_soft_triggered()
{
    softNameFW = QFileDialog::getOpenFileName(this,"UE Updater",QDir::currentPath());
    programFW = programFW.arg(softNameFW);
}

void MainWindow::on_action_SSB_Loader_triggered()
{
    softNameSSB = QFileDialog::getOpenFileName(this,"SSB loader",QDir::currentPath());
    programSSB = programSSB.arg(softNameSSB);
}

void MainWindow::on_actionabout_triggered()
{

}

void MainWindow::on_pushButton_start0_2_clicked()
{
    ui->pushButton_status_0->setText("连接中");
    if(ui->lineEdit_SSB->text().isEmpty()) {
        QMessageBox::information(this,"提示","请先选择SSB文件");
        ui->pushButton_status_0->setText("");
        return;
    }
    m_stopWatch[0].start(1000);
    argument<<"UpdateAll"<<"--in"<<fileNameFW<<"--port"<<ui->comboBox_3->currentText();
    connect(&process[3],&QProcess::readyReadStandardOutput,this,&MainWindow::_slotReadMsg_3);
    process[3].start(programFW,argument);
}
