#ifndef CPORT_H
#define CPORT_H

#include <QObject>
#include <QObject>
#include <QSerialPortInfo>
#include <QtSerialPort>

class cPort : public QObject
{
    Q_OBJECT
public:
    QString resData;// Данные полученные с вольтметра
    QString text,text1;//текст для установки сопротивления
    bool firstCon=false; // проверка на соединение
    int i=0;
    explicit cPort(QObject *parent = nullptr);
    ~cPort();
    bool connect(QString portName);//Соединение с портом
    void parsData(QByteArray b);//Обработка полученных данных

signals:
    void dataReceived();//Передает исходные полученные данные в main
    void threadQuit();//Завершение потока

public slots:
    void dataReady();
    void quitPort();//Cлот Закрытие порта
    void valinquiry();
    void setText(QString t1,QString t2);

private:
    QSerialPort *serial;
    QTimer *timer;

};

#endif // CPORT_H
