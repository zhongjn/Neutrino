#pragma once

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QDialog>
#include <QPlainTextEdit>
#include "MailManager.h"

#include <iostream>

class MailMore : public QPushButton
{
	Q_OBJECT

public:
	Mail m;
	explicit MailMore(Mail mail, QWidget *parent = 0);
	//~MailMore();
public slots:
	void OnMoreClicked();
};

class MailRead : public QCheckBox
{
	Q_OBJECT

public:
	Mail m;
	explicit MailRead(Mail mail, QWidget *parent = 0);
	//~MailRead();
public slots:
	void OnReadClicked();
};
