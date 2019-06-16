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
	connect(ui->pushButton_quit, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
	connect(ui->pushButton_logout, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
	connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(OnTreeChosen()));
	connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(OnSearchEnter()));
}

inbox::~inbox()
{
    delete ui;
	delete ptLoop;
	for (auto &v : vc) {
		delete v;
	}
	for (auto &v : vf) {
		delete v;
	}
	for (auto &v : vr) {
		delete v;
	}
	for (auto &v : vm) {
		delete v;
	}
	vc.clear();
	vf.clear();
	vr.clear();
	vm.clear();
}

bool inbox::exec()
{
	setWindowModality(Qt::ApplicationModal);
	show();
	ptLoop->exec();
	return closeResult;
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

void inbox::OnTreeChosen()
{
	MailSearch(false);
}

void inbox::OnSearchEnter()
{
	MailSearch(true);
}

void inbox::MailSearch(bool flag)
{
	for (auto &v : vc) {
		delete v;
	}
	for (auto &v : vf) {
		delete v;
	}
	for (auto &v : vr) {
		delete v;
	}
	for (auto &v : vm) {
		delete v;
	}
	vc.clear();
	vf.clear();
	vr.clear();
	vm.clear();

	ListSource sour = GetTreeItem();
	ListCondition con = ListCondition();
    if (flag) con.match_full = ui->lineEdit->text().toStdString();

	int count = 0;
	int x0 = ui->scrollArea_2->geometry().x();
	int y0 = ui->scrollArea_2->geometry().y();//TODO: zoom
	//mgr.FetchMails();
	if (flag == false) {
		mails = mgr.ListMails(sour);
	}
	else {
		mails = mgr.ListMails(sour, con);
	}
	for (auto& mail : mails) {
		//mail
		count++;

		MailChoose* mc = new MailChoose(&mail, &cm, this);
		MailFlag* mf = new MailFlag(&mail, this);
		MailRead* mr = new MailRead(&mail, this);
		MailMore* mm = new MailMore(&mail, this);
		vc.push_back(mc);
		vf.push_back(mf);
		vr.push_back(mr);
		vm.push_back(mm);
		mc->show();
		mf->show();
		mr->show();
		mm->show();
		mc->resize(CHECKBOXWIDTH, MAILHEIGHT);
		mf->resize(CHECKBOXWIDTH, MAILHEIGHT);
		mr->resize(CHECKBOXWIDTH, MAILHEIGHT);
		mm->resize(BUTTONWIDTH, MAILHEIGHT);
		mc->move(x0 + COLUNMSPACE, y0 + count * (MAILHEIGHT + ROWSPACE));
		mr->move(x0 + 2 * COLUNMSPACE + CHECKBOXWIDTH, y0 + count * (MAILHEIGHT + ROWSPACE));
		mm->move(x0 + 3 * COLUNMSPACE + 2 * CHECKBOXWIDTH, y0 + count * (MAILHEIGHT + ROWSPACE));
		mf->move(x0 + 4 * COLUNMSPACE + 2 * CHECKBOXWIDTH + BUTTONWIDTH, y0 + count * (MAILHEIGHT + ROWSPACE));
		
	}
	
}

ListSource inbox::GetTreeItem()
{
	ListSource source;
	QTreeWidgetItem *item = ui->treeWidget->currentItem();
	if (item == NULL) {
		source.type = ListSource::Type::All;
		return source;
	}
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
	else {
		source.type = ListSource::Type::All;
	}

	return source;
}

