#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

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

    void on_YSpinBox_valueChanged(double valuey);

    void on_XSpinBox_valueChanged(double valuex);

    void updatePos(QString);


    void on_runButton_clicked();

    void on_Yspinbox_valueChanged(double arg1);

    void on_Xspinbox_valueChanged(double arg1);

    void on_checkBox_clicked();

    void on_radioButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_Circle_stateChanged(int arg1);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *Arduino;
    static const quint16 arduino_uno_vendor_id = 9025;         //arduino identifier
    static const quint16 arduino_uno_product_id = 66;
    QString arduino_port_name;
    bool arduino_is_available;
    double tetaS ;
    double tetaE = 0;


};

#endif // MAINWINDOW_H
