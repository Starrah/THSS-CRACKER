#include "FiveChess.h"

#include "define.h"
#include "searchmove.h"
#include "makemove.h"
#include "gameover.h"
#include<iostream>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<QString>
#include<QKeyEvent>
#include<QImage>
#include<QLabel>
#include<time.h>
#include<stdlib.h>
#include<QMessageBox>

using namespace std;


int isstart = 0;//判断游戏是否开始
QImage myimage;

FiveChess::FiveChess(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowTitle(u8"○欢乐五子棋●");
	ui.label = new MyLabel(ui.centralWidget);
	ui.chessborad->setMouseTracking(true);
	ui.repeat->hide();
	connect(ui.label, SIGNAL(Update()), this, SLOT(updatechess()));
	connect(ui.newblack, SIGNAL(clicked()), this, SLOT(newblack()));
	connect(ui.newwhite, SIGNAL(clicked()), this, SLOT(newwhite()));
	connect(ui.repeat, SIGNAL(clicked()), this, SLOT(unmake()));
}
void FiveChess::updatechess() {
	
	QPixmap temp = QPixmap::fromImage(myimage);
	QPainter painter(&myimage);
	QPen pen;
	pen.setColor(QColor(0, 0, 0));
	pen.setWidth(1);
	painter.setPen(pen);
	for (int i = 10; i <= 440; i += 30) {
		QPoint p1(i, 10);
		QPoint p2(i, 430);
		QPoint p3(10, i);
		QPoint p4(430, i);
		painter.drawLine(p1, p2);
		painter.drawLine(p3, p4);
	}

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			if (chessBoard[i][j] == 1) {
				painter.setBrush(QBrush(QColor(0,0,0)));
				QPoint p((j-1) * 30 + 10, (i-1) * 30 + 10);
				painter.drawEllipse(p, 10, 10);
			}
			else if (chessBoard[i][j] == 2) {
				painter.setBrush(QBrush(QColor(255, 255, 255)));
				QPoint p((j-1) * 30 + 10, (i-1) * 30 + 10);
				painter.drawEllipse(p, 10, 10);
			}
		}
	}

	ui.chessborad->setPixmap(temp);

}
void FiveChess::updateboard() {
	myimage = QImage(440, 440, QImage::Format_RGB32);
	myimage.fill(QColor(255, 228, 181));
	QPixmap temp = QPixmap::fromImage(myimage);
	QPainter painter(&myimage);
	QPen pen;
	pen.setColor(QColor(0, 0, 0));
	pen.setWidth(1);
	painter.setPen(pen);
	for (int i = 10; i <= 440; i += 30) {
		QPoint p1(i, 10);
		QPoint p2(i, 430);
		QPoint p3(10, i);
		QPoint p4(430, i);
		painter.drawLine(p1, p2);
		painter.drawLine(p3, p4);
	}

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			if (chessBoard[i][j] == 1) {
				painter.setBrush(QBrush(QColor(0, 0, 0)));
				QPoint p((j - 1) * 30 + 10, (i - 1) * 30 + 10);
				painter.drawEllipse(p, 10, 10);
			}
			else if (chessBoard[i][j] == 2) {
				painter.setBrush(QBrush(QColor(255, 255, 255)));
				QPoint p((j - 1) * 30 + 10, (i - 1) * 30 + 10);
				painter.drawEllipse(p, 10, 10);
			}
		}
	}
	ui.chessborad->setPixmap(temp);

}
void FiveChess::newblack() {
	isstart = 1;
	InitializeGame("newblack");
	ui.newblack->hide();
	ui.newwhite->hide();
	ui.repeat->show();
}
void FiveChess::newwhite() {
	isstart = 1;
	InitializeGame("newwhite");
	ui.newblack->hide();
	ui.newwhite->hide();
	ui.repeat->show();
	updateboard();
	updatechess();
}
void FiveChess::unmake() {
	if (unMakeMove() == 0) {
		QMessageBox::information(NULL, "Error", u8"不可再悔棋！");
	}
	else {
		updateboard();
		updatechess();
	}
}
void FiveChess::replays() {
	int PlayerFlag = 0;
	int ComputerFlag = 0;
	int CurrentPrint = black;
	QString temp;
	for (int i = 0; i < PlayerList.size() + ComputerList.size(); i++)
	{
		temp = "";
		if (CurrentPrint == Player)
		{
			int x = PlayerList[PlayerFlag].x;
			int y = PlayerList[PlayerFlag].y;
			chessBoard[x][y] = Player;
			temp += u8"第 ";
			temp += QString::number(i + 1, 10, 0)+ u8"步，玩家落子于(";
			temp += QString::number(x, 10, 0) + "," + QString::number(y, 10, 0) + u8")位置";
			PlayerFlag++;
			CurrentPrint = Opposite(CurrentPrint);
		}
		else if (CurrentPrint == Computer)
		{
			int x = ComputerList[ComputerFlag].x;
			int y = ComputerList[ComputerFlag].y;
			chessBoard[x][y] = Computer;
			temp += u8"第 ";
			temp += QString::number(i + 1, 10, 0) + u8"步，电脑落子于(";
			temp += QString::number(x, 10, 0) + "," + QString::number(y, 10, 0) + u8")位置";
			ComputerFlag++;
			CurrentPrint = Opposite(CurrentPrint);
		}
		ui.textBrowser->append(temp);
	}
	if (GameResult == Player)
	{
		ui.textBrowser->append(u8"最终玩家获胜");
		//cout << "最终玩家获胜" << endl;
	}
	else if (GameResult == Computer)
	{
		ui.textBrowser->append(u8"最终电脑获胜");
		//cout << "最终电脑获胜" << endl;
	}
	else if (GameResult == blank)
	{
		ui.textBrowser->append(u8"最终和棋");
		//cout << "最终和棋" << endl;
	}
}
void FiveChess::mousePressEvent(QMouseEvent* event) {
	if (isstart) {
		int x = event->pos().x() - 40;
		int y = event->pos().y() - 102;
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 16; j++) {
				int tempx = (j - 1) * 30 + 10;
				int tempy = (i - 1) * 30 + 10;
				if (((tempx - x) * (tempx - x) + (tempy - y) * (tempy - y)) <= 50&&chessBoard[i][j]==blank) {
					if (Player == black) {
						chessBoard[i][j] = black;
						Point NewMove(i, j);
						PlayerList.push_back(NewMove);
						updateboard();
						updatechess();
						if (GameOver() == Player) {
							QMessageBox::information(NULL, u8"游戏结束", u8"玩家获胜！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
						else if (GameOver() == Computer) {
							QMessageBox::information(NULL, u8"游戏结束", u8"电脑获胜！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
						else if (GameOver() == -1) {
							QMessageBox::information(NULL, u8"游戏结束", u8"和棋！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
						SwitchSide();
						SearchMove();
						makeMove();
						SwitchSide();
						updateboard();
						updatechess();
						if (GameOver() == Player) {
							QMessageBox::information(NULL, u8"游戏结束", u8"玩家获胜！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
						else if (GameOver() == Computer) {
							QMessageBox::information(NULL, u8"游戏结束", u8"电脑获胜！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
						else if (GameOver() == -1) {
							QMessageBox::information(NULL, u8"游戏结束", u8"和棋！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
					}
					else
					{
						chessBoard[i][j] = white;
						Point NewMove(i, j);
						PlayerList.push_back(NewMove);
						updateboard();
						updatechess();
						if (GameOver() == Player) {
							QMessageBox::information(NULL, u8"游戏结束", u8"玩家获胜！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
						else if (GameOver() == Computer) {
							QMessageBox::information(NULL, u8"游戏结束", u8"电脑获胜！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
						else if (GameOver() == -1) {
							QMessageBox::information(NULL, u8"游戏结束", u8"和棋！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
						SwitchSide();
						SearchMove();
						makeMove();
						SwitchSide();
						updateboard();
						updatechess();
						if (GameOver() == Player) {
							QMessageBox::information(NULL, u8"游戏结束", u8"玩家获胜！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
						else if (GameOver() == Computer) {
							QMessageBox::information(NULL, u8"游戏结束", u8"电脑获胜！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
						else if (GameOver() == -1) {
							QMessageBox::information(NULL, u8"游戏结束", u8"和棋！");
							isstart = 0;
							ui.repeat->hide();
							replays();
							break;
						}
					}
					break;
				}
			}
		}
	}
	
}

MyLabel::MyLabel(QWidget* parent) :QLabel(parent) {
	myimage = QImage(440, 440, QImage::Format_RGB32);
	myimage.fill(QColor(255, 228, 181));
}
void MyLabel::paintEvent(QPaintEvent* event) {

	QLabel::paintEvent(event);
	emit Update();
}