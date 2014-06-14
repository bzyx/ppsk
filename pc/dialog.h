#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSystemTrayIcon>
#include <QEvent>

namespace Ui {
class Dialog;
class QStringList;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    virtual void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    ~Dialog();

public slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::Dialog *ui;
    bool warringShown = false;
    QAction *quitAction;
    QMenu* trayIconMenu;

    QSystemTrayIcon *trayIcon;
    void addSerialPorts();
    QStringList availablePorts();
};

#endif // DIALOG_H
