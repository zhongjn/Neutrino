#include "inbox_detail.h"
#include "ui_inbox_detail.h"

inbox_detail::inbox_detail(Mail *mail, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::inbox_detail)
{
	ui->setupUi(this);
	m = mail;
	ptLoop = new QEventLoop(this);
	ui->label_title->setText(QString::fromStdString(mail->GetSubject()));
	ui->label_sender->setText(QString::fromStdString(mail->GetSender()));
	//ui->label_time->setText(QString::fromStdString(mail->GetSender()));
	ui->label_time->setText("2019-06-12");
	ui->textBrowser->setText(QString::fromStdString(mail->GetContent()));

	connect(ui->pushButton_back, SIGNAL(clicked()), this, SLOT(OnBackClicked()));
	connect(ui->pushButton_reply, SIGNAL(clicked()), this, SLOT(OnReplyClicked()));
	connect(ui->pushButton_delete, SIGNAL(clicked()), this, SLOT(OnDeleteClicked()));
	connect(ui->comboBox_mark, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnMarkClicked()));
	connect(ui->pushButton_file, SIGNAL(clicked()), this, SLOT(OnFileClicked()));
}

inbox_detail::~inbox_detail()
{
	delete ui;
}

void inbox_detail::exec()
{
	this->show();
	ptLoop->exec();
}

void inbox_detail::OnBackClicked()
{
	ptLoop->quit();
	this->close();
}

void inbox_detail::OnReplyClicked()
{
	send w;
	w.exec();
}

void inbox_detail::OnDeleteClicked()
{
	if (m->GetSpam() == false) {
		mgr->SetMailSpam(m->GetId(), true);
	}
	else {
		mgr->DeleteMail(m->GetId());
	}
	OnBackClicked();
}

void inbox_detail::OnMarkClicked()
{
	if (ui->comboBox_mark->currentIndex() == 1) {
		mgr->SetMailRead(m->GetId(), true);
	}
	else if (ui->comboBox_mark->currentIndex() == 2) {
		mgr->SetMailRead(m->GetId(), false);
	}
	else if (ui->comboBox_mark->currentIndex() == 3) {
		mgr->SetMailFlag(m->GetId(), true);
	}
	else if (ui->comboBox_mark->currentIndex() == 4) {
		mgr->SetMailFlag(m->GetId(), false);
	}
	ui->comboBox_mark->setCurrentIndex(0);
}

void inbox_detail::OnFileClicked()
{
	string att = "explorer /select, ./attachments/" + m->GetAttachmentName();
	system(att.c_str());
	//QDesktopServices::openUrl(QUrl(QString::fromStdString(att), QUrl::TolerantMode));
}
