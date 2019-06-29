#pragma once
#pragma execution_character_set("UTF-8")
#include <QWidget>
#include <QLabel>
#include <QFont>
#include <QTreeWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QDialog>
#include <QLineEdit>
#include <vector>
#include "MailManager.h"
#include "UI/Common.h"
#include "UI/inbox_detail.h"
#include <functional>
#define InputWidth 200
#define InputHeight 100

class MailMore : public QPushButton
{
	Q_OBJECT

public:
	explicit MailMore(Mail *mail, bool *block, function<void()> onRefresh, QWidget *parent = 0);
	//~MailMore();
private:
	Mail *m;
	bool *block;
	function<void()> onRefresh;
private slots:
	void OnMoreClicked();
};

class MailChoose : public QCheckBox
{
	Q_OBJECT

public:
	Mail *m;
	vector<int> *c;
	explicit MailChoose(Mail *mail, vector<int> *cid, QWidget *parent = 0);
	//~MailRead();
public slots:
	void OnChooseClicked();
};

class MailRead : public QCheckBox
{
	Q_OBJECT

public:
	Mail *m;
	explicit MailRead(Mail *mail, QWidget *parent = 0);
	//~MailRead();
public slots:
	void OnReadClicked();
};

class MailFlag : public QCheckBox
{
	Q_OBJECT

public:
	Mail *m;
	explicit MailFlag(Mail *mail, QWidget *parent = 0);
	//~MailFlag();
public slots:
	void OnFlagClicked();
};

class StringInput : public QDialog
{
	Q_OBJECT

public:
	string *ss;
	QLabel *label;
	QLineEdit *lineEdit;
	QPushButton *pushButton_ok;
	QPushButton *pushButton_clear;
	explicit StringInput(string *s, QWidget *parent = 0);
	~StringInput();

private slots:
	void OnOkClicked();
	void OnCloseClicked();
};