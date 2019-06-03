#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>
#include <QEventLoop>

#include "MailManager.h"

namespace Ui {
	class welcome;
}
class welcome : public QWidget
{
    Q_OBJECT

public:
    explicit welcome(MailManager mgr, QWidget *parent = 0);
    ~welcome();

	bool exec();

private:
    Ui::welcome *ui;
	MailManager m;
	QEventLoop *ptLoop;
	bool closeResult;
private slots:
	void OnQuitClicked();
	void OnLogoutClicked();
	void OnInboxClicked();
	void OnSendClicked();
};

#endif // WELCOME_H
