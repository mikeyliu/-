#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QSerialPortInfo>
#include <QSettings>
#include <QThread>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    programFW("\"%1\""),
    programSSB("\"%1\"")
{
    ui->setupUi(this);
    this->setWindowTitle("量产烧录工具 V1.00 ");
    this->setWindowIcon(QIcon(":/style/title.png"));
    this->setWindowFlags(Qt::Dialog);

    init();

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

void MainWindow::myMesParsing(int channel, QString msg)
{
    if(msg.isEmpty()){
        return;
    }
    textVec[channel]->append(msg);
    textVec[channel]->update();
    progressVec[channel]->setValue(msg.split("%").at(0).toInt());

    if(msg.contains("SecurityA")){
        msgVec[channel]->setText("正在烧写安全核心镜像A");
    }else if(msg.contains("ProtocolA")){
        msgVec[channel]->setText("正在烧写协议核心镜像A");
    }else if(msg.contains("SecurityB")){
        msgVec[channel]->setText("正在烧写安全核心镜像B");
    }else if(msg.contains("ApplicationA")){
        msgVec[channel]->setText("正在烧写应用核心镜像A");
    }else if(msg.contains("frontend")){
        msgVec[channel]->setText("前端配置");
    }else if(msg.contains("general")){
        msgVec[channel]->setText("常规配置");
    }else if(msg.contains("image")){
        msgVec[channel]->setText("SSB加载中");
    }else if(msg.contains("Sorry")){
        burningUE(channel);
        return;
    }else if(msg.contains("finished") || msg.contains("Finished")){
        m_second[channel]=0;
        m_stopWatch[channel].stop();
        statusVec[channel]->setText("烧写完成");
        return;
    }else{
        msgVec[channel]->setText(msg);
    }
}



// UpdateAll --in TK05_SW_V3.4-B018.fwpkg --port com3
void MainWindow::burningUE(int channel)
{
    process[channel].close();
    statusVec[channel]->setText("连接中");
    if(ui->lineEdit_FW->text().isEmpty()) {
        QMessageBox::information(this,"提示","请先选择FW文件");
        statusVec[channel]->setText("");
        return;
    }
    QStringList argument;
    argument<<"UpdateAll"<<"--In"<<fileNameFW<<"--port"<<m_comboBox[channel]->currentText();
    connect(&process[channel],&QProcess::readyReadStandardOutput,this,_slotReadMsgF[channel]);
    process[channel].start(programFW,argument);
}

void MainWindow::_slotReadMsg_0()
{
    QString recMsg = QString::fromLocal8Bit(process[0].readAllStandardOutput());
     myMesParsing(0,recMsg.simplified());
}

void MainWindow::_slotReadMsg_1()
{
    QString recMsg = QString::fromLocal8Bit(process[1].readAllStandardOutput());
    myMesParsing(1,recMsg.simplified());
}

void MainWindow::_slotReadMsg_2()
{
    QString recMsg = QString::fromLocal8Bit(process[1].readAllStandardOutput());
    myMesParsing(2,recMsg.simplified());
}

void MainWindow::_slotReadMsg_3()
{
    QString recMsg = QString::fromLocal8Bit(process[3].readAllStandardOutput());
    myMesParsing(3,recMsg.simplified());
}

void MainWindow::_slotTime0()
{
    m_second[0] += 1;
    ui->pushButton_time_0->setText(QString::number(m_second[0]));
    statusVec[0]->setText("烧写中");
}

void MainWindow::_slotTime1()
{
    m_second[1] += 1;
    ui->pushButton_time_1->setText(QString::number(m_second[1]));
    statusVec[1]->setText("烧写中");
}

void MainWindow::_slotTime2()
{
    m_second[2] += 1;
    ui->pushButton_time_2->setText(QString::number(m_second[2]));
    statusVec[2]->setText("烧写中");
}

void MainWindow::_slotTime3()
{
    m_second[3] += 1;
    ui->pushButton_time_3->setText(QString::number(m_second[3]));
    statusVec[3]->setText("烧写中");
}

void MainWindow::on_pushButton_SSB_clicked()
{
    fileNameSSB = QFileDialog::getOpenFileName(this,tr("Open file"),QDir::currentPath());
    ui->lineEdit_SSB->setText(fileNameSSB);
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("firmWare");
    settings.setValue("SSB",fileNameSSB);
    settings.endGroup();
}

void MainWindow::on_pushButton_FW_clicked()
{
    fileNameFW = QFileDialog::getOpenFileName(this,tr("Open file"),QDir::currentPath());
    ui->lineEdit_FW->setText(fileNameFW);
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("firmWare");
    settings.setValue("UE",fileNameFW);
    settings.endGroup();
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
            portName [nCurSize++] = info.portName();
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

    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("soft");
    settings.setValue("UE",softNameFW);
    settings.endGroup();
}

void MainWindow::on_action_SSB_Loader_triggered()
{
    softNameSSB = QFileDialog::getOpenFileName(this,"SSB loader",QDir::currentPath());
    programSSB = programSSB.arg(softNameSSB);
    QSettings settings("config.ini", QSettings::IniFormat);
    settings.beginGroup("soft");
    settings.setValue("SSB",softNameSSB);
    settings.endGroup();
}

void MainWindow::on_actionabout_triggered()
{

}

bool MainWindow::isReady(int channel)
{
    statusVec[channel]->setText("连接中");
    if(ui->lineEdit_SSB->text().isEmpty() ) {
        QMessageBox::information(this,"提示","请先选择SSB文件");
        statusVec[channel]->setText("");
        return false;
    }
    if(m_comboBox[channel]->currentText().isEmpty()){
        QMessageBox::information(this,"提示","设备未连接");
        statusVec[channel]->setText("");
        return false;
    }
    return true;
}
// loadfirst -p port -i filename
void MainWindow::on_pushButton_start0_clicked()
{
    if(!isReady(0)){
        return;
    }
    m_stopWatch[0].start(1000);
    QStringList argument;
    argument<<"loadfirst"<<"-p"<<m_comboBox[0]->currentText()<< "-i" <<fileNameSSB;
    connect(&process[0],&QProcess::readyReadStandardOutput,this,&MainWindow::_slotReadMsg_0);
    process[0].start(programSSB,argument);
}

void MainWindow::on_pushButton_start1_clicked()
{
    if(!isReady(1)){
        return;
    }
    m_stopWatch[1].start(1000);
    QStringList argument;
    argument<<"loadfirst"<<"-p"<<m_comboBox[1]->currentText()<< "-i" <<fileNameSSB;
    connect(&process[1],&QProcess::readyReadStandardOutput,this,&MainWindow::_slotReadMsg_1);
    process[1].start(programSSB,argument);
}

void MainWindow::on_pushButton_start2_clicked()
{
    if(!isReady(2)){
        return;
    }
    m_stopWatch[2].start(1000);
    QStringList argument;
    argument<<"loadfirst"<<"-p"<<m_comboBox[2]->currentText()<< "-i" <<fileNameSSB;
    connect(&process[2],&QProcess::readyReadStandardOutput,this,&MainWindow::_slotReadMsg_2);
    process[2].start(programSSB,argument);
}

void MainWindow::on_pushButton_start3_clicked()
{
    if(!isReady(3)){
        return;
    }
    m_stopWatch[3].start(1000);
    QStringList argument;
    argument<<"loadfirst"<<"-p"<<m_comboBox[3]->currentText()<< "-i" <<fileNameSSB;
    connect(&process[3],&QProcess::readyReadStandardOutput,this,&MainWindow::_slotReadMsg_3);
    process[3].start(programSSB,argument);
}

void MainWindow::init()
{
    textVec <<ui->textEdit<<ui->textEdit_2<<ui->textEdit_3<<ui->textEdit_4;
    msgVec<<ui->lineEdit_msg0<<ui->lineEdit_msg1<<ui->lineEdit_msg2<<ui->lineEdit_msg3;
    statusVec<< ui->pushButton_status_0 << ui->pushButton_status_1 << ui->pushButton_status_2 << ui->pushButton_status_3;
    m_comboBox<<ui->comboBox_0<<ui->comboBox_1<<ui->comboBox_2<<ui->comboBox_3;
    progressVec<<ui->progressBar_0<<ui->progressBar_1<<ui->progressBar_2<<ui->progressBar_3;
    m_second[0] = 0;
    m_second[1] = 0;
    m_second[2] = 0;
    m_second[3] = 0;
    _slotReadMsgF<<&MainWindow::_slotReadMsg_0<<&MainWindow::_slotReadMsg_1<<&MainWindow::_slotReadMsg_2<<&MainWindow::_slotReadMsg_3;
    QSettings settings("config.ini", QSettings::IniFormat);
    ui->lineEdit_FW->setText(settings.value("firmWare/UE").toString());
    ui->lineEdit_SSB->setText(settings.value("firmWare/SSB").toString());
    fileNameFW = ui->lineEdit_FW->text();
    fileNameSSB = ui->lineEdit_SSB->text();

    softNameFW = settings.value("soft/UE").toString();
    softNameSSB = settings.value("soft/SSB").toString();

    programFW = programFW.arg(softNameFW);
    programSSB =programSSB.arg(softNameSSB);

    for(int i=0;i<textVec.size();i++){
        textVec[i]->hide();
    }
    this->resize(1024,300);
}

void MainWindow::on_actionhide_triggered()
{
    this->resize(QSize(1024,258));
    for(int i=0;i<textVec.size();i++){
        textVec[i]->hide();
    }
}

void MainWindow::on_actionshow_triggered()
{
    for(int i=0;i<textVec.size();i++){
        textVec[i]->show();
    }
    this->resize(1024,750);
}
