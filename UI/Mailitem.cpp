#include "UI/Mailitem.h"

MailMore::MailMore(Mail *mail, QWidget *parent) : QPushButton(parent)
{
	m = mail;
	this->setText(QString::fromStdString(m->GetSubject()));
	connect(this, SIGNAL(clicked()), this, SLOT(OnMoreClicked()));
}

void MailMore::OnMoreClicked()
{
	mgr.SetMailRead(m->GetId(), true);
	inbox_detail *w = new inbox_detail(m, parentWidget());
	w->show();
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
	mgr.SetMailRead(m->GetId(), this->isChecked());
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
}

StringInput::StringInput(string *s, QWidget *parent) : QDialog(parent)
{
	ss = s;
	this->setFixedSize(InputWidth, InputHeight);
	label = new QLabel(this);
	lineEdit = new QLineEdit(this);
	pushButton_ok = new QPushButton(this);
	pushButton_clear = new QPushButton(this);
	label->setText("Please enter the name:");
	QFont font("Microsoft YaHei", 12, 50);
	label->setFont(font);
	label->move(InputWidth / 20, InputHeight / 20);
	lineEdit->setFixedSize(InputWidth * 2 / 3, InputHeight / 5);
	lineEdit->move(InputWidth / 6, InputHeight / 3);
	pushButton_ok->setText("ok");
	pushButton_ok->setFixedSize(InputWidth / 4, InputHeight / 5);
	pushButton_ok->move(InputWidth / 6, InputHeight * 2 / 3);
	pushButton_clear->setText("close");
	pushButton_clear->setFixedSize(InputWidth / 4, InputHeight / 5);
	pushButton_clear->move(InputWidth * 7 / 12, InputHeight * 2 / 3);

	connect(pushButton_ok, SIGNAL(clicked()), this, SLOT(OnOkClicked()));
	connect(pushButton_clear, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));
}

StringInput::~StringInput()
{
	delete label;
	delete lineEdit;
	delete pushButton_ok;
	delete pushButton_clear;
}

void StringInput::OnOkClicked()
{
	*ss = lineEdit->text().toStdString();
	this->close();
}

void StringInput::OnCloseClicked()
{
	this->close();
}