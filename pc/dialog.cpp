#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/img/base/logo.png"));
    trayIcon->show();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
               this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    quitAction = new QAction("&Zakończ", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(quitAction);

    trayIcon->setContextMenu(trayIconMenu);

    addSerialPorts();
}

void Dialog::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        if(!isMinimized())
            this->show();
        qDebug() << "Triger";
        break;
    case QSystemTrayIcon::DoubleClick:
        qDebug() << "DoubleClick";
        if(!isMinimized())
            this->show();
        break;
    case QSystemTrayIcon::MiddleClick:
        qDebug() << "MiddleClick";
        break;
    default:
        ;
    }
}

void Dialog::changeEvent(QEvent *e)
{
    if(e->type() == QEvent::WindowStateChange)
        if(isMinimized())
            this->hide();
    QDialog::changeEvent(e);
}

void Dialog::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        if (!warringShown)
            QMessageBox::information(this, tr("Systray"),
                                 "Ten program będzie działał ciągle w tle."
                                 "Możesz śledzić jego stan za pomocą ikony w pasku zadań"
                                 "Aby zakończyć aplikcję wybierz opcję <b>Zakończ</b> z menu podręcznego"
                                 );
        hide();
        event->ignore();
        warringShown = true;
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::addSerialPorts(){
    ui->serialPortComboBox->addItems(availablePorts());
}

QStringList Dialog::availablePorts()
{
    QStringList ports;

    for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
    {
        qDebug() << port.portName() << port.vendorIdentifier() << port.productIdentifier()
                 << port.hasProductIdentifier() << port.hasVendorIdentifier() << port.isBusy();
        ports += port.portName();
    }
    return ports;
}
