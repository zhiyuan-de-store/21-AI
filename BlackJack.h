#pragma once//保证本.h头文件只被包含一次
#include <iostream>
using namespace std;

class CBlackJack//类声明
{
public://公有成员：可以被类外部访问的成员
	CBlackJack(void);//默认构造函数声明，类必有的初始化函数（出生函数）
	~CBlackJack(void);//析构函数声明，类必有的解体函数（死亡函数）
	void Play();//声明公有玩函数
	friend void TestCards();//声明友元测试函数

protected://保护成员：仅子类和友元可以访问
	
	//函数作用：牌序转牌面字符
	//输入参数：order—牌序号，A牌序号为0,2牌序号为1，...K牌序号为12
	//返回值：牌面字符
	char Order2Char(int order);

	//函数作用：牌面字符转牌序
	//输入参数：c—牌面字符
	//返回值：牌序，A牌序为0，2牌序为1，...K牌序为12
	int Char2Order(char c);

	//函数作用：每回合开始自动给每个玩家发两张牌
	//输入参数：cards—洗好的52张牌数组
	//输入输出参数：roundCards—两玩家回合存牌数组，roundCardCnts—两玩家回合存牌计数器数组，topIndex—牌数组中当前面上的牌序号
	void SendCard(char cards[52], char roundCards[2][5], int roundCardCnts[2], int &topIndex);

	//函数作用：某个玩家拿一张牌的过程（包括提示拿牌、拿牌、亮牌三个过程）
	//输入参数：id—玩家ID（0或1），cards—洗好的52张牌数组
	//输入输出参数：myCards—该玩家回合存牌数组，myCnt—该玩家回合存牌计数器，topIndex—牌数组中当前面上的牌序号
	//返回值：执行本函数后，玩家是否还有要牌权
	bool TakeCard(int id, char cards[52], char myCards[5], int &myCnt,  int &topIndex);

	//函数作用：显示某玩家手上牌
	//输入参数：id—玩家ID（0或1），myCards—该玩家回合存牌数组，myCnt—该玩家回合存牌计数器
	//返回值：执行本函数后，玩家是否还有要牌权
	void ShowCards(int id, char myCards[5], int myCnt);

	//函数作用：计算某个玩家当前手上牌点数
	//输入参数：myCards—该玩家回合存牌数组，myCnt—该玩家回合存牌计数器
	//返回值：该玩家当前手上牌点数
	int CalcuPoint(char myCards[5], int myCnt);

	//函数作用：裁决谁赢了本回合
	//返回值：赢得本回合的玩家ID，0或1，若战平则返回-1（表示没有胜利者）
	int WhoWinRound(int points[2]);	
	
	//新增成员变量（类共享数据抽象）
	char m_cCards[52];//52张牌数组
	int m_iTopIndex;//当前最上面一张牌在m_cCards[52]数组中的序号
	char m_cRoundCards[2][5];//两玩家回合存牌数组
	int m_iRoundCardCnts[2];//两玩家回合存牌数组计数器
	bool m_bTakeRights[2];//两玩家当前是否有要牌权
	int m_iTotalScores[2];//两玩家的累计分数（各自赢的回合数）	

private://私有成员，仅本类调用
	//函数作用：产生随机洗牌数组
	//输出参数：cards—洗好的52张牌数组
	void WashCards(char cards[52]);//洗牌函数	

	///////////////////////////////////////////////////////////////
	//以下是对Play函数的进一步提炼，为便于升级到CAI21类
	///////////////////////////////////////////////////////////////
public:
	//函数作用：使用一副洗好的牌，完整整个21点双人对战的打牌过程，提炼成双人对战和人机对战的几个共性函数调用
	void Play2();

protected:
	//函数作用：回合开始提示和自动发牌
	//输入输出参数：iRoundCnt—回合计数器
	void RoundBegin(int &iRoundCnt);

	//函数作用：回合结束统计和提示输赢情况
	//输入参数：iRoundCnt—回合计数器
	void RoundEnd(int iRoundCnt);

	//函数作用：游戏结束提示输赢情况
	void GameEnd();
};
