#include "cport.h"

#include <QString>

cPort::cPort(QObject *parent) : QObject(parent), serial(nullptr)
{
    timer=new QTimer(this);
    timer->setInterval(100);
    QObject::connect(timer,&QTimer::timeout,this,&cPort::valinquiry);
}

bool cPort::connect(QString portName)
{

    if(serial != nullptr){
        serial->close();
        delete serial;
    }

    serial = new QSerialPort(this);
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);

    if(serial->open(QIODevice::ReadWrite)){
        qDebug()<<"Отлично";
        QObject::connect(serial,&QSerialPort::readyRead,this,&cPort::dataReady);

    }else
    {
        qDebug()<<"Ошибка соединения с портом";
    }
    timer->start();
    return serial->isOpen();
}

void cPort::setText(QString t1,QString t2){
    text=t1,text1=t2;//текст для установки сопротивления
    firstCon=false;
}

void cPort::dataReady()
{
    if(serial->isOpen()){
        parsData(serial->readAll());
        return;
    }
}



void cPort::parsData(QByteArray b)
{
    QString dataB=b;
    QRegExp rx("[\r]");

    QThread::msleep(50);
    int pos=0;
    pos=rx.indexIn(dataB,pos);
    if(pos!=-1){
        dataB.remove(pos,2);
    }
    resData=dataB;
    if (resData.size()>7) resData.remove(7,19);//Устранение лишних данных, если приходят дважды
    if(resData.size()>4){
        double volt=resData.toDouble()*10;
        resData=QString::number(volt,'g',4);
        //resData.push_back(" В");
        emit dataReceived();
        serial->clear();
    }
    if(resData.size()>1) firstCon=true;//Первая отправка была выполнена
}

void cPort::valinquiry()
{
    if(serial->isOpen()){
        if(firstCon==false){
            QString s;
            s="*CLS";
            QString EndOfLine("\r\n");
            s+=EndOfLine;
            if(text.size()<1){
                text="10";
                text1="кОм";
            }
            int number=text.toDouble();
            if(text1=="МОм"){
                text=QString::number(number*1000000);
            } else if(text1=="кОм") text=QString::number(number*1000);
            s+="CONF:RES ";
            s+=text;
            s+="M";
            s+=EndOfLine;
            s+=":VAL?";
            s+=EndOfLine;
            serial->write(s.toUtf8());
            firstCon=true;
        }else
        {
            QString s;
            s+=":VAL?";
            QString EndOfLine("\r\n");
            s+=EndOfLine;
            serial->write(s.toUtf8());
        }
    }
}

void cPort::quitPort(){//Закрытие порта
    if(serial != nullptr){
        serial->close();
        delete serial;
        serial=nullptr;
    }
    emit threadQuit();
}

cPort::~cPort(){
    if(serial != nullptr){
        serial->close();
        delete serial;
    }
        emit threadQuit();
}
