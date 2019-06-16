#ifndef INBOX_DETAIL_H
#define INBOX_DETAIL_H

#include <QWidget>
#include "Mail.h"
#include "send.h"

namespace Ui {
	class inbox_detail;
}

class inbox_detail : public QWidget
{
	Q_OBJECT

public:
	explicit inbox_detail(Mail *mail, QWidget *parent = 0);
	~inbox_detail();

private:
	Ui::inbox_detail *ui;

	Mail *m;
private slots:
	void OnBackClicked();
	void OnReplyClicked();
	void OnDeleteClicked();
	void OnMarkClicked();
};

#endif // INBOX_DETAIL_H