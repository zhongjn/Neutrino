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
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(OnChooseAll()));
	connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(OnReadAll()));
	connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(OnDeleteClicked()));
	connect(ui->comboBox_2, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnMark()));
	connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnMove()));
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

		MailChoose* mc = new MailChoose(&mail, &cid, this);
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
	}
	else if (item->text(0).toStdString() == "Read") {
		source.type = ListSource::Type::Read;
	}
	else if (item->text(0).toStdString() == "Unread") {
		source.type = ListSource::Type::Unread;
	}
	else if (item->text(0).toStdString() == "Flagged") {
		source.type = ListSource::Type::Flagged;
	}
	else if (item->text(0).toStdString() == "Deleted") {
		source.type = ListSource::Type::Spam;
	}
	else if (item->text(0).toStdString() == "Folder") {
		source.type = ListSource::Type::Folder;
		//	source.folderId = 0;
	}
	else {
		source.type = ListSource::Type::All;
	}

	return source;
}

void inbox::OnChooseAll()
{
	if (cid.size() < mails.size()) {
		for (vector<Mail>::iterator iter = mails.begin(); iter != mails.end(); iter++) {
			bool find = false;
			for (vector<int>::iterator i = cid.begin(); i != cid.end(); i++) {
				if (*i == iter->GetId()) {
					find = true;
					break;
				}
			}
			if (find == false) {
				cid.push_back(iter->GetId());
			}
		}
	}
	else {
		cid.clear();
	}
	MailSearch(false);
}

void inbox::OnReadAll()
{
	for (vector<Mail>::iterator iter = mails.begin(); iter != mails.end(); iter++) {
		mgr.SetMailRead(iter->GetId(), true);
	}
	MailSearch(false);
}

void inbox::OnDeleteClicked()
{
	for (vector<int>::iterator iter = cid.begin(); iter != cid.end(); iter++) {
		;
	}
	MailSearch(false);
}

void inbox::OnMark()
{
	if (ui->comboBox_2->currentIndex() == 1) {
		for (vector<int>::iterator iter = cid.begin(); iter != cid.end(); iter++) {
			mgr.SetMailRead(*iter, true);
		}
	}
	else if (ui->comboBox_2->currentIndex() == 2) {
		for (vector<int>::iterator iter = cid.begin(); iter != cid.end(); iter++) {
			mgr.SetMailRead(*iter, false);
		}
	}
	else if (ui->comboBox_2->currentIndex() == 3) {
		for (vector<int>::iterator iter = cid.begin(); iter != cid.end(); iter++) {
			mgr.SetMailFlag(*iter, false);
		}
	}
	else if (0) {
		;
	}
	ui->comboBox_2->setCurrentIndex(0);
	MailSearch(false);
}

void inbox::OnMove()
{

}