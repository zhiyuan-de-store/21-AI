// ConsoleGame.cpp : 定义控制台应用程序的入口点。
/*框架程序使用说明：
整个程序框架已经搭建起来了，但有三个核心函数的代码未写，需要填写具体实现代码并调通程序
这三个需要在函数定义中充实具体实现代码的函数是CBlackJack::WashCards、CBlackJack::CalcuPoint、CAI21::IfTakeCard
*/

#include "stdafx.h"
#include "Ai21.h"//让本文件的函数认识CBlackJack和CAI21类型

//函数作用：测试洗牌结果，将cards数组的牌逐个打印到屏幕以验证牌是否洗好,仅调试目的
//函数说明：是CBlackJack类的友元函数
void TestCards()
{
	CBlackJack jack;//调用CBlackJack类的默认构造器为类对象jack分配内存空间，并间接实现了jack对象自己处理洗牌
	jack.WashCards(jack.m_cCards);
	for(int i = 0; i < 52; i++)
	{
		if(jack.m_cCards[i] == 'D')//虚拟字符10的特殊处理
			printf("%4d", 10);			
		else//其他字符处理
			printf("   %c", jack.m_cCards[i]);		
		if((i + 1) % 13 == 0)//每13张牌的最后一张
			printf("\n");//换行
	}
}

void main()
{
	//TestCards();//仅测试洗牌用	
	//悬挂对象指针
	CBlackJack *pJack;
	CAI21 *pAI;
	//程序主循环
	while(1)
	{
		//选择游戏	
		printf("请按1键进入双人对战，或按2键进入人机对战，其他键退出程序");
		char c = getchar();
		//更新缓冲区，即清除标准输出缓冲区（一般是指键盘）
		fflush(stdin);
		//根据选择使用不同游戏
		if(c == '1')
		{
			pJack = new CBlackJack;//动态创建CBlackJack对象（动态分配内存空间），并使用默认构造器间接实现洗牌
			//pJack->Play();
			pJack->Play2();
			delete pJack;//删除动态内存
		}
		else if(c == '2')
		{
			pAI = new CAI21;//动态创建CAI21对象（动态分配内存空间），并使用基类CBlackJack的默认构造器间接实现洗牌
			pAI->PlayAI();
			delete pAI;//删除动态内存
		}
		else
			break;		
	}	
}


