#pragma once

#include <QPushButton>

class MailButton : public QPushButton
{
public:
	MailButton(QString filename);
	~MailButton();
};
MailButton::MailButton(QString filename)
{
	this->filename = filename;
	this->setText(this->filename);
	connect(this, SIGNAL(clicked(bool)), this, SLOT(displaySlot()));
}