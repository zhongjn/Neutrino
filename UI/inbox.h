#ifndef INBOX_H
#define INBOX_H

#include <QWidget>
#include <QTreeWidget>
#include <QEventLoop>
#include <QPushButton>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <vector>
#include "UI/send.h"
#include "MailManager.h"

#define MAILHEIGHT 25
#define CHECKBOXWIDTH 60
#define BUTTONWIDTH 540
#define ROWSPACE 35
#define COLUNMSPACE 5

namespace Ui {
class inbox;
}

class inbox : public QWidget
{
    Q_OBJECT

public:
    explicit inbox(MailManager m, QWidget *parent = 0);
    ~inbox();
	bool exec();
private:
    Ui::inbox* ui;
	QEventLoop* ptLoop;
	MailManager mgr;
	vector<QCheckBox*> vc;
	vector<QPushButton*> vb;
	bool closeResult;
private slots:
	void OnTreeChosen();
	void OnWriteClicked();
	void OnReadClicked(Mail mail, QCheckBox* c);
	void OnMailClicked(Mail mail);
	void OnReturnClicked();
};

#endif // INBOX_H
