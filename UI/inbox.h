#ifndef INBOX_H
#define INBOX_H

#include <QWidget>
#include <QTreeWidget>
#include <QEventLoop>
#include <QMouseEvent>
#include <QMessageBox>
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
	bool block;
	vector<MailChoose*> vc;
	vector<MailFlag*> vf;
	vector<MailRead*> vr;
	vector<MailMore*> vm;
	bool closeResult;

	ListSource GetTreeItem();
	void MailSearch(bool);
	void WheelEvent(QWheelEvent *event);
	int FindFolderId(string);
	int FindFolderIndex(string);
	QTreeWidgetItem *FindItemFolder(string Folder);
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
	int FolderNew();
	void FolderRename();
	void FolderRemove();
	void ScroolWidget(int);
	void OnFetchClicked();
};

#endif // INBOX_H
