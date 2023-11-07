#include <iostream>
#include <vector>
using namespace std;

struct Process // 进程
{
    string name;
    int size;
};

struct Partition // 分配的区域
{
    string name;
    int begin;
    int end;
    char status; // ’u‘ 表示 use，f 表示 free
};

vector<Partition> storage_list; // 使用 vector 表示内存中的情况

void init()
{
    // 添加 OS 内存占用和剩余空间
    Partition os, f;
    os.name = "OS";
    os.begin = 0;
    os.end = 39;
    os.status = 'u';
    storage_list.push_back(os);

    f.name = "Free Space";
    f.begin = 40;
    f.end = 639;
    f.status = 'f';
    storage_list.push_back(f);
}

void show()
{
    for (const auto &p : storage_list)
    {
        cout << p.name << ": " << p.begin << "-" << p.end << endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

void combineFreespace() {
    // 从头遍历 storage_list, 合并空闲分区
    bool flag = false; // 上一个分区是否是空闲分区
    for (int i = 0; i < storage_list.size(); i++) {
        if (storage_list[i].name == "Free Space" && !flag) {
            flag = true;
            continue;
        }
        if (flag) {
            if (storage_list[i].name == "Free Space") {
                storage_list[i - 1].end = storage_list[i].end;
                storage_list.erase(storage_list.begin() + i);
                i--;
            }
            continue;
        }
        flag = false;
    }
}

void allocateMemory(const string &name, int size)
{
    // 遍历 storage_list, 找到第一个空闲分区，分配内存（first fit）
    for (int i = 0; i < storage_list.size(); i++) {
        if (storage_list[i].status == 'f' && storage_list[i].end - storage_list[i].begin + 1 >= size) {
            Partition allocated;
            allocated.name = name;
            allocated.begin = storage_list[i].begin;
            allocated.end = storage_list[i].begin + size - 1;
            allocated.status = 'u';
            

            Partition remaining;
            remaining.name = "Free Space";
            remaining.begin = allocated.end + 1;
            remaining.end = storage_list[i].end;
            remaining.status = 'f';

            storage_list[i] = allocated;
            storage_list.insert(storage_list.begin() + i + 1, remaining);

            show();
            return;
        }
    }

    cout << "Not enough free memory to allocate " << name << endl;
}

void freeMemory(const string& name) {
    // 遍历 storage_list, 找到对应的进程，释放内存，然后调用 combineFreespace() 合并空闲分区
    for (int i =0; i<storage_list.size(); i++){
        if (storage_list[i].name == name) {
            Partition freed;
            freed.name = "Free Space";
            freed.begin = storage_list[i].begin;
            freed.end = storage_list[i].end;
            freed.status = 'f';

            storage_list[i] = freed;
            combineFreespace();
            show();
            return;
        }
    }

    cout << "No such process " << name << endl;
}


int main() {
    init(); // 初始化内存

    // 测试：
    // 分配 A 80，分配 B 60，分配 C 100，释放 B，分配 D 50，释放 C，释放 A，释放 D 
    cout << "allocate A 80" << endl;
    allocateMemory("A", 80);

    cout << "allocate B 60" << endl;
    allocateMemory("B", 60);

    cout << "allocate C 100" << endl;
    allocateMemory("C", 100);

    cout << "free B" << endl;
    freeMemory("B");

    cout << "allocate D 50" << endl;
    allocateMemory("D", 50);

    cout << "free C" << endl;
    freeMemory("C");

    cout << "free A" << endl;
    freeMemory("A");

    cout << "free D" << endl;
    freeMemory("D");

    return 0;
}
