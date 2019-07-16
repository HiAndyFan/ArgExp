# ArgExp
ArgExp是一个用于解析处理程序启动参数的C++类, 使用接口简单, 支持常见的参数覆盖及简单的参数冲突检测. 
**由于本文中程序参数和类中函数的参数容易引起混淆, 本文所有"参数"均指需要解析的程序参数, 以"函数输入"表示函数参数**
## 构造函数
只有一个构造函数, ```ArgExp(bool StrictSymbol = false);``` StrictSymbol设置是否严格处理参数前缀符号, 默认关闭时, 将无视'/'  '\'  '-'三个参数前缀符号的区别, -w /w \w三个参数表示形式将视为同一个参数.
## 基本用法
最基本的用法为:
1. addArgType();//添加参数类型
2. readArg();//读入参数
3. isActived();//查看指定参数是否被使用
4. getValue();//获取指定参数值


int addArgType(const char* ArgStr, bool ValueNeeded, int group = 0), 3个输入分别为参数名, 是否需要参数值, 以及参数组别. 
例如 
```
ping -t -w 200 google.com
```

其中-t, -w是参数名, 200是-w的参数值, -t则是一个无需参数值的参数,  google.com为目标参数.
所有不在参数类型内也不是参数值的项将被视为目标参数, 可用getTargetNum()获取其数目, 用getTarget(id)获取该目标参数的值.
组别是参数的分组, 默认为0, 可用于参数冲突关系的检测, 详见"参数冲突与分组"部分.
函数返回值为该参数类型的索引号., 值为{1,2,3,4,...}, 也可用id("参数名")获得.

 readArg(int argc, char* argv[])函数读入需要解析的参数个数和参数内容, 形式与main函数传入的参数保持一致, 一般情况下都可直接传入.

bool isActived(int id)函数用于查看指定参数是否被用户使用, 函数输入为参数类型的索引号.

const char* getValue(int id)函数用于查看指定参数的参数值, 若无则返回空字符串, 函数输入为参数类型的索引号.

## 例程
``` C++
int main(int argc, char* argv[])
{
	ArgExp a;
	a.addArgType("-t", false);
	a.addArgType("-a", false);
	a.addArgType("-f", false);
	a.addArgType("-n", true);
	a.addArgType("-l", true);
	a.readArg(argc, argv);
	a._PrintAll();//此函数专为调试用, 只有在debug模式下才有输出.
	std::cout << "value of n is:" << a.getValue(a.id("n"));
}
```
![](https://i.loli.net/2019/07/16/5d2ded4e3e13d59252.png)

## 所有接口函数
``` C++
int addArgType(const char* ArgStr, bool ValueNeeded, int group = 0);//添加参数类型
int OverwriteArg(const char* ArgName, int n, ...);//声明参数覆盖关系
int readArg(int argc, char* argv[]); //读取运行时参数, 检查运行时参数是否合法, 正常返回0, 参数冲突时返回问题参数的索引号
int checkWit();//检查并执行覆盖关系, 成功返回0
int checkCfl();//检查参数冲突情况, 正常返回0, 参数冲突时返回问题参数的索引号
int id(const char* inArgName);//获取参数类型的索引号, 查无则返回0
const char* ArgName(int id);//查询索引号对应参数名, 查无返回空字符串
int isActived(int id);//根据索引号查询参数是否被使用
const char* getValue(int id);//根据索引号查询参数值
int getTargetNum();//获取目标参数的个数
const char* getTarget(int target_x);//获取第x个目标参数
int _PrintAll();//调试用, 打印所有参数解析结果, 此函数仅在调试模式下工作.
```

## 参数冲突与分组
对ipv6的连接参数不应该与ipv4的地址参数同时存在, 若参数类型之间有功能上的冲突, 分组提供了一个简单的功能冲突检测功能, 在使用addArgType()函数添加参数类型时, 默认提供的分组为group=0, 该分组不与其他任何分组有冲突关系. 函数的第三个输入可以手动指定分组号,  一个参数名只能对应一个分组号. 各分组之间的参数为冲突关系, 不可同时混用,若参数表出现冲突时, addArgType()返回冲突参数的索引号. checkCfl()可手动检查冲突情况, 返回冲突参数的索引号.

## 参数覆盖关系
int OverwriteArg(const char* ArgName, int n, ...)可声明参数覆盖关系, 所谓参数覆盖关系即是目标参数将覆盖掉一个或多个参数类型, 使之失效. 这种覆盖关系往往源于功能逻辑, 例如:关闭灯光的参数自然就覆盖了调亮灯光的参数.函数的第一个输入为目标函数, 第二个输入为被覆盖参数的参数名, 后面的可变输入为被覆盖参数的索引号. 参数覆盖的检查将先于参数冲突的检查, 即如果某参数因为覆盖关系被禁用, 则不会再触发冲突关系. 此外, 对于同一个类型的参数, 最后的参数值自然覆盖前面的参数值.

## 带有参数覆盖与冲突关系的例程
``` C++
#include <iostream>
#include "ArgExp.h"
int main(int argc, char* argv[])
{
	ArgExp a;
	a.addArgType("-t", false);
	a.addArgType("-a", false);
	a.addArgType("-n", true);
	a.addArgType("-l", true);
	a.addArgType("-f", false);
	a.OverwriteArg("-f",2,"-a");
	debug(a.readArg(argc, argv));
	a.readArg(argc, argv);
	debug(a.getValue(a.id("n")));
	a._PrintAll();
	std::cout << "value of n is:" << a.getValue(a.id("n"));
}
```
![](https://i.loli.net/2019/07/16/5d2ded089ccf169814.png
)