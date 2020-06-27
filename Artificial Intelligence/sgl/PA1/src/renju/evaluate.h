#ifndef _EVALUATE_
#define _EVALUATE_
//估值生成器

//棋型类
struct ChessShape
{
	//棋型数组,1黑子,2白子,0空白,-1越界
	int List[6];
	//数组有效位置长度
	int TotalNum;
};

//得分形状类
struct ScoreShape
{
	//棋型数组,1黑子,2白子,0空白,-1越界
	int List[6];
	//数组有效位置长度
	int TotalNum;
	//坐标数组
	Point Place[6];
	//得分
	int Score;
	//x方向
	int DX;
	//y方向
	int DY;
};
extern std::vector<ScoreShape> ScoreList;

/*
描述：判断当前位置棋型得分
参数：被判断的棋类型, 棋型
返回：得分
*/
extern int EvaluateOneShape(int Evaluated, ChessShape TheShape);

/*
描述：计算某一点的得分
参数：当前点x,y，被估计的棋子颜色
返回：得分
*/
extern int EvaluateOnePoint(int x, int y, int Evaluated);

/*
描述：计算总得分
参数：被估计的棋子颜色
返回：得分
*/
extern int Evaluate(int Evaluated);

/*
描述：判断当前点是否被重复计算
参数：当前点x,y，方向dx，dy
返回：是否
*/
extern bool JudgeRepeat(int x, int y, int dx, int dy);
/*
描述：判断当前点是否在其他方向有得分，有的话计算额外加分
参数：当前点x,y，方向dx，dy，当前得分
返回：额外加分
*/
extern int CalculateAddScore(int x, int y, int dx, int dy, int score);
/*
描述：打印得分列表
参数：当前点x,y，方向dx，dy，当前得分无
返回：无
*/
extern void PrintScoreList();

#endif
