#pragma once

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QDialog>
#include <QPlainTextEdit>
#include <vector>
#include "MailManager.h"
#include "Common.h"
#include "inbox_detail.h"
#include <iostream>

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
