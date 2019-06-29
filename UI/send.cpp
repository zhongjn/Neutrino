#include "UI/send.h"
#include "ui_send.h"

send::send(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::send)
{
    ui->setupUi(this);
	ptLoop = new QEventLoop(this);
	setWindowFlags(windowFlags()&~Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint);
	connect(ui->pushButton_send, SIGNAL(clicked()), this, SLOT(OnSendClicked()));
	connect(ui->pushButton_return, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
	connect(ui->pushButton_attach, SIGNAL(clicked()), this, SLOT(OnAttachClicked()));
	connect(ui->pushButton_clear, SIGNAL(clicked()), this, SLOT(OnClearClicked()));
	
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
	mgr->SendMail(mail, file);
	QMessageBox::information(this, "WARNING", "Send Successfully");
}

void send::OnAttachClicked()
{
	this->file = QFileDialog::getOpenFileName(NULL, "Choose a file", ".").toStdString();
}

void send::OnClearClicked()
{
	ui->lineEdit_3->clear();
	ui->textEdit->clear();
	ui->lineEdit_2->clear();
	file = Null();
}
