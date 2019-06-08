#include "UI/Mailitem.h"

MailMore::MailMore(Mail mail, QWidget *parent) : QPushButton(parent)
{
	m = mail;
	this->setText(QString::fromStdString(mail.GetSubject()));
	connect(this, SIGNAL(clicked()), this, SLOT(OnMoreClicked()));
}

void MailMore::OnMoreClicked()
{
	QDialog* w = new QDialog();
	//TO DO: UI
	QPlainTextEdit* m = new QPlainTextEdit(w);
	m->appendPlainText(QString::fromStdString(this->m.GetSubject()));
	m->setFocusPolicy(Qt::NoFocus);
	//TO DO: read
	m->show();
	w->exec();
}


MailRead::MailRead(Mail mail, QWidget *parent) : QCheckBox(parent)
{
	m = mail;
	this->setText("unread");
	connect(this, SIGNAL(clicked()), this, SLOT(OnReadClicked()));
}

void MailRead::OnReadClicked()
{
	if (this->isChecked() == true) {
		this->setChecked(false);
		//mail
	}
	else {
		this->setChecked(true);
		//mail
	}
}

MailFlag::MailFlag(Mail mail, QWidget *parent) : QCheckBox(parent)
{
	m = mail;
	this->setText("flag");
	connect(this, SIGNAL(clicked()), this, SLOT(OnReadClicked()));
}

void MailFlag::OnFlagClicked()
{
	if (this->isChecked() == true) {
		this->setChecked(false);
		//mail
	}
	else {
		this->setChecked(true);
		//mail
	}
}