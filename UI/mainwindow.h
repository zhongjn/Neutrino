#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma execution_character_set("UTF-8")

#include <QMainWindow>
#include <QString>
#include <QMessageBox>

#include "UI/Common.h"
#include "MailManager.h"

namespace Ui {
	class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void ShowInbox();

private slots:
	void OnSigninClicked();
	void OnCloseClicked();
};

#endif // MAINWINDOW_H
