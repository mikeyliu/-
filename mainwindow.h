#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QProcess>
class QComboBox;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_start0_clicked();
    void on_pushButton_start1_clicked();
    void on_pushButton_start2_clicked();
    void on_pushButton_start3_clicked();
    void on_pushButton_SSB_clicked();
    void on_pushButton_FW_clicked();

    void _slotQueryPortInfo();
    void _slotReadMsg_0();
    void _slotReadMsg_1();
    void _slotReadMsg_2();
    void _slotReadMsg_3();
    void _slotTime0();
    void _slotTime1();
    void _slotTime2();
    void _slotTime3();
    void on_soft_triggered();

    void on_actionabout_triggered();

private:
    Ui::MainWindow *ui;
    QString                 fileNameSSB,fileNameFW;
    QProcess              process[4];
    QString                 programFW;
    QString                 softNameFW;
    QString                 programSSB;
    QString                 softNameSSB;
    QStringList            argument;
    QTimer                  m_timer;
    QTimer                  m_stopWatch[4];
    QComboBox*        m_comboBox[4];
    quint32                 progressNum;
    quint32                 m_second[4];

private:
    void myMesParsing(QString msg);
};

#endif // MAINWINDOW_H
