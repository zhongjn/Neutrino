#include "UI/Mailitem.h"

MailMore::MailMore(Mail *mail, QWidget *parent) : QPushButton(parent)
{
	m = mail;
	this->setText(QString::fromStdString(m->GetSubject()));
	connect(this, SIGNAL(clicked()), this, SLOT(OnMoreClicked()));
}

void MailMore::OnMoreClicked()
{
	mgr.SetMailRead(m->GetId());
	QDialog *w = new QDialog();
	//TO DO: UI
	QPlainTextEdit *t = new QPlainTextEdit(w);
	t->appendPlainText(QString::fromStdString(m->GetSubject()));
	t->setFocusPolicy(Qt::NoFocus);
	//TO DO: read
	t->show();
	w->exec();
}

MailChoose::MailChoose(Mail *mail, vector<int> *cid, QWidget *parent) : QCheckBox(parent)
{
	m = mail;
	c = cid;
	this->setChecked(false);
	for (vector<int>::iterator iter = c->begin(); iter != c->end(); iter++) {
		if (*iter == m->GetId()) {
			this->setChecked(true);
			break;
		}
	}
	connect(this, SIGNAL(clicked()), this, SLOT(OnChooseClicked()));
}

void MailChoose::OnChooseClicked()
{
	if (this->isChecked() == true) {
		c->push_back(m->GetId());
	}
	else {
		for (vector<int>::iterator iter = c->begin(); iter != c->end(); iter++) {
			if (*iter == m->GetId()) {
				iter = c->erase(iter);
				break;
			}
		}
	}
	
}

MailRead::MailRead(Mail *mail, QWidget *parent) : QCheckBox(parent)
{
	m = mail;
	this->setText("unread");
	this->setChecked(m->GetRead());
	connect(this, SIGNAL(clicked()), this, SLOT(OnReadClicked()));
}

void MailRead::OnReadClicked()
{
	mgr.SetMailRead(m->GetId());
	//m->SetRead(this->isChecked());
}

MailFlag::MailFlag(Mail *mail, QWidget *parent) : QCheckBox(parent)
{
	m = mail;
	this->setText("flag");
	this->setChecked(m->GetFlag());
	connect(this, SIGNAL(clicked()), this, SLOT(OnFlagClicked()));
}

void MailFlag::OnFlagClicked()
{
	mgr.SetMailFlag(m->GetId(), this->isChecked());
	//m->SetFlag(this->isChecked());
}