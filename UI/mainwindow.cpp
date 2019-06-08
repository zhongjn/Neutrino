#include "UI/mainwindow.h"
#include "ui_mainwindow.h"

#include "UI/inbox.h"

#include <QString>

#include "MailManager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(OnSigninClicked()));
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));
	
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnSigninClicked()
{
	MailManager mgr;

	QString account = ui->lineEdit->text();
	QString password = ui->lineEdit_2->text();

	mgr.SetCredential(CredentialInfo(MailAddress("610223674@qq.com"), "ycagpzuevtubbbee",
		ServerEndPoint("smtp.qq.com", 465), ServerEndPoint("pop.qq.com", 995)));
	//mgr.SetCredential(CredentialInfo(MailAddress(account.toStdString()), password.toStdString(),
	//	ServerEndPoint("smtp.qq.com", 465), ServerEndPoint("pop.qq.com", 995)));
	///mgr.FetchMails();

	if (true) {
		inbox w(&mgr);
		this->hide();
		if (w.exec() == true) {
			this->close();
		}
		else {
			this->show();
		}
	}
}
void MainWindow::OnCloseClicked()
{
	this->close();
}
