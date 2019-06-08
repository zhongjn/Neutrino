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
	for (auto &v : vm) {
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
	for (auto &v : vr) {
		delete v;
	}
	for (auto &v : vm) {
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

	int count = 0;
	int x0 = ui->scrollArea_2->geometry().x();
	int y0 = ui->scrollArea_2->geometry().y();
	//int w = this->geometry().width();
	//int h = this->geometry().height();
	mgr.FetchMails();
	auto mails = mgr.ListMails(source);
	for (auto& mail : mails) {
		//mail
		count++;

		MailRead* mr = new MailRead(mail, this);
		MailMore* mm = new MailMore(mail, this);
		vr.push_back(mr);
		vm.push_back(mm);
		mr->show();
		mm->show();
		mr->resize(CHECKBOXWIDTH, MAILHEIGHT);
		mr->move(x0, y0 + count * (MAILHEIGHT + ROWSPACE));
		mm->resize(BUTTONWIDTH, MAILHEIGHT);
		mm->move(x0 + CHECKBOXWIDTH + COLUNMSPACE, y0 + count * (MAILHEIGHT + ROWSPACE));
		if (1) {
			mr->setChecked(true);
		}
		else {
			mr->setChecked(false);
		}
		mr->setText("unread");
		
	}
	cout << count << std::endl;///
}

void inbox::OnWriteClicked()
{
	send w;
	w.exec();
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