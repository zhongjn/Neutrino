#ifndef INBOX_DETAIL_H
#define INBOX_DETAIL_H

#include <QWidget>
#include <QEventLoop>
#include "Mail.h"
#include "UI/send.h"

namespace Ui {
	class inbox_detail;
}

class inbox_detail : public QWidget
{
	Q_OBJECT

public:
	explicit inbox_detail(Mail *mail, QWidget *parent = 0);
	~inbox_detail();
	void exec();
private:
	Ui::inbox_detail *ui;
	Mail *m;
	QEventLoop *ptLoop;
private slots:
	void OnBackClicked();
	void OnReplyClicked();
	void OnDeleteClicked();
	void OnMarkClicked();
};

#endif // INBOX_DETAIL_H