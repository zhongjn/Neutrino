#include "UI/mainwindow.h"
#include "ui_mainwindow.h"
#include "UI/inbox.h"

void MainWindow::ShowInbox() {
    QMessageBox::information(this, "WARNING", "Please wait for a moment until fetching finished");
    inbox w;
    this->hide();
    if (w.exec() == true) {
        this->close();
    }
    else {
        this->show();
    }
}

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(), this->height());
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(OnSigninClicked()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(OnCloseClicked()));
    if (!mgr->IsLoginNeeded()) {
        // QMessageBox::information(this, "PASS", "You have been remembered and you can sign in without inputs");
        auto result = QMessageBox::question(this, "Remember Me", "You have been remembered. Do you want to sign in directly?", QMessageBox::Yes | QMessageBox::No);
        if (result == QMessageBox::Yes) {
            ShowInbox();
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnSigninClicked()
{
    QString account = ui->lineEdit_account->text();
    QString password = ui->lineEdit_password->text();
    QString smtp = ui->lineEdit_smtp->text();
    QString smtp_port = ui->lineEdit_smtp_port->text();
    QString pop3 = ui->lineEdit_pop3->text();
    QString pop3_port = ui->lineEdit_pop3_port->text();
    madr = MailAddress(account.toStdString());

    if (!mgr->Login(CredentialInfo(madr, password.toStdString(), ServerEndPoint(smtp.toStdString(), smtp_port.toInt()), ServerEndPoint(pop3.toStdString(), pop3_port.toInt())),
        ui->checkBox->isChecked())
        )
    {
        QMessageBox::warning(this, "WARNING", "Wrong Input");
        return;
    }
    /*
    madr = MailAddress("610223674@qq.com");
    mgr->Login(CredentialInfo(madr, "ycagpzuevtubbbee",
        ServerEndPoint("smtp.qq.com", 465), ServerEndPoint("pop.qq.com", 995)), ui->checkBox->isChecked());
    */
    ShowInbox();

}
void MainWindow::OnCloseClicked()
{
    this->close();
}
