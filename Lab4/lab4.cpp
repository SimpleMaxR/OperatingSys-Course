#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
using namespace std;
class disk
{
    // 模拟磁盘储存数据
public:
    char *saved;

    disk()
    {
        // 字符数组储存
        saved = new char[1024 * 128];
        memset(saved, '\0', 1024 * 128);
    }
};
disk Disk;

struct ipoint
{
    // I节点
    char name[8];
    int size;
    int blockPointer[8];
    int used;
};
class superblock
{
    // 超级块
public:
    ipoint ipointList[16]; // I节点的列表
    superblock()
    {
        // 初始化
        for (int i = 0; i < 128; i++)
            Disk.saved[i] = 0;
        for (int i = 0; i < 16; i++)
            ipointList[i].used = 0;
    }
};
class filesystem
{
public:
    superblock first; // 实例化超级块
    int p = 0;
    void create(char name[8], int size)
    {
        // 找到空的I节点
        for (p = 0; p < 16; p++)
            if (first.ipointList[p].used == 0)
                break;
        strcpy(first.ipointList[p].name, name); // 复制名字
        first.ipointList[p].size = size;        // 赋值大小
        first.ipointList[p].used = 1;           // 更改使用情况
        for (int i = 0; i < size; i++)
            for (int j = 0; j < 128; j++)
            {
                if (Disk.saved[j] == 0) // 如果为空，就占用
                {
                    first.ipointList[p].blockPointer[i] = j;
                    Disk.saved[j] = 1;
                    break;
                }
            }
    }
    void deleteFile(char name[8])
    {
        // 匹配文件名
        for (int i = 0; i < 16; i++)
        {
            if (strcmp(first.ipointList[p].name, name) == 0)
                break;
        }
        first.ipointList[p].used = 0; // 修改使用状态
        for (int i = 0; i < first.ipointList[p].size; i++)
        {
            int blocknum = first.ipointList[p].blockPointer[i];
            Disk.saved[blocknum] = 0;
            memset(Disk.saved + blocknum * 1024, '\0', 1024); // 清零储存
        }
    }
    void write(char name[8], int blockNum, char buf[1024])
    {
        // 匹配文件名
        for (int i = 0; i < 16; i++)
            if (strcmp(first.ipointList[p].name, name) == 0)
                break;
        int blocknum = first.ipointList[p].blockPointer[blockNum - 1];
        Disk.saved[blocknum] = 1;
        strncpy(Disk.saved + blocknum * 1024, buf, 1024); // 复制内容到指定块
    }
    void read(char name[8], int blockNum, char buf[1024])
    {
        // 匹配文件名
        for (int i = 0; i < 16; i++)
            if (strcmp(first.ipointList[p].name, name) == 0)
                break;
        int blocknum = first.ipointList[p].blockPointer[blockNum - 1];
        strncpy(buf, Disk.saved + blocknum * 1024, 1024); // 复制内容到指定数组
    }
};
int main()
{
    filesystem S;
    char a[8];
    char c[1024], d[1024];
    int input, size, blockNum;
    while (1)
    {
        cout << "\n1.新建文件\n2.删除文件\n3.写入文件块\n4.读取文件块\n";
        cin >> input;
        switch (input)
        {
        case 1:
            cout << "输入文件名及大小\n";
            cin >> a;
            cin >> size;
            S.create(a, size);
            break;
        case 2:
            cout << "输入文件名\n";
            cin >> a;
            S.deleteFile(a);
            break;
        case 3:
            cout << "输入文件名，文件块号，以及写入的字符\n";
            cin >> a;
            cin >> blockNum;
            cin >> c;
            S.write(a, blockNum, c);
            break;
        case 4:
            cout << "输入文件名，文件块号\n";
            cin >> a;
            cin >> blockNum;
            S.read(a, blockNum, d);
            cout << endl
                 << d << endl;
            break;
        default:
            break;
        }
    }
}