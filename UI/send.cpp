#include "UI/send.h"
#include "ui_send.h"

#include <QString>
#include "Mail.h"
#include "MailManager.h"

send::send(MailManager mgr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::send)
{
    ui->setupUi(this);
	m = mgr;
	ptLoop = new QEventLoop(this);
	connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(OnSendClicked()));
	connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
}

send::~send()
{
    delete ui;
}

void send::exec()
{
	this->setWindowModality(Qt::ApplicationModal);
	this->show();
	ptLoop->exec();
}
void send::OnReturnClicked()
{
	ptLoop->quit();
}
void send::OnSendClicked()
{
	int id;
	string subject = ui->lineEdit_3->text().toStdString();
	string content = ui->textEdit->toPlainText().toStdString();
	MailAddress sender(ui->lineEdit->text().toStdString());
	MailAddress receiver(ui->lineEdit_2->text().toStdString());
	Mail mail(id, std::move(subject), std::move(content), std::move(sender), std::move(receiver));
	m.SendMail(mail);
}
