#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <Qdebug>
#include <QtWidgets>
#include <QMessageBox>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    arduino_is_available = false;
    arduino_port_name = "";
    Arduino = new QSerialPort;
    /*
     *  Testing code, prints the description, vendor id, and product id of all ports.
     *  Used it to determine the values for the arduino.
     *
     *
    */
    qDebug() << "Number of ports: " << QSerialPortInfo::availablePorts().length() << "\n";
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        qDebug() << "Description: " << serialPortInfo.description() << "\n";
        qDebug() << "Has vendor id?: " << serialPortInfo.hasVendorIdentifier() << "\n";
        qDebug() << "Vendor ID: " << serialPortInfo.vendorIdentifier() << "\n";
        qDebug() << "Has product id?: " << serialPortInfo.hasProductIdentifier() << "\n";
        qDebug() << "Product ID: " << serialPortInfo.productIdentifier() << "\n";
    }
    /*
    */


    /*
     *   Identify the port the arduino is on.
     */
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
   {
       ui->portComboBox->addItem(serialPortInfo.portName());
      if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
           if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
               if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
                   arduino_port_name = serialPortInfo.portName();
                   arduino_is_available = true;


               }
           }
       }

   }


   if(arduino_is_available){

   qDebug()<<"Found the arduino port...\n";                                 //Connect to arduino
   Arduino->setPortName(arduino_port_name);
   Arduino->setBaudRate(QSerialPort::Baud57600);
   Arduino->setDataBits(QSerialPort::Data8);
   Arduino->setParity(QSerialPort::NoParity);
   Arduino->setStopBits(QSerialPort::OneStop);
   Arduino->setFlowControl(QSerialPort::NoFlowControl);
   Arduino->open(QIODevice::ReadWrite);
   Arduino->write("oke");
   ui->FirmwareLabel->setText("Connected");
}
   else{
       qDebug()<<"Couldnt find correct port for arduino \n";
       QMessageBox::information(this, "Serial Port Error","Couldn't open serial port to arduino");
   }


}

MainWindow::~MainWindow()
{
     if(Arduino->isOpen()){
            Arduino->close();
        }

    delete ui;
}



void MainWindow::updatePos(QString command){
    if(Arduino->isWritable()){
        Arduino->write(command.toStdString().c_str());

    }else{
        qDebug()<<"Couldn't write to serial";
    }
}


void MainWindow::on_runButton_clicked()
{
    double valuex = ui->Xspinbox->value();                              //get value from encoder
    double valuey = ui->Yspinbox->value();


    double a=valuex;
    double b=valuey;
    double r1=18.5;
    double r2=20.042;
    double pi=3.1415926535897;
    double tetaQ;

    double num = (a*a)+(b*b);
    double sqrt = pow(num,0.5);
    double var1 = ((a*a)+(b*b)+(r1*r1)-(r2*r2))/(2*r1*sqrt);
    double var2 = ((a*a)+(b*b)-(r1*r1)-(r2*r2))/(2*r1*r2);



    tetaQ=acos(var1)*180/pi;                                                //inverse kinematics
    tetaS=((atan2(b,a))*180/pi)-tetaQ ;
    tetaE=acos(var2)*180/pi;                //change to degree
    tetaS= tetaS /0.711462;                                                      //change to pulse
    tetaE= tetaE /0.711462;

    MainWindow::updatePos(QString("A%1").arg(tetaS));                      //update pulse value to arduino
    qDebug()<<"value tetaS"<<tetaS;
    MainWindow::updatePos(QString("B%1").arg(tetaE));
    qDebug()<<"value tetaE"<<tetaE;


}
/*
void MainWindow::on_Yspinbox_valueChanged(double )
{

}

void MainWindow::on_Xspinbox_valueChanged(double )
{

}
*/



void MainWindow::on_checkBox_stateChanged(int arg1)
{
      if(arg1==2){
        MainWindow::updatePos(QString("r%1"));
         qDebug()<<"rectangle is clicked";
    }
}


void MainWindow::on_Circle_stateChanged(int arg2)
{
      if(arg2==2){
        MainWindow::updatePos(QString("c%1"));
        qDebug()<<"circle is clicked";
    }
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(this, tr("Help"),
                                  tr("How to operate SCARABOT\n\n"
                                     " 1.Connect computer with Arduino\n\n"
                                     " 2.Wait until it says found the arduino port"
                                     " 3.Enter the desire value x and y\n\n"
                                     " 4.Press the run button.\n"),
                                  QMessageBox::Close);
}
