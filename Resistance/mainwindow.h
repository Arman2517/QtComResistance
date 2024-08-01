#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include "cport.h"
#include "plot.h"
#include "qcustomplot.h"
#include "detector.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   MainWindow(QWidget *parent = nullptr);
   ~MainWindow();
   bool startPort=false;
   QString text="10",text1="кОм";//текст для установки сопротивления
   int count=0;
   QString data;
private slots:
   void readData();//Получение данных с класса Порта

   void on_Start_clicked();
   void on_Stop_clicked();
   void on_changeUn_clicked();
   void on_createPlot_clicked();
   void on_deleteOne_clicked();

   void on_writeOne_clicked();

   void on_saveToJson_clicked();

   void on_loadFromJson_clicked();

   void on_selectDat_clicked();

signals:
   void unitsDataSend(QString t1,QString t2);//Сигнал для отправки единиц измерения
   void plotData(QString resData);//Сигнал для отправки данных для графика
   void portQuit();//Сигнал закрытия порта
   void portQuitV();//Сигнал закрытия порта вольтметра
   void sendResData(QString s);
private:
   Ui::MainWindow *ui;
   cPort port;
   Plot ploter;
   QThread thread_1;
   Detector sensor;
   void loadPorts();//Поиск доступных портов
};
#endif // MAINWINDOW_H
