#pragma once
#define MAX_ARGC 32

#include <string>
#include <stdarg.h>
#include "luck.h"
using namespace std;

struct Argm
{
	string ArgName;
	string ArgValue;
	bool NeedValue = false;
	bool Active = false;
	Argm* OverWitList[MAX_ARGC] = { 0 };
	int OverWitCount = 0;
	int group;
};

class ArgExp
{
private:
	Argm ArgLst[MAX_ARGC];
	int ArgNum = 0;
	int CnfCount[MAX_ARGC] = { 0 };
	bool StrictSymbol = false;

	int addTargetArg(const char* value);
	Argm* getArgmByName(const char* ArgName);
public:
	ArgExp(bool StrictSymbol = false);
	//是否严格处理前置符号(例:将/w 与 -w视为同一个参数)

	int addArgType(const char* ArgStr, bool ValueNeeded, int group = 0);//添加参数类型
	int OverwriteArg(const char* ArgName, int n, ...);//声明参数覆盖关系
	int readArg(int argc, char* argv[]); //读取运行时参数, 检查运行时参数是否合法, 正常返回0, 参数冲突时返回问题参数的索引号
	int checkWit();//检查覆盖
	int checkCfl();//检查参数冲突, 正常返回0, 参数冲突时返回问题参数的索引号
	int id(const char* inArgName);//查询参数类型的索引号, 查无返回0
	const char* ArgName(int id);//查询索引号对应参数名, 查无返回空字符串
	int isActived(int id);//根据索引号查询参数是否激活
	bool Activate(int id);//无论是否使用, 强行激活参数, 但可被覆盖关系关闭, 用于指定默认参数.
	const char* getValue(int id);//根据索引号查询参数值
	int getTargetNum();//获取目标参数的个数
	const char* getTarget(int target_x);//获取第x个目标参数
	int getIntValue(int id);//尝试以int类型获取参数值
	int setIntBy(int& pin, const char* ArgName);//设置传入int变量为参数名对应的参数数值, 若无有效数字, 返回0
	
	//定义:目标参数是不以 - / \ 开头的不在参数模板内的参数组

	int _PrintAll();//调试用, 打印所有参数解析结果


};