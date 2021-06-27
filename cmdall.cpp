#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <vector>

#include "rl_command.h"
#include "my_command.h"
#include "cmdall.h"

// 去掉前后空格——中间的不去掉
std::string& trim(std::string &str)
{
	if (str.empty())
	{
		return str;
	}
	str.erase(0, str.find_first_not_of(" ")); //去除左边空格
	str.erase(str.find_last_not_of(" ") + 1);//去除右边空格
	return str;
}

// 分割string，能自动去掉分隔符前后的空格
std::vector <std::string> splitString(const std::string & s, const std::string & delim)
{
    std::vector <std::string> elems;
    std::string tmp;
    size_t pos = 0;
    size_t len = s.length();
    size_t delim_len = delim.length();
    if (delim_len == 0)
        return elems;
    while (pos < len)
    {
        int find_pos = s.find(delim, pos);
        if (find_pos < 0)
        {
            tmp = s.substr(pos, len - pos);
            elems.push_back(trim(tmp));
            break;
        }
        tmp = s.substr(pos, find_pos - pos);
        elems.push_back(trim(tmp));
        pos = find_pos + delim_len;
    }
    return elems;
}

/////////////////////////

/*
本函数功能：
用于测试组装命令的场景。
使用如下：
./a_all.out "print abc; print 100 200;"
即只有一个参数。为了复用已有命令终端，将参数分析还原为argc argv形式，再调用
*/

int readline_cmd_allone(int argc, char ** argv)
{
    if (argc < 2) return -1;

// 命令参数最大为10个
#define MAX_ARGC 10
    std::vector<std::string> v = splitString(argv[1], ";");
	cmd_tbl_t *ptable = NULL;
    char* myargv[MAX_ARGC] = {NULL};
    int myargc = 0;
	for(unsigned int i=0; i<v.size(); i++)
	{
        // printf("split: [%s] %d\n", v[i].c_str(), v[i].empty());
		if(v[i].empty())
		{
			continue;
		}
        char cmd[128];
		memcpy(cmd, v[i].c_str(), 128);

		// 是否再转成argv的形式？？
		
        std::vector<std::string> vv = splitString(cmd, " ");
        myargc = (int)vv.size();
        myargc = myargc > MAX_ARGC ? MAX_ARGC : myargc;
        for (int j = 0; j < myargc; j++)
        {
            myargv[j] = (char*)vv[j].c_str();
        }

        // for (int j = 0; j < myargc; j++)
        // {
        //     printf("myargv[%d]: %s\n", j, myargv[j]);
        // }

        ptable = find_table(myargv[0]);
		if (ptable == NULL)
		{
			printf("cmd name: [%s] not found\n", myargv[0]);
			continue;
		}
		printf("name: %s\n", ptable->name);

		ptable->cmd(myargc, myargv);
    }
        
    return 0;
}