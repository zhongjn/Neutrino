#include "UI/inbox.h"
#include "ui_inbox.h"

inbox::inbox(MailManager m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inbox)
{
    ui->setupUi(this);
	ptLoop = new QEventLoop(this);
	mgr = m;
	mgr.FetchMails();
	connect(ui->treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(OnTreeChosen()));
	//connect(ui->pushButton_27, SIGNAL(clicked()), this, SLOT(OnReturnClicked()));
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

void inbox::exec()
{
	this->setWindowModality(Qt::ApplicationModal);
	this->show();
	OnTreeChosen();
	ptLoop->exec();
}

void inbox::OnTreeChosen()
{
	for (auto &v : vc) {
		delete v;
	}
	for (auto &v : vb) {
		delete v;
	}

	ListSource source;
	QTreeWidgetItem *item = ui->treeWidget->currentItem();
	if (item->text(0) == "ALL") {
		source.type = ListSource::Type::All;
		source.folderId = -1;
	}
	//else if (item->text(0) == "Unread") {
	//	source.type = ;
	//	source.folderId = -1;
	//}
	//else if (item->text(0) == "Flagged") {
	//	source.type = ListSource::Type::Flagged;
	//	source.folderId = -1;
	//}
	else if (item->text(0) == "Deleted") {
		source.type = ListSource::Type::Spam;
		source.folderId = -1;
	}
	else {
		source.type = ListSource::Type::Folder;
	//	source.folderId = 0;
	}

	auto mails = mgr.ListMails(source, ListCondition());
	for (auto& mail : mails) {
		//mail
		QCheckBox* mc = new QCheckBox(this);
		QPushButton* mb = new QPushButton(this);
		vc.push_back(mc);
		vb.push_back(mb);
		//place mc
		//place mb
		//mc setChecked
		mb->setText(mail.GetSubject);
		connect(mc, SIGNAL(clicked()), this, SLOT(OnReadClicked()));
		connect(mb, SIGNAL(clicked()), this, SLOT(OnMailClicked(mail)));
		mc->show();
		mb->show();
	}
}

void inbox::OnReadClicked()
{
	
}

void inbox::OnMailClicked(Mail mail)
{
	QPlainTextEdit* m = new QPlainTextEdit();
	m;
	m->setFocusPolicy(Qt::NoFocus);
	//mail->setEnabled(false);
	m->show();
}

void inbox::OnReturnClicked()
{
	ptLoop->quit();
}