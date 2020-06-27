#include <QtWidgets/QMainWindow>
#include "ui_FiveChess.h"
#include<QImage>
#include<QPainter>
#include<QColor>
#include<QPixmap>
#include<QLabel>
#include<QString>

class FiveChess : public QMainWindow
{
	Q_OBJECT

public:
	FiveChess(QWidget *parent = Q_NULLPTR);
	void mousePressEvent(QMouseEvent* event);
	void updateboard();
	void replays();


private:
	Ui::FiveChessClass ui;

private slots:
	void updatechess();
	void newblack();
	void newwhite();
	void unmake();
};



class MyLabel :public QLabel
{
	Q_OBJECT
public:
	MyLabel(QWidget* parent = nullptr);

	void paintEvent(QPaintEvent* event) override;//绘制线条函数


signals:
	void Update();//更新画面上图片绘制状态的函数

};
