#pragma once
#include "blackjack.h"

//21点人机对战类
class CAI21 : public CBlackJack
{
public:
	CAI21(void);
	~CAI21(void);

	//函数作用：使用一副洗好的牌，完整整个21点双人对战的打牌过程，提炼成双人对战和人机对战的几个共性函数调用
	//函数说明：经过共性提炼后的Play2函数，形式更为简单，几乎可以聚焦于拿牌函数的升级
	void PlayAI();
private:

	//函数作用：判断玩家是否应该拿牌
	//输入参数：id—玩家ID(0或1)，cards—洗好的52张牌数组，roundCards—两玩家回合存牌数组，roundCardCnts—两玩家回合存牌计数器数组，topIndex—牌数组中当前面上的牌序号
	//返回值：是否拿牌
	bool IfTakeCard(int id, char cards[52], char roundCards[2][5], int roundCardCnts[2], int topIndex);

	//函数作用：机器人玩家的拿牌函数（包括提示拿牌、拿牌、亮牌三个过程）
	//输入参数：id—机器人玩家ID（0或1），cards—洗好的52张牌数组
	//输入输出参数：roundCards—两玩家回合存牌数组，roundCardCnts—两玩家回合存牌计数器数组，topIndex—牌数组中当前面上的牌序号
	//返回值：执行本函数后，玩家是否还有要牌权
	bool RobotTakeCard(int id, char cards[52], char roundCards[2][5], int roundCardCnts[2],  int &topIndex);	
};
