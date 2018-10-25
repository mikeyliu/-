#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QProcess>
class QLineEdit;
class QTextEdit;
class QComboBox;
class QPushButton;
class QProgressBar;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    typedef void (MainWindow::*fp)();
public slots:
    void _slotReadMsg_0();
    void _slotReadMsg_1();
    void _slotReadMsg_2();
    void _slotReadMsg_3();
private slots:
    void on_pushButton_SSB_clicked();
    void on_pushButton_FW_clicked();

    void _slotQueryPortInfo();
    void _slotTime0();
    void _slotTime1();
    void _slotTime2();
    void _slotTime3();
    void on_soft_triggered();
    void on_actionabout_triggered();
    void on_action_SSB_Loader_triggered();
    void on_pushButton_start0_2_clicked();
    void on_pushButton_start1_1_clicked();
    void on_pushButton_start2_2_clicked();
    void on_pushButton_start3_3_clicked();
    void on_actionhide_triggered();
    void on_actionshow_triggered();

private:
    Ui::MainWindow *ui;
    QString                     fileNameSSB,fileNameFW;
    QVector<QLineEdit*>  msgVec;
    QVector<QTextEdit*>  textVec;
    QVector<QPushButton*>   statusVec;
    QVector<QComboBox*>   m_comboBox;
    QVector<QProgressBar*> progressVec;
    QProcess                  process[4];
    QString                     programFW;
    QString                     softNameFW;
    QString                     programSSB;
    QString                     softNameSSB;
    QTimer                     m_timer;
    QTimer                     m_stopWatch[4];
    quint32                     progressNum;
    quint32                     m_second[4];
    QVector<fp>            _slotReadMsgF;
private:
    void init();
    void burningUE(int channel);
    void myMesParsing(int channel, QString msg);
};

#endif // MAINWINDOW_H
