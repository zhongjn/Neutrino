#ifndef INBOX_H
#define INBOX_H

#include <QWidget>
#include <QTreeWidget>
#include <QEventLoop>
#include <QPushButton>
#include <QCheckBox>
#include <QPlainTextEdit>

#include <vector>
#include "MailManager.h"

namespace Ui {
class inbox;
}

class inbox : public QWidget
{
    Q_OBJECT

public:
    explicit inbox(MailManager m, QWidget *parent = 0);
    ~inbox();
	void exec();
private:
    Ui::inbox* ui;
	QEventLoop* ptLoop;
	MailManager mgr;
	vector<QCheckBox*> vc;
	vector<QPushButton*> vb;
private slots:
	void OnTreeChosen();
	void OnReadClicked();
	void OnMailClicked(Mail mail);
	void OnReturnClicked();
};

#endif // INBOX_H
