#ifndef SEND_H
#define SEND_H

#pragma execution_character_set("UTF-8")
#include <QWidget>
#include <QEventLoop>
#include <QString>
#include <QFileDiaLog>
#include <QMessageBox>
#include "UI/Common.h"
#include "Nullable.h"
#include "Mail.h"
#include "MailManager.h"

namespace Ui {
class send;
}

class send : public QWidget
{
    Q_OBJECT

public:
    explicit send(QWidget *parent = 0);
    ~send();
	void exec();
private:
    Ui::send *ui;
	QEventLoop *ptLoop;
	Nullable<string> file;
private slots:
	void OnSendClicked();
	void OnReturnClicked();
	void OnAttachClicked();
	void OnClearClicked();
};

#endif // SEND_H
