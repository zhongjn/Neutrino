#pragma once

#include <QWidget>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QCheckBox>
#include <QDialog>
#include <QLineEdit>
#include <vector>
#include "MailManager.h"
#include "Common.h"
#include "inbox_detail.h"
#include <iostream>

#define InputWidth 200
#define InputHeight 100

class MailMore : public QPushButton
{
	Q_OBJECT

public:
	Mail *m;
	explicit MailMore(Mail *mail, QWidget *parent = 0);
	//~MailMore();
public slots:
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