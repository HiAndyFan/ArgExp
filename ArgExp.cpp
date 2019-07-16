#include <iostream>
#include "ArgExp.h"
ArgExp::ArgExp(bool StrictSymbol)
{
	this->StrictSymbol = StrictSymbol;
}

int ArgExp::addArgType(const char* ArgStr, bool ValueNeeded, int group)
{
	string ArgStr_s = ArgStr;
	if (!StrictSymbol)
		if ((ArgStr[0] == '/') | (ArgStr[0] == '\\')) ArgStr_s[0] = '-';// 非严格符号要求时替换'/' '\' 前置符为'-'
	ArgLst[ArgNum].ArgName = ArgStr_s;
	ArgLst[ArgNum].NeedValue = ValueNeeded;
	ArgLst[ArgNum].group = group;
	ArgNum++;
	return ArgNum;
}

int ArgExp::addTargetArg(const char* value)
{
	int idx = 0;
	idx = addArgType("___TARGET_VALUE___", false);
	ArgLst[idx - 1].ArgValue = value;
	return idx;
}

Argm* ArgExp::getArgmByName(const char* ArgName)
{
	return &ArgLst[id(ArgName) - 1];
}


int ArgExp::OverwriteArg(const char* ArgName, int n, ...)
{
	va_list valist;
	va_start(valist, n);
	for (int i = 0; i < n; i++)
	{
		Argm * WitObj = getArgmByName(ArgName);
		WitObj->OverWitList[WitObj->OverWitCount++] = getArgmByName(va_arg(valist, char*));
		//witObj->OverWitList[witObj->OverWitCount++] = &ArgLst[va_arg(valist, int)-1];
	}
	va_end(valist);
	return 0;
}

int ArgExp::id(const char* inArgName)
{
	string inArgName_s = inArgName;
	if (!StrictSymbol)
		if ((inArgName[0] == '/') | (inArgName[0] == '\\')) inArgName_s[0] = '-';// 非严格符号要求时替换'/' '\' 前置符为'-'
	if (inArgName_s[0] != '-') inArgName_s.insert(0, "-");
	for (int i = 0; i < ArgNum; i++)
	{
		if (ArgLst[i].ArgName == inArgName_s)
			return i+1;
	}
	return 0;
}

const char* ArgExp::ArgName(int id)
{
	if (id > ArgNum)return "";
	return ArgLst[id-1].ArgName.c_str();
}

int ArgExp::isActived(int id)
{
	return ArgLst[id-1].Active;
}

bool ArgExp::Activate(int id)
{
	ArgLst[id - 1].Active = true;
	checkCfl();
	return isActived(id);
}

const char* ArgExp::getValue(int id)
{
	return ArgLst[id - 1].ArgValue.c_str();
}

int ArgExp::readArg(int argc, char* argv[])
{
	string strArg;
	for (int i = 1; i < argc; i++)
	{
		int idx = 0;
		strArg = argv[i];
		if (!StrictSymbol)
			if ((strArg[0] == '/') | (strArg[0] == '\\')) strArg[0] = '-';// 非严格符号要求时替换'/' '\' 前置符为'-'
		idx = id(strArg.c_str());
		if (idx != 0)
		{
			ArgLst[idx - 1].Active = true;
			if (ArgLst[idx - 1].NeedValue)
			{
				i++;
				ArgLst[idx - 1].ArgValue = argv[i];
			}
		} else {
			addTargetArg(argv[i]);
		}
		
	}

	checkWit();
	return checkCfl();
}

int ArgExp::checkWit()
{
	for (int i = 0; i < ArgNum; i++)
	{
		if (ArgLst[i].Active && ArgLst[i].OverWitCount != 0)
		{
			for (int j = 0; j < ArgLst[i].OverWitCount; j++)
			{
				ArgLst[i].OverWitList[j]->Active = false;
			}
		}
	}
	return 0;
}

int ArgExp::checkCfl()
{
	int group_s = 0;
	for (int i = ArgNum - 1 ; i >= 0; i--)
	{
		if (ArgLst[i].Active && ArgLst[i].group != 0)
		{
			group_s = group_s ? group_s : ArgLst[i].group;
			if (ArgLst[i].group != group_s) return i + 1;
		}

	}
	return 0;
}

int ArgExp::getTargetNum()
{
	int TargetNum = 0;
	for (int i = 0; i < ArgNum; i++)
	{
		if (ArgLst[i].ArgName == "___TARGET_VALUE___")
			TargetNum++;
	}
	return TargetNum;
}

const char* ArgExp::getTarget(int target_x)
{
	int TargetNum = 0;
	for (int i = 0; i < ArgNum; i++)
	{
		if(ArgLst[i].ArgName == "___TARGET_VALUE___")
			TargetNum++;
		if (TargetNum == target_x) return ArgLst[i].ArgValue.c_str();
	}
	return "";
}

int ArgExp::setIntBy(int& pin, const char* ArgName)
{
	if(getArgmByName(ArgName)->Active)
		pin = getIntValue(id(ArgName));
	return pin;
}

int ArgExp::getIntValue(int id)
{
	string val;
	string::iterator itr;
	val = getValue(id);
	itr = val.begin();
	while (itr != val.end()) {
		if ((*itr < '0') || (*itr > '9'))
		{
			val.erase(itr);
		}
		else {
			itr++;
		}
	}
	if (val == "") val = "0";
	return stoi(val);
}

int ArgExp::_PrintAll()
{
	if (isDebug)
	{
		cout << "arg\tActived\tNeedVal\tValue\tGroup\n";
		for (int i = 0; i < ArgNum; i++)
		{
			cout << ArgLst[i].ArgName << "\t"
				<< strBool(ArgLst[i].Active) << "\t"
				<< strBool(ArgLst[i].NeedValue) << "\t"
				<< ArgLst[i].ArgValue.c_str() << "\t"
				<< ArgLst[i].group << endl;
			}
	}
	return 0;
}
