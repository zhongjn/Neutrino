#ifndef SEND_H
#define SEND_H

#include <QWidget>
#include <QEventLoop>
#include "Common.h"
#include "MailManager.h"

namespace Ui {
class send;
}

class send : public QWidget
{
    Q_OBJECT

public:
    explicit send(QWidget *parent = 0);
    ~send();
	void exec();
private:
    Ui::send *ui;
	QEventLoop *ptLoop;
private slots:
	void OnSendClicked();
	void OnReturnClicked();
};

#endif // SEND_H
