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
	//�Ƿ��ϸ���ǰ�÷���(��:��/w �� -w��Ϊͬһ������)

	int addArgType(const char* ArgStr, bool ValueNeeded, int group = 0);//��Ӳ�������
	int OverwriteArg(const char* ArgName, int n, ...);//�����������ǹ�ϵ
	int readArg(int argc, char* argv[]); //��ȡ����ʱ����, �������ʱ�����Ƿ�Ϸ�, ��������0, ������ͻʱ�������������������
	int checkWit();//��鸲��
	int checkCfl();//��������ͻ, ��������0, ������ͻʱ�������������������
	int id(const char* inArgName);//��ѯ�������͵�������, ���޷���0
	const char* ArgName(int id);//��ѯ�����Ŷ�Ӧ������, ���޷��ؿ��ַ���
	int isActived(int id);//���������Ų�ѯ�����Ƿ񼤻�
	bool Activate(int id);//�����Ƿ�ʹ��, ǿ�м������, ���ɱ����ǹ�ϵ�ر�, ����ָ��Ĭ�ϲ���.
	const char* getValue(int id);//���������Ų�ѯ����ֵ
	int getTargetNum();//��ȡĿ������ĸ���
	const char* getTarget(int target_x);//��ȡ��x��Ŀ�����
	int getIntValue(int id);//������int���ͻ�ȡ����ֵ
	int setIntBy(int& pin, const char* ArgName);//���ô���int����Ϊ��������Ӧ�Ĳ�����ֵ, ������Ч����, ����0
	
	//����:Ŀ������ǲ��� - / \ ��ͷ�Ĳ��ڲ���ģ���ڵĲ�����

	int _PrintAll();//������, ��ӡ���в����������


};