#include "ui_welcome.h"
#include "UI/welcome.h"
#include "UI/inbox.h"
#include "UI/send.h"

welcome::welcome(MailManager mgr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::welcome)
{
    ui->setupUi(this);
	m = mgr;
	ptLoop = new QEventLoop(this);
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(OnInboxClicked()));
	connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(OnSendClicked()));
	connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(OnLogoutClicked()));
	connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(OnQuitClicked()));
}

welcome::~welcome()
{
    delete ui;
}
bool welcome::exec()
{
	setWindowModality(Qt::ApplicationModal);
	show();
	ptLoop->exec();

	return closeResult;
}
void welcome::OnLogoutClicked()
{
	closeResult = false;
	ptLoop->quit();
}
void welcome::OnQuitClicked()
{
	closeResult = true;
	ptLoop->quit();
}
void welcome::OnInboxClicked()
{
	inbox w(m);
	w.exec();
}
void welcome::OnSendClicked()
{
	send w(m);
	w.exec();
}