#include "UI/send.h"
#include "ui_send.h"

send::send(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::send)
{
    ui->setupUi(this);
	ptLoop = new QEventLoop(this);
	connect(ui->pushButton_send, SIGNAL(clicked()), this, SLOT(OnSendClicked()));
	connect(ui->pushButton_return, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
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
	string subject = ui->lineEdit_3->text().toStdString();
	string content = ui->textEdit->toPlainText().toStdString();
	MailAddress receiver(ui->lineEdit_2->text().toStdString());
	Mail mail(std::move(subject), std::move(content), madr, std::move(receiver));
	mgr.SendMail(mail);
}
