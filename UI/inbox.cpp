#include "UI/inbox.h"
#include "ui_inbox.h"

#include <iostream>///

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
	for (auto &v : vr) {
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
	ptLoop->exec();
	return closeResult;
}

void inbox::OnTreeChosen()
{
	cout << "treechosen" <<std::endl;///
	for (auto &v : vr) {
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

	cout << item->text(0).toStdString() << std::endl;///

	int count = 0;
	int x0 = ui->scrollArea_2->geometry().x();
	int y0 = ui->scrollArea_2->geometry().y();
	//int x0 = 0;
	//int y0 = 0;
	//int w = this->geometry().width();
	//int h = this->geometry().height();
	mgr.FetchMails();
	auto mails = mgr.ListMails(source);
	for (auto& mail : mails) {
		//mail
		count++;

		QCheckBox* mr = new QCheckBox(this);
		QPushButton* mb = new QPushButton(this);
		vr.push_back(mr);
		vb.push_back(mb);
		mr->show();
		mb->show();
		mr->resize(CHECKBOXWIDTH, MAILHEIGHT);
		mr->move(x0, y0 + count * (MAILHEIGHT + ROWSPACE));
		mb->resize(BUTTONWIDTH, MAILHEIGHT);
		mb->move(x0 + CHECKBOXWIDTH + COLUNMSPACE, y0 + count * (MAILHEIGHT + ROWSPACE));
		if (1) {
			mr->setChecked(true);
		}
		else {
			mr->setChecked(false);
		}
		mr->setText("unread");
		mb->setText(QString::fromStdString(mail.GetSubject()));
		connect(mr, SIGNAL(clicked()), this, SLOT(OnReadClicked(mail, mr)));
		connect(mb, SIGNAL(clicked()), this, SLOT(OnMailClicked(mail)));
		
	}
	cout << count << std::endl;///
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
	QDialog* w = new QDialog();
	cout << "OMC" << std::endl;
	QPlainTextEdit* m = new QPlainTextEdit(w);
	m->appendPlainText(QString::fromStdString(mail.GetContent()));
	//m->setFocusPolicy(Qt::NoFocus);
	//m->setEnabled(false);
	m->show();
	w->exec();
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