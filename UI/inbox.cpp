#include "UI/inbox.h"
#include "ui_inbox.h"

#include <iostream>

inbox::inbox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inbox)
{
    ui->setupUi(this);
	ptLoop = new QEventLoop(this);
	mgr.FetchMails();
	connect(ui->pushButton_write, SIGNAL(clicked()), this, SLOT(OnWriteClicked()));
	connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(OnTreeChosen()));
	connect(ui->pushButton_quit, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
	connect(ui->pushButton_logout, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
}

inbox::~inbox()
{
    delete ui;
	delete ptLoop;
	for (auto &v : vc) {
		delete v;
	}
	for (auto &v : vb) {
		delete v;
	}
}

bool inbox::exec()
{
	setWindowModality(Qt::ApplicationModal);
	show();
	//OnTreeChosen();
	ptLoop->exec();
	return closeResult;
}

void inbox::OnTreeChosen()
{
	cout << "treechosen" <<std::endl;
	for (auto &v : vc) {
		delete v;
	}
	for (auto &v : vb) {
		delete v;
	}

	ListSource source;
	QTreeWidgetItem *item = ui->treeWidget->currentItem();
	if (item->text(0).toStdString() == "ALL") {
		source.type = ListSource::Type::All;
		source.folderId = -1;
	}
	else if (item->text(0).toStdString() == "Unread") {
		source.type = ListSource::Type::Unread;
		source.folderId = -1;
	}
	else if (item->text(0).toStdString() == "Flagged") {
		source.type = ListSource::Type::Flagged;
		source.folderId = -1;
	}
	else if (item->text(0).toStdString() == "Deleted") {
		source.type = ListSource::Type::Spam;
		source.folderId = -1;
	}
	//else if (item->text(0).toStdString() == "") {
	//	source.type = ListSource::Type::Folder;
	//	//	source.folderId = 0;
	//}

	cout << item->text(0).toStdString() << std::endl;

	int count = 0;
	int x0 = ui->scrollArea_2->geometry().x();
	int y0 = ui->scrollArea_2->geometry().y();
	//int w = this->geometry().width();
	//int h = this->geometry().height();
	mgr.FetchMails();
	auto mails = mgr.ListMails(source);
	for (auto& mail : mails) {
		//mail
		QCheckBox* mc = new QCheckBox(this);
		QPushButton* mb = new QPushButton(this);
		vc.push_back(mc);
		vb.push_back(mb);
		mc->setVisible(true);
		mb->setVisible(true);
		//set mc size
		mc->resize(CHECKBOXWIDTH, MAILHEIGHT);
		//place mc
		mc->move(x0, y0 + count * ROWSPACE);
		//set mb size
		mb->resize(BUTTONWIDTH, MAILHEIGHT);
		//place mb
		mc->move(x0 + COLUNMSPACE, y0 + count * ROWSPACE);
		if (1) {
			mc->setChecked(true);
		}
		else {
			mc->setChecked(false);
		}
		mb->setText(QString::fromStdString(mail.GetSubject()));
		connect(mc, SIGNAL(clicked()), this, SLOT(OnReadClicked(mail, mc)));
		connect(mb, SIGNAL(clicked()), this, SLOT(OnMailClicked(mail)));
		
		count++;
	}
	cout << count << std::endl;
}

void inbox::OnWriteClicked()
{
	send w;
	w.exec();
}

void inbox::OnReadClicked(Mail mail, QCheckBox* c)
{
	if (c->isChecked() == true) {
		c->setChecked(false);
		//mail
	}
	else {
		c->setChecked(true);
		//mail
	}
}

void inbox::OnMailClicked(Mail mail)
{
	QPlainTextEdit* m = new QPlainTextEdit();
	m->appendPlainText(QString::fromStdString(mail.GetContent()));
	m->setFocusPolicy(Qt::NoFocus);
	//m->setEnabled(false);
	m->show();
}

void inbox::OnReturnClicked()
{
	if (sender() == ui->pushButton_logout) {
		closeResult = false;
	}
	else {
		closeResult = true;
	}
	ptLoop->quit();
}