#ifndef INBOX_H
#define INBOX_H

#include <QWidget>
#include <QEventLoop>

#include "MailManager.h"

namespace Ui {
class inbox;
}

class inbox : public QWidget
{
    Q_OBJECT

public:
    explicit inbox(MailManager mgr, QWidget *parent = 0);
    ~inbox();
	void exec();
private:
    Ui::inbox *ui;
	MailManager m;
	QEventLoop *ptLoop;
private slots:
	void OnReturnClicked();
};

#endif // INBOX_H
