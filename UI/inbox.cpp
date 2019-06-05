#include "UI/inbox.h"
#include "ui_inbox.h"

inbox::inbox(MailManager mgr, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inbox)
{
    ui->setupUi(this);
	m = mgr;
	ptLoop = new QEventLoop(this);
	connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
}

inbox::~inbox()
{
    delete ui;
}
void inbox::exec()
{
	this->setWindowModality(Qt::ApplicationModal);
	this->show();
	ptLoop->exec();
}
void inbox::OnReturnClicked()
{
	ptLoop->quit();
}