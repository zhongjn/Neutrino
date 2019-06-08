#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>
#include <QEventLoop>
#include "Common.h"
#include "MailManager.h"

/*
	it's abandoned now
*/

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
	QEventLoop *ptLoop;
	bool closeResult;
private slots:
	void OnQuitClicked();
	void OnLogoutClicked();
	void OnInboxClicked();
	void OnSendClicked();
};

#endif // WELCOME_H
