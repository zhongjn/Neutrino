#include "UI/inbox.h"
#include "ui_inbox.h"


inbox::inbox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inbox)
{
    ui->setupUi(this);
	ptLoop = new QEventLoop(this);
	block = false;
	mgr->FetchMails();

	QTreeWidgetItem *item = FindItemFolder("Folder");
	vector<Folder> folders = mgr->ListFolders();
	for (auto& folder : folders) {		
		QTreeWidgetItem *i = new QTreeWidgetItem(item, QStringList(QString::fromStdString(folder.GetName())));
		ui->comboBox_move->addItem(folder.GetName().c_str());
	}

	connect(ui->pushButton_write, SIGNAL(clicked()), this, SLOT(OnWriteClicked()));
	connect(ui->pushButton_quit, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
	connect(ui->pushButton_logout, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
	connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(OnTreeChosen()));
	connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(OnSearchEnter()));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(OnChooseAll()));
	connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(OnReadAll()));
	connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(OnDeleteClicked()));
	connect(ui->comboBox_mark, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnMark()));
	connect(ui->comboBox_move, SIGNAL(currentIndexChanged(QString)), this, SLOT(OnMove()));
	connect(ui->pushButton_new, SIGNAL(clicked()), this, SLOT(FolderNew()));
	connect(ui->pushButton_rename, SIGNAL(clicked()), this, SLOT(FolderRename()));
	connect(ui->pushButton_remove, SIGNAL(clicked()), this, SLOT(FolderRemove()));
	connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(ScroolWidget(int)));
	connect(ui->pushButton_fetch, SIGNAL(clicked()), this, SLOT(OnFetchClicked()));
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
	if (block == false) {
		if (sender() == ui->pushButton_logout) {
			closeResult = false;
		}
		else {
			closeResult = true;
		}
		ptLoop->quit();
	}
	else {
		QMessageBox::warning(this, "WARNING", "Please return to the mailbox first");
	}
}

void inbox::OnTreeChosen()
{
	if (block == false) {
		MailSearch(false);
	}
}

void inbox::OnSearchEnter()
{
	if (block == true) {
		QMessageBox::warning(this, "WARNING", "Please return to the mailbox first");
	}
	else {
		MailSearch(true);
	}
}

void inbox::MailSearch(bool flag)
{
	if (block == true) {
		return;
	}
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

	int count = -1;
	if (flag == false) {
		mails = mgr->ListMails(sour);
	}
	else {
		mails = mgr->ListMails(sour, con);
	}
	for (auto& mail : mails) {
		count++;

		MailChoose* mc = new MailChoose(&mail, &cid, ui->scrollArea_mail);
		MailFlag* mf = new MailFlag(&mail, ui->scrollArea_mail);
		MailRead* mr = new MailRead(&mail, ui->scrollArea_mail);
		MailMore* mm = new MailMore(&mail, &block, [this] { this->OnTreeChosen(); }, ui->scrollArea_mail);
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
		mc->move(COLUNMSPACE, ROWSPACE + count * (MAILHEIGHT + ROWSPACE));
		mr->move(2 * COLUNMSPACE + CHECKBOXWIDTH, ROWSPACE + count * (MAILHEIGHT + ROWSPACE));
		mm->move(3 * COLUNMSPACE + 2 * CHECKBOXWIDTH, ROWSPACE + count * (MAILHEIGHT + ROWSPACE));
		mf->move(4 * COLUNMSPACE + 2 * CHECKBOXWIDTH + BUTTONWIDTH, ROWSPACE + count * (MAILHEIGHT + ROWSPACE));
		
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
	if (item->text(0).toStdString() == "All") {
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
	else if (item->text(0).toStdString() == "Spam") {
		source.type = ListSource::Type::Spam;
	}
	else if (item->text(0).toStdString() == "Folder") {
		source.type = ListSource::Type::Folder;
	}
	else if (item->parent() != NULL) {
		if (item->parent()->text(0).toStdString() == "Folder") {
			source.type = ListSource::Type::Folder;
			source.folderId = FindFolderId(item->text(0).toStdString());
		}
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
		mgr->SetMailRead(iter->GetId(), true);
	}
	MailSearch(false);
}

void inbox::OnDeleteClicked()
{
	for (vector<int>::iterator i = cid.begin(); i != cid.end(); i++) {
		bool spam = false;
		for (vector<Mail>::iterator iter = mails.begin(); iter != mails.end(); iter++) {
			if (iter->GetId() == *i) {
				spam = iter->GetSpam();
				break;
			}
		}
		if (spam == false) {
			mgr->SetMailSpam(*i, true);
		}
		else {
			mgr->DeleteMail(*i);
		}
	}
	MailSearch(false);
}

void inbox::OnMark()
{
	if (ui->comboBox_mark->currentIndex() == 1) {
		for (vector<int>::iterator iter = cid.begin(); iter != cid.end(); iter++) {
			mgr->SetMailRead(*iter, true);
		}
	}
	else if (ui->comboBox_mark->currentIndex() == 2) {
		for (vector<int>::iterator iter = cid.begin(); iter != cid.end(); iter++) {
			mgr->SetMailRead(*iter, false);
		}
	}
	else if (ui->comboBox_mark->currentIndex() == 3) {
		for (vector<int>::iterator iter = cid.begin(); iter != cid.end(); iter++) {
			mgr->SetMailFlag(*iter, true);
		}
	}
	else if (ui->comboBox_mark->currentIndex() == 4) {
		for (vector<int>::iterator iter = cid.begin(); iter != cid.end(); iter++) {
			mgr->SetMailFlag(*iter, false);
		}
	}
	ui->comboBox_mark->setCurrentIndex(0);
	MailSearch(false);
}

void inbox::OnMove()
{
	if (ui->comboBox_move->currentIndex() == 0) {
		return;
	}
	else if (ui->comboBox_move->currentIndex() == 1) {
		for (vector<int>::iterator i = cid.begin(); i != cid.end(); i++) {
			mgr->SetMailFolder(*i, NULL);
		}
	}
	else if (ui->comboBox_move->currentIndex() == 2) {
		for (vector<int>::iterator i = cid.begin(); i != cid.end(); i++) {
			mgr->SetMailSpam(*i, true);
		}
	}
	else if (ui->comboBox_move->currentIndex() == 3) {
		int fid = FolderNew();
		for (vector<int>::iterator i = cid.begin(); i != cid.end(); i++) {
			mgr->SetMailFolder(*i, fid);
		}
	}
	else {
		int fid = FindFolderId(ui->comboBox_move->currentText().toStdString());
		for (vector<int>::iterator i = cid.begin(); i != cid.end(); i++) {
			mgr->SetMailSpam(*i, false);
			mgr->SetMailFolder(*i, fid);
		}
	}
	ui->comboBox_move->setCurrentIndex(0);
	MailSearch(false);
}

int inbox::FolderNew()
{
	QTreeWidgetItem *item = FindItemFolder("Folder");
	string s;
	StringInput in(&s);
	in.exec();
	//cout << s << endl;
	mgr->AddFolder(s);
	QTreeWidgetItem *folder = new QTreeWidgetItem(item, QStringList(QString::fromStdString(s)));
	ui->comboBox_move->addItem(s.c_str());
	return FindFolderId(s);
}

void inbox::FolderRename()
{
	QTreeWidgetItem *item = ui->treeWidget->currentItem();
	if (item == NULL) {
		QMessageBox::warning(this, "WARNING", "Please choose a folder you want to rename");
	}
	else if (item->parent() == NULL) {
		QMessageBox::warning(this, "WARNING", "You can't rename this folder");
	}
	else if (item->parent()->text(0).toStdString() == "Folder") {
		ui->comboBox_move->removeItem(FindFolderIndex(item->text(0).toStdString()));
		
		int fid = FindFolderId(item->text(0).toStdString());
		string s;
		StringInput in(&s);
		in.exec();
		mgr->RenameFolder(fid, s);
		item->setText(0, QString::fromStdString(s));
		ui->comboBox_move->addItem(s.c_str());
	}

}

void inbox::FolderRemove()
{
	QTreeWidgetItem *item = ui->treeWidget->currentItem();
	if (item == NULL) {
		QMessageBox::warning(this, "WARNING", "Please choose a folder you want to rename");
	}
	else if (item->parent() == NULL) {
		QMessageBox::warning(this, "WARNING", "You can't rename this folder");
	}
	else if (item->parent()->text(0).toStdString() == "Folder") {
		int fid = FindFolderId(item->text(0).toStdString());
		mgr->DeleteFolder(fid);
		ui->comboBox_move->removeItem(FindFolderIndex(item->text(0).toStdString()));
		delete item->parent()->takeChild(ui->treeWidget->currentIndex().row());
	}
}

void inbox::ScroolWidget(int value)
{
	double p = static_cast<double>(value) / static_cast<double>(ui->verticalScrollBar->maximum());
	ui->scrollArea_mail->move(0, MAILHEIGHT - (ui->scrollArea_mail->height() - 300) * p);
}

void inbox::WheelEvent(QWheelEvent *event) {
	int p = event->angleDelta().y();    
	if (p < 0) {
		if (ui->verticalScrollBar->value() + WHEELUNIT <= 100) {
			emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->value() + WHEELUNIT);
			ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->value() + WHEELUNIT);
		}
		else {
			emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->maximum());
			ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->maximum());
		}
	}
	else {
		if (ui->verticalScrollBar->value() - WHEELUNIT >= 0) {
			emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->value() - WHEELUNIT);
			ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->value() - WHEELUNIT);
		}
		else {
			emit ui->verticalScrollBar->valueChanged(ui->verticalScrollBar->minimum());
			ui->verticalScrollBar->setSliderPosition(ui->verticalScrollBar->minimum());
		}
	}
}

int inbox::FindFolderId(string fname)
{
	vector<Folder> folders = mgr->ListFolders();
	for (auto& folder : folders) {
		if (folder.GetName() == fname) {
			return folder.GetId();
		}
	}
	return -1;
}

int inbox::FindFolderIndex(string fname)
{
	for (int i = 3; i < ui->comboBox_move->count(); i++) {
		if (ui->comboBox_move->itemText(i) == QString::fromStdString(fname)) {
			return i;
		}
		
	}
	return -1;
}

QTreeWidgetItem *inbox::FindItemFolder(string Folder)
{
	QTreeWidgetItemIterator it(ui->treeWidget);
	while (*it) {
		if ((*it)->text(0) == QString::fromStdString(Folder)) {
			return (*it);
		}
		it++;
	}
	return NULL;
}

void inbox::OnFetchClicked()
{
	mgr->FetchMails();
}