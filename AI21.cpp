#include "StdAfx.h"
#include "AI21.h"


CAI21::CAI21(void)
{

}


CAI21::~CAI21(void)
{

}

//函数作用：判断玩家是否应该拿牌的决策函数
//输入参数：id—玩家ID(0或1)，cards—洗好的52张牌数组，roundCards—两玩家回合存牌数组，roundCardCnts—两玩家回合存牌计数器数组，topIndex—牌数组中当前面上的牌序号
//返回值：是否应该拿下一张牌
//函数说明：本函数被机器人拿牌函数RobotTakeCard所调用，RobotTakeCard大体上是与人类拿牌函数TakeCard差不多的，而主要的核心差别就是对本函数的调用取代了玩家1的TakeCard
bool CAI21::IfTakeCard(int id, char cards[52], char roundCards[2][5], int roundCardCnts[2], int topIndex)
{//请在以下分段注释下空白处填写相应的实现代码
	//1、排除必须拿和必须不拿的情况
	//如果已经无牌可拿，则直接返回false
	if (topIndex == 52)
		return false;

	
	//计算自己和对方当前手上牌点
	int point[2];
	for (int n = 0; n < 2; n++)
	{
		point[n] = CalcuPoint(roundCards[n], roundCardCnts[n]);
	}


	//如果自己已经爆了或者正好拿到21点满分，或者对方已经爆了，则肯定不再拿牌，返回false
	if ((point[0] > 21) || (point[0] == 21) || (point[1] > 21))
		return false;


	//在都未爆的情况下，如果自己牌点落后于对方，则无论如何都必须拿牌，返回true
	if ((point[0] < point[1]) && (point[1] < 21))
		return true;


	//2、在当前牌点领先或持平于对方的情况下，依据爆的概率大小决定是否拿牌
	//统计13种面值的剩余牌张数（为避免记忆已经打过的牌，这里直接从topIndex开始统计剩余牌，但要清楚实际打牌的时候是一副牌扣除打过的牌得到剩余牌张及其数量的，即需要记牌）
	

	//生成预测拿牌临时数组（该临时数组仅用于预测拿牌后是否会爆）
	char myCards[5], myCnt = roundCardCnts[id];//我的预测拿牌数组和当前牌数
	for(int i = 0; i < myCnt; i++)
		myCards[i] = roundCards[id][i];//复制当前我手上的牌数组
	//计算拿下一张牌后爆的总概率（可能性）
	float percent=0;//总爆率
	/*int win_times;
	for (int n = (52 - topIndex); n > 0; n--)
	{
		int test_point = CalcuPoint(myCards, (myCnt + 1));
		if (test_point <= 21)
			win_times += 1;
	}*/
	/*int remainderCards;*///AI未模拟拿牌时，剩余的牌数
	int remainderCards = 0;
	remainderCards = 52 - topIndex;
	srand((unsigned)time(NULL));
	for (/*int remainderCards*/; remainderCards > 0; remainderCards--)
	{//先判断数组是否可以容纳下一张牌
		int Temp = rand() % (52 - topIndex);
		Temp += topIndex;
		myCards[(myCnt+1)] = cards[Temp];
		if (CalcuPoint(myCards, (myCnt + 1)) > 0)
			percent++;
		myCards[(myCnt + 1)] = NULL;
	}
	float Temp_ = 52 - (float)topIndex;
	percent /= Temp_;



	//依据爆率是否小于0.5决定是否拿牌，爆率小于0.5则返回true，否则返回false
	return percent < 0.5;
}

//函数作用：机器人玩家的拿牌函数（包括提示拿牌、拿牌、亮牌三个过程）
//输入参数：id—机器人玩家ID（0或1），cards—洗好的52张牌数组
//输入输出参数：roundCards—两玩家回合存牌数组，roundCardCnts—两玩家回合存牌计数器数组，topIndex—牌数组中当前面上的牌序号
//返回值：执行本函数后，玩家是否还有要牌权
bool CAI21::RobotTakeCard(int id, char cards[52], char roundCards[2][5], int roundCardCnts[2],  int &topIndex)
{	
	//系统提示拿牌
	std::cout << "玩家"<<id + 1<<"：按下\'y\'键拿牌或按下\'n\'键放弃：" ;	
	//机器人自动决策拿牌并自动回应
	bool bRet = IfTakeCard(id, cards, roundCards, roundCardCnts, topIndex);//确定是否有要牌权
	std::cout << (bRet ? 'y' : 'n') << endl;//自动显示回应
	//拿牌	
	if(bRet)
	{
		//拿牌
		roundCards[id][roundCardCnts[id]] = cards[topIndex];
		roundCardCnts[id]++;//回合牌计数器递增
		topIndex++;//牌数组面牌序号递增			
		//亮牌
		ShowCards(id, roundCards[id], roundCardCnts[id]);	
		std::cout << endl;//换行
	}
	return bRet;
}

//函数作用：使用一副洗好的牌，完整整个21点人机对战的打牌过程，提炼成双人对战和人机对战的几个共性函数调用
//函数说明：经过共性提炼后的Play2函数升级而来，形式更为简单，几乎可以聚焦于拿牌函数的升级
void CAI21::PlayAI()
{
	//提示21点双人对战开始
	std::cout << "牌已洗好，欢迎进入21点人机对战！" << endl;
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
			//玩家1：机器人玩家的拿牌决策
			if(m_bTakeRights[0])//只对有要牌权的人给提示
				m_bTakeRights[0] = m_iTopIndex < 52 ? RobotTakeCard(0, m_cCards, m_cRoundCards, m_iRoundCardCnts, m_iTopIndex) : false;//拿牌并更新拿牌权			
			//玩家2：人类玩家的拿牌决策
			if(m_bTakeRights[1])//只对有要牌权的人给提示
				m_bTakeRights[1] = m_iTopIndex < 52 ? TakeCard(1, m_cCards, m_cRoundCards[1], m_iRoundCardCnts[1], m_iTopIndex) : false;//拿牌并更新拿牌权			
		}
		//本回合结束，统计输赢并提示
		RoundEnd(iRoundCnt);		
	}
	//游戏结束前的统计和提示
	GameEnd();	
}
