#include "StdAfx.h"
#include "BlackJack.h"
#include <stdlib.h>//随机函数需要的库
#include <time.h>//时间函数需要的库

CBlackJack::CBlackJack(void):m_iTopIndex(0)//构造函数定义
{
	WashCards(m_cCards);//构造器会在类的对象创建时自动调用洗牌函数，完成初始化
	//本类其他成员变量的初始化....
	m_iTopIndex = 0;//打牌计数器初始化
	m_iTotalScores[0] = m_iTotalScores[1] = 0;//积分清零
}

CBlackJack::~CBlackJack(void)//析构函数定义
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//以下是新增成员函数 的定义
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//函数作用：每回合开始自动给每个玩家发两张牌
	//输入参数：cards—洗好的52张牌数组
	//输入输出参数：roundCards—两玩家回合存牌数组，roundCardCnts—两玩家回合存牌计数器数组，topIndex—牌数组中当前面上的牌序号
void CBlackJack::SendCard(char cards[52], char roundCards[2][5], int roundCardCnts[2], int& topIndex)
{
	//给玩家0发牌
	roundCards[0][0] = cards[topIndex++];
	roundCards[0][1] = cards[topIndex++];
	roundCardCnts[0]=2;
	
	//给玩家1发牌
	roundCards[1][0] = cards[topIndex++];
	roundCards[1][1] = cards[topIndex++];
	roundCardCnts[1]=2;
	

	

}



//函数作用：牌序转牌面字符
//输入参数：order—牌序号，A牌序号为0，2牌序号为1，...K牌序号为12
//返回值：牌面字符
char CBlackJack::Order2Char(int order)
{
	char c;
	switch(order + 1)//牌的最小面值是1,即A,j+1就是牌面
	{
		case 1:
			c = 'A';
			break;
		case 10:
			c = 'D';//用虚拟字符D代替10
			break;
		case 11:
			c = 'J';
			break;
		case 12:
			c = 'Q';
			break;
		case 13:
			c = 'K';
			break;
		default:
			c = 0x30 + order + 1;//2~9数字转字符只需加0x30
			break;
	}
	return c;		
}

//函数作用：牌面字符转牌序
//输入参数：c—牌面字符
//返回值：牌序，A牌序为0，2牌序为1，...K牌序为12
int CBlackJack::Char2Order(char c)
{
	int order;//牌序0-12
	switch(c)
	{
		case 'A'://花牌A
			order = 0;
			break;
		case 'D'://牌面10、J、Q、K
			order = 9;
			break;
		case 'J':
			order = 10;
			break;
		case 'Q':
			order = 11;
			break;
		case 'K':
			order = 12;
			break;
		default://牌面2~9
			order = c - 0x30 - 1;//2~9字符转数字，再-1
			break;
	}
	return order;
}

//函数作用：产生随机洗牌数组
//输出参数：cards—洗好的52张牌数组
void CBlackJack::WashCards(char cards[52])
{
	
	const char pip[13] = { 'A','2','3','4','5','6','7','8','9','D','J','Q','K' };
	for (int n = 0; n < 52; n++)
	{
		cards[n] = pip[(n % 13)];
	}
	int i, j;
	srand((unsigned)time(NULL));
	for (int n = 0; n < 52; n++)
	{
		char c = 0;
		i = rand() % 52;
		j = rand() % 52;
		c = cards[i];
		cards[i] = cards[j];
		cards[j] = c;
	}

}

//函数作用：某个玩家拿一张牌的过程（包括提示拿牌、拿牌、亮牌三个过程）
//输入参数：id—玩家ID（0或1），cards—洗好的52张牌数组
//输入输出参数：myCards—该玩家回合存牌数组，myCnt—该玩家回合存牌计数器，topIndex—牌数组中当前面上的牌序号
//返回值：执行本函数后，玩家是否还有要牌权
bool CBlackJack::TakeCard(int id, char cards[52], char myCards[5], int &myCnt,  int &topIndex)
{	
	//系统提示拿牌
	std::cout << "玩家"<<id + 1<<"：按下\'y\'键拿牌或按下\'n\'键放弃：" ;	
	//等待玩家回应
	char c;
	std::cin>>c;
	fflush(stdin);
	//拿牌并亮牌
	bool bRet = c == 'y';//确定是否有要牌权
	if(bRet)
	{
		//拿牌
		myCards[myCnt] = cards[topIndex];
		myCnt++;//回合牌计数器递增
		topIndex++;//牌数组面牌序号递增			
		//亮牌
		ShowCards(id, myCards, myCnt);	
		std::cout << endl;//换行
	}
	return bRet;
}

//函数作用：显示某玩家手上牌（亮牌）
//输入参数：id—玩家ID（0或1），myCards—该玩家回合存牌数组，myCnt—该玩家回合存牌计数器
//返回值：执行本函数后，玩家是否还有要牌权
void CBlackJack::ShowCards(int id, char myCards[5], int myCnt)
{	
	std::cout << "玩家"<<id + 1<<"牌：" ;
	for(int i = 0; i < myCnt; i++)
	{
		if(myCards[i] == 'D')//虚拟字符D的特殊处理
			std::cout << "10" << "  ";
		else//其他字符原样输出
			std::cout <<myCards[i]<< "  ";		
	}	
}

//计算某个玩家当前手上牌点数
//输入参数：myCards—该玩家回合存牌数组，myCnt—该玩家回合存牌计数器
//返回值：该玩家当前手上牌点数
int CBlackJack::CalcuPoint(char myCards[5], int myCnt)
{
	int point = 0, a = 0;//总牌点和A牌的张数
	//统计总点数和A牌张数
	
	for (int n = 0; n < myCnt; n++)
	{
		if (myCards[n] == 'A')
		{
			a += 1;
			point += 1;
		}
		else if (myCards[n] == '2' ||
			myCards[n] == '3' ||
			myCards[n] == '4' ||
			myCards[n] == '5' ||
			myCards[n] == '6' ||
			myCards[n] == '7' ||
			myCards[n] == '8' ||
			myCards[n] == '9')
		{
			point += (myCards[n] - 48);//利用ASCⅡ码表示牌面为2~9的牌点
		}
		else point += 10;
	}


	//试凑法：对A牌做处理，在不超过21点的情况下取最大值
	
	if (a > 0)//判断是否有A
	{
		for (int n = a; n > 0; n--)
		{
			if ((point + 10) > 21)//逐步将A的值由1变为11。如果变为11后，点数大于21，那么取消操作，同时退出A的值由1变为11的循环
				break;
			else point += 10;
		}
	}


	return point;	
}

//函数作用：裁决谁赢了本回合
//输入参数：points—两玩家手上牌点数
//返回值：赢得本回合的玩家ID，0或1，若战平则返回-1（表示没有胜利者）
int CBlackJack::WhoWinRound(int points[2])
{
	if(points[0] == points[1])//平局
		return -1;
	else if(points[0] <= 21 && points[1] <= 21)//双方都未爆，牌点大者胜
		return points[1] > points[0] ? 1 : 0;
	else if(points[0] > 21 && points[1] > 21)//双方都爆，牌点小者胜
		return points[1] < points[0] ? 1 : 0;
	else//一方爆另一方未爆，则未爆者胜
		return points[1] <= 21 ? 1 : 0;	
}

//函数作用：使用一副洗好的牌，完整整个21点双人对战的打牌过程
void CBlackJack::Play()
{
	//提示21点双人对战开始
	std::cout << "牌已洗好，欢迎进入21点双人对战！" << endl;
	//进行回合循环过程控制
	int iRoundCnt = 0;//回合计数器
	while(m_iTopIndex < 52)//计牌器未数到52（也就是牌还未打完）
	{
		//本回合开始提示
		iRoundCnt++;
		std::cout << "回合" << iRoundCnt << "开始：";
		//自动交替给每人发两张牌
		SendCard(m_cCards, m_cRoundCards, m_iRoundCardCnts, m_iTopIndex);
		for(int i = 0; i < 2; i++)//调用函数显示双方牌面和牌点
			ShowCards(i, m_cRoundCards[i], m_iRoundCardCnts[i]);//亮牌
		std::cout << endl;
		//本回合的拿牌过程
		m_bTakeRights[0] = m_bTakeRights[1] = true;//本回合拿牌前两人都有要牌权
		for(int j = 2; j < 5 && (m_bTakeRights[0] || m_bTakeRights[1]); j++)//只要两人之一有要牌权，本回合就继续
		{
			for(int i = 0; i < 2; i++)//遍历两人
				if(m_bTakeRights[i])//只对有要牌权的人给提示
					m_bTakeRights[i] = m_iTopIndex < 52 ? TakeCard(i, m_cCards, m_cRoundCards[i], m_iRoundCardCnts[i], m_iTopIndex) : false;//拿牌并更新拿牌权			
		}
		//本回合结束，计算牌点和输赢
		int points[2];
		for(int i = 0; i < 2; i++)//遍历两人
			points[i] = CalcuPoint(m_cRoundCards[i], m_iRoundCardCnts[i]);//计算牌点
		int iRoundWinner = WhoWinRound(points);//裁决谁赢了本回合
		if(iRoundWinner >= 0)//非平局
			m_iTotalScores[iRoundWinner]++;//赢者加分
		//显示本回合牌面和牌点
		std::cout << "回合" << iRoundCnt << "结束：";//提示回合结束
		for(int i = 0; i < 2; i++)//调用函数显示双方牌面和牌点
		{
			ShowCards(i, m_cRoundCards[i], m_iRoundCardCnts[i]);//牌面
			std::cout << "计"<<points[i]<<"点；  ";//牌点			
		}
		std::cout << endl;//换行
		//显示回合胜负和累计比分
		if(iRoundWinner >= 0)//非平局
			std::cout << "本回合玩家" << iRoundWinner + 1 <<"胜，";
		else//平局
			std::cout << "本回合两玩家和，";
		std::cout << "当前累计比分" << m_iTotalScores[0] <<  " : " << m_iTotalScores[1] << endl;	
		std::cout << endl;//回合结束空一行
	}
	//牌已拿完，确定胜者
	int winner;
	if(m_iTotalScores[0] == m_iTotalScores[1])
		winner = -1;		
	else
		winner = m_iTotalScores[1] > m_iTotalScores[0] ? 1 : 0;
	// 提示游戏结束和最后输赢情况
	std::cout << "游戏结束，" << "最终比分" << m_iTotalScores[0] <<  " : " << m_iTotalScores[1] << "，";//比赛结束和游戏比分提示
	if(winner < 0)//平局
		cout << "两玩家和";
	else
		cout << "恭喜" << "玩家" << winner + 1 <<"赢得比赛";
}

///////////////////////////////////////////////////////////////
//以下是对Play函数的进一步提炼，为便于升级到CAI21类
///////////////////////////////////////////////////////////////

//函数作用：使用一副洗好的牌，完整整个21点双人对战的打牌过程，提炼成双人对战和人机对战的几个共性函数调用
//函数说明：Play函数经过共性提炼后的Play2函数，功能完全相同，但形式更为简单，几乎可以聚焦于拿牌函数的升级
void CBlackJack::Play2()
{
	//提示21点双人对战开始
	std::cout << "牌已洗好，欢迎进入21点双人对战！" << endl;
	//进行回合循环过程控制
	int iRoundCnt = 0;//回合计数器
	while(m_iTopIndex < 52)//计牌器未数到52（也就是牌还未打完）
	{
		//本回合开始提示，并自动交替给每人发两张牌
		RoundBegin(iRoundCnt);
		//本回合的拿牌过程
		m_bTakeRights[0] = m_bTakeRights[1] = true;//本回合拿牌前两人都有要牌权
		for(int j = 2; j < 5 && (m_bTakeRights[0] || m_bTakeRights[1]); j++)//只要两人之一有要牌权，本回合就继续
		{
			for(int i = 0; i < 2; i++)//遍历两人
				if(m_bTakeRights[i])//只对有要牌权的人给提示
					m_bTakeRights[i] = m_iTopIndex < 52 ? TakeCard(i, m_cCards, m_cRoundCards[i], m_iRoundCardCnts[i], m_iTopIndex) : false;//拿牌并更新拿牌权			
		}
		//本回合结束，统计输赢并提示
		RoundEnd(iRoundCnt);		
	}
	//游戏结束前的统计和提示
	GameEnd();	
}

//函数作用：回合开始提示和自动发牌
//输入输出参数：iRoundCnt—回合计数器
void CBlackJack::RoundBegin(int &iRoundCnt)
{
	//本回合开始提示
	iRoundCnt++;
	std::cout << "回合" << iRoundCnt << "开始：";
	//自动交替给每人发两张牌
	SendCard(m_cCards, m_cRoundCards, m_iRoundCardCnts, m_iTopIndex);
	for(int i = 0; i < 2; i++)//调用函数显示双方牌面和牌点
		ShowCards(i, m_cRoundCards[i], m_iRoundCardCnts[i]);//亮牌
	std::cout << endl;
}

//函数作用：回合结束统计和提示输赢情况
//输入参数：iRoundCnt—回合计数器
void CBlackJack::RoundEnd(int iRoundCnt)
{
	//本回合结束，计算牌点和输赢
	int points[2];
	for(int i = 0; i < 2; i++)//遍历两人
		points[i] = CalcuPoint(m_cRoundCards[i], m_iRoundCardCnts[i]);//计算牌点
	int iRoundWinner = WhoWinRound(points);//裁决谁赢了本回合
	if(iRoundWinner >= 0)//非平局
		m_iTotalScores[iRoundWinner]++;//赢者加分
	//显示本回合牌面和牌点
	std::cout << "回合" << iRoundCnt << "结束：";//提示回合结束
	for(int i = 0; i < 2; i++)//调用函数显示双方牌面和牌点
	{
		ShowCards(i, m_cRoundCards[i], m_iRoundCardCnts[i]);//牌面
		std::cout << "计"<<points[i]<<"点；  ";//牌点			
	}
	std::cout << endl;//换行
	//显示回合胜负和累计比分
	if(iRoundWinner >= 0)//非平局
		std::cout << "本回合玩家" << iRoundWinner + 1 <<"胜，";
	else//平局
		std::cout << "本回合两玩家和，";
	std::cout << "当前累计比分" << m_iTotalScores[0] <<  " : " << m_iTotalScores[1] << endl;	
	std::cout << endl;//回合结束空一行
}

//函数作用：游戏结束提示输赢情况
void CBlackJack::GameEnd()
{
	//牌已拿完，确定胜者
	int winner;
	if(m_iTotalScores[0] == m_iTotalScores[1])
		winner = -1;		
	else
		winner = m_iTotalScores[1] > m_iTotalScores[0] ? 1 : 0;
	// 提示游戏结束和最后输赢情况
	std::cout << "游戏结束，" << "最终比分" << m_iTotalScores[0] <<  " : " << m_iTotalScores[1] << "，";//比赛结束和游戏比分提示
	if(winner < 0)//平局
		cout << "两玩家和";
	else
		cout << "恭喜" << "玩家" << winner + 1 <<"赢得比赛";
}
