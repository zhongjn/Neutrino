#ifndef INBOX_H
#define INBOX_H

#include <QWidget>
#include <QTreeWidget>
#include <QEventLoop>
#include <QMouseEvent>
#include <vector>
#include "UI/send.h"
#include "MailManager.h"
#include "UI/Common.h"

#include "UI/Mailitem.h"

#define MAILHEIGHT 25
#define CHECKBOXWIDTH 20
#define BUTTONWIDTH 540
#define ROWSPACE 5
#define COLUNMSPACE 5
#define WHEELUNIT 30

namespace Ui {
class inbox;
}

class inbox : public QWidget
{
    Q_OBJECT

public:
    explicit inbox(QWidget *parent = 0);
    ~inbox();
	bool exec();

	vector<Mail> mails;
	vector<int> cid;
private:
    Ui::inbox *ui;
	QEventLoop *ptLoop;
	vector<MailChoose*> vc;
	vector<MailFlag*> vf;
	vector<MailRead*> vr;
	vector<MailMore*> vm;
	bool closeResult;

	ListSource GetTreeItem();
	void MailSearch(bool);
	void wheelEvent(QWheelEvent *event);
private slots:
	void OnWriteClicked();
	void OnReturnClicked();
	void OnTreeChosen();
	void OnSearchEnter();
	void OnChooseAll();
	void OnReadAll();
	void OnDeleteClicked();
	void OnMark();
	void OnMove();
	void FolderNew();
	void FolderRename();
	void FolderRemove();
	void ScroolWidget(int);
};

#endif // INBOX_H
