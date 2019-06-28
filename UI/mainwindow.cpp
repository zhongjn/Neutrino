#include "UI/mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/inbox.h"

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
	QString account = ui->lineEdit->text();
	QString password = ui->lineEdit_2->text();
	//madr = MailAddress(account.toStdString());
	madr = MailAddress("610223674@qq.com");
	mgr->SetCredential(CredentialInfo(madr, "ycagpzuevtubbbee",
		ServerEndPoint("smtp.qq.com", 465), ServerEndPoint("pop.qq.com", 995)));
	//mgr.SetCredential(CredentialInfo(madr, password.toStdString(),
	//	ServerEndPoint("smtp.qq.com", 465), ServerEndPoint("pop.qq.com", 995)));

	if (true) {
		inbox w;
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
