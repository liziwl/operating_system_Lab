#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <limits>
#include <queue>

/*
--实现功能--
man:显示功能目录man目录显示参数命令后加p为提供路径选项。
pwd:显示当前工作目录的路径
ls:显示当前工作目录下的所有子目录与文件
lsppath: 显示路径为path目录下的所有子目录与文件
mkdir dir_name: 在当前工作目录创建一个名称为dir_name的子目录。
mkdirp path dir_name:在路径为path目录下创建一个名称为dir_name的子目录
cd dir_name: 进入当前工作目录名称为 dir_name 的子目录
cdp path: 进入路径为 path 的子目录
dedir dir_name: 在当前工作目录删除一个名称为 dir_name 的子目录。
dedirp path dir_name: 在路径为 path 目录下删除一个名称为 dir_name 的子目录
create file_name omode: 在当前工作目录创建一个名称为 file_name 的文件, 并指定文件的权限 omode。
(omode 以 0~7 的数字表示，1 执行权限，2 写权限，4 读权限 )
createp path file_name: 在路径为 path 目录下创建一个名称为 file_name 的文件，并指定文件的权限 omode。

search file_name: 查找所有文件名为 file_name 的文件，并输出文件所在路径
open file_name: 在当前工作目录打开一个名称为 file_name 的文件。
openp path file_name: 在路径为 path 目录下打开一个名称为 file_name 的文件
lsfile:显示所有打开的文件。并逐行输出以打开文件的
”file_path file_name file_creater file_opener file_omode”
只有打开的文件，且权限对应才可以进行相应的读写操作

write file_name buff wmode: 在当前工作目录名称为 file_name 的文件按照 wmode 写入 buff
write path file_name buff wmode: 在路径为 path 目录下名称为 file_name 的文件按照 wmode 写入 buff
wmode: 文件写入的方式，0 从末尾写入。1 从文件指针处开始替换。2 从文件指针处开始插入。写完后文件指针指向文件末尾。

reposition file_name pos: 设置当前工作目录 file_name 文件的文件指针为 pos
repositionp path file_name pos: 设置路径为 path 目录下 file_name 文件的文件指针为 pos
cat file_name: 显示当前工作目录名称为 file_name 文件的内容
catp path file_name: 显示路径为 path 目录下的 file_name 文件
truncate file_name cnt: 保留当前工作目录名称为 file_name 文件的前 cnt 个字符
truncatep path file_name cnt: 保留路径为 path 目录的名称为 file_name 文件的前cnt 个字符
chmod file_name: 改变当前目录下 file_name 文件的权限。
chmod path file_name: 改变路径为 path 目录下的 file_name 的文件权限
close file_name: 关闭当前目录下 file_name 文件。
closep path file_name: 关闭路径为 path 目录下的 file_name 的文件
delete file_name: 删除当前工作目录下的 file_name 文件
deletep path file_name:删除路径为 path 目录下的 file_name 文件
regusr username password: 创建用户
delusr username: 删除用户
disusr: 显示所有用户 总有一个 root 用户，密码为 111
login username: 登陆为 username 用户

--测试方式--
我们会预置一系列指令。对于合法指令，应正确执行得到相应的结果。
对于无法正确执行的指令，应给出对应提示：
目录已经存在：directory already exists
目录不存在：directory does not exist
文件已经存在：file already exists
文件不存在：file does not exist
文件写入成功: write ok
文件重定位成功: reposition ok
文件重定位失败(pos 位置不合理): reposition failed
文件已经打开: file already open
文件未打开: file does not open
文件操作失败(权限不对应): permission denied
用户注册成功: register ok
用户已经存在: user already exists
用户不存在: user does not exist
用户登陆成功: login ok
密码错误: password wrong
查看/删除用户权限不够: permission denied
没有找到文件: file does not found
具体的输出请参考 demo 截图。部分错误存在逻辑关系，如未打开文件进行文件操作，
此时应仅返回文件未打开错误。
*/

#define INIT_PATH "/"
#define INIT_USR "root"
#define READ 0x4
#define WRITE 0x2
#define EXC 0x1

const std::string SPACE = " ";
const std::string SLASH = "/";

using namespace std;

typedef struct user
{
    string name;
    string password;
} User;

typedef struct file
{
    string name;
    User *creater;
    User *opener;
    int mode;    // RWX 4,2,1
    char opened; // 0 close, 1 open
    int pos;     // pointer pos
    int size;
    string content;
    struct dir *location;
} File;

typedef struct dir
{
    string name;
    struct dir *parent;
    vector<struct file *> *f;
    vector<struct dir *> *d;
} Dir;
Dir *root_dir;
Dir *cur_dir;
User *root_usr;
User *cur_usr;
vector<User *> users_list;
vector<File *> opened_files;

string &trim(string &s);

vector<string> split(string &s, const string &delim);

void _man();

// DIR cmds
string _pwd(Dir *path);
void _cd(string dir_name);
Dir *_cdp(string path);
void _mkdir(string dir_name);
void _mkdirp(string path, string dir_name);
void _dedir(string dir_name);
void _dedirp(string path, string dir_name);

// FILE cmds
void _ls();
void _lsp(string path);
void print_head(string usr, string path);
string read_cmd();
Dir *prase_path(string path);
Dir *find_dir(string path, vector<Dir *> *d);
Dir *set_path(Dir *path);
int permission_check(File *f, int mode);
void _create(string file_name, int omode);
void _createp(string path, string file_name, int omode);
void _chmod(string file_name, int omode);
void _chmodp(string path, string file_name, int omode);
void _open(string file_name);
void _openp(string path, string file_name);
void _close(string file_name);
void _closep(string path, string file_name);
void _delete(string file_name);
void _deletep(string path, string file_name);
void _cat(string file_name);
void _catp(string path, string file_name);
void _write(string file_name, string buff, int wmode);
void _writep(string path, string file_name, string buff, int wmode);
void _reposition(string file_name, int pos);
void _repositionp(string path, string file_name, int pos);
void _truncate(string file_name, int cnt);
void _truncatep(string path, string file_name, int cnt);
void _lsfile();
void _search(string file_name);

// USER cmds
void _reguser(string username, string password);
void _deluser(string username);
void _login(string username);
void _disuser();

void init();

string &trim(string &s)
{
    if (s.empty())
    {
        return s;
    }
    s.erase(0, s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

vector<string> split(string &s, const string &delim)
{
    size_t last = 0;
    size_t index = s.find_first_of(delim, last);
    vector<string> ret;
    while (index != string::npos)
    {
        ret.push_back(s.substr(last, index - last));
        last = index + 1;
        index = s.find_first_of(delim, last);
    }
    if (index - last > 0)
    {
        ret.push_back(s.substr(last, index - last));
    }
    return ret;
}

void _man()
{
    cout << "Manual:\n"
         << "0) pwd (current work directory)\n"
         << "1) mkdir(p) (path) directory_name\n"
         << "2) dedir(p) (path) directory_name\n"
         << "3) search file_name\n"
         << "4) create(p) (path) file_name omode\n"
         << "5) cat(p) (path) file_name\n"
         << "6) delete(p) (path) file_name\n"
         << "7) write(p) (path) file_name buff wmode\n"
         << "8) reposition(p) (path) file_name pos\n"
         << "9) truncate(p) (path) file_name cnt\n"
         << "10) ls(p) (path)\n"
         << "11) open(p) (path) file_name\n"
         << "12) close(p) (path) file_name\n"
         << "13) regusr user_name password\n"
         << "14) delusr user_name [root only]\n"
         << "15) disusr\n"
         << "16) login user_name\n"
         << "17) lsfile\n"
         << "18) chmod(p) (path) file_name mode\n"
         << "19) cd(p) (path) dir_name\n"
         << "20) exit" << endl;
}

string _pwd(Dir *path)
{
    string out;
    if (path->name != "/")
    {
        out = path->name + "/";
    }
    else
    {
        out = path->name;
    }
    while (NULL != path->parent)
    {
        path = path->parent;
        if (path->name != "/")
        {
            out = path->name + "/" + out;
        }
        else
        {
            out = path->name + out;
        }
    }
    return out;
}

// 单层 change dir
void _cd(string dir_name)
{
    string name = _pwd(cur_dir) + dir_name + "/";
    _cdp(name);
}

// 多层解析 change dir
Dir *_cdp(string path)
{
    Dir *tmp = prase_path(path);
    if (tmp != NULL)
        return set_path(tmp);
    else
    {
        cout << "directory does not exist" << endl;
        return NULL;
    }
}

void _mkdir(string dir_name)
{
    string name = _pwd(cur_dir) + dir_name + "/";
    Dir *tmp = prase_path(name);
    if (tmp == NULL)
    {
        Dir *tmp = new Dir;
        tmp->f = new vector<File *>;
        tmp->d = new vector<Dir *>;
        tmp->name = dir_name;
        tmp->parent = cur_dir;
        cur_dir->d->push_back(tmp);
    }
    else
    {
        cout << "directory already exists" << endl;
    }
}

void _mkdirp(string path, string dir_name)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _mkdir(dir_name);
        set_path(old_dir);
    }
}

void _dedir(string dir_name)
{
    string name = _pwd(cur_dir) + dir_name + "/";
    Dir *tmp = prase_path(name);
    if (tmp != NULL)
    {
        for (auto iter = tmp->parent->d->begin(); iter != tmp->parent->d->end(); iter++)
        {
            if ((*iter) == tmp)
            {
                iter = tmp->parent->d->erase(iter);
                break;
            }
        }
        delete tmp;
    }
    else
    {
        cout << "directory does not exist" << endl;
    }
}

void _dedirp(string path, string dir_name)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _dedir(dir_name);
        set_path(old_dir);
    }
}

void _ls()
{
    for (auto iter = cur_dir->d->begin(); iter != cur_dir->d->end(); iter++)
    {
        cout << (*iter)->name << endl;
    }
    for (auto iter = cur_dir->f->begin(); iter != cur_dir->f->end(); iter++)
    {
        cout << (*iter)->name << endl;
    }
}

void _lsp(string path)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _ls();
        set_path(old_dir);
    }
}

void _create(string file_name, int omode)
{
    bool exist = false;
    for (auto iter = cur_dir->f->begin(); iter != cur_dir->f->end(); iter++)
    {
        if ((*iter)->name == file_name)
        {
            exist = true;
            break;
        }
    }
    if (!exist)
    {
        File *tmp = new File;
        tmp->name = file_name;
        tmp->content = "";
        tmp->pos = 0;
        tmp->mode = omode;
        tmp->opened = 0;
        tmp->size = 0;
        tmp->creater = cur_usr;
        tmp->opener = NULL;
        tmp->location = cur_dir;
        cur_dir->f->push_back(tmp);
    }
    else
    {
        cout << "file already exists" << endl;
    }
}

void _createp(string path, string file_name, int omode)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _create(file_name, omode);
        set_path(old_dir);
    }
}

void _chmod(string file_name, int omode)
{
    File *tmp = NULL;
    for (auto iter = cur_dir->f->begin(); iter != cur_dir->f->end(); iter++)
    {
        if ((*iter)->name == file_name)
        {
            tmp = (*iter);
            break;
        }
    }
    if (tmp != NULL)
    {
        tmp->mode = omode;
    }
    else
    {
        cout << "file does not exist" << endl;
    }
}

void _chmodp(string path, string file_name, int omode)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _chmod(file_name, omode);
        set_path(old_dir);
    }
}

void _open(string file_name)
{
    File *tmp = NULL;
    for (auto iter = cur_dir->f->begin(); iter != cur_dir->f->end(); iter++)
    {
        if ((*iter)->name == file_name)
        {
            tmp = (*iter);
            break;
        }
    }
    if (tmp != NULL)
    {
        if (tmp->opened == 1)
            cout << "file already open" << endl;
        else
        {
            tmp->opened = 1;
            opened_files.push_back(tmp);
            tmp->opener = cur_usr;
        }
    }
    else
    {
        cout << "file does not exist" << endl;
    }
}

void _openp(string path, string file_name)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _open(file_name);
        set_path(old_dir);
    }
}

void _close(string file_name)
{
    File *tmp = NULL;
    for (auto iter = cur_dir->f->begin(); iter != cur_dir->f->end(); iter++)
    {
        if ((*iter)->name == file_name)
        {
            tmp = (*iter);
            break;
        }
    }
    if (tmp != NULL)
    {
        if (tmp->opened == 0)
            cout << "file does not open" << endl;
        else
        {
            tmp->opened = 0;
            tmp->opener = NULL;
            for (auto iter = opened_files.begin(); iter != opened_files.end(); iter++)
            {
                if ((*iter) == tmp)
                {
                    iter = opened_files.erase(iter);
                    break;
                }
            }
        }
    }
    else
    {
        cout << "file does not exist" << endl;
    }
}

void _closep(string path, string file_name)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _close(file_name);
        set_path(old_dir);
    }
}

void _delete(string file_name)
{
    // 当作强制删除
    File *tmp = NULL;
    for (auto iter = cur_dir->f->begin(); iter != cur_dir->f->end(); iter++)
    {
        if ((*iter)->name == file_name)
        {
            iter = cur_dir->f->erase(iter);
            tmp = (*iter);
            break;
        }
    }
    if (tmp != NULL)
    {
        if (tmp->opened == 1)
        {
            for (auto iter = opened_files.begin(); iter != opened_files.end(); iter++)
            {
                if ((*iter) == tmp)
                {
                    iter = opened_files.erase(iter);
                    break;
                }
            }
        }
        delete tmp;
    }
    else
    {
        cout << "file does not exist" << endl;
    }
}

void _deletep(string path, string file_name)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _delete(file_name);
        set_path(old_dir);
    }
}

void _cat(string file_name)
{
    File *tmp;
    for (auto iter = cur_dir->f->begin(); iter != cur_dir->f->end(); iter++)
    {
        if ((*iter)->name == file_name)
        {
            tmp = (*iter);
            break;
        }
    }
    if (tmp != NULL)
    {
        if (tmp->opened != 1)
            cout << "file does not open" << endl;
        else if (permission_check(tmp, READ) == 0)
            cout << "permission denied" << endl;
        else
            cout << tmp->content << endl;
    }
    else
    {
        cout << "file does not exist" << endl;
    }
}

void _catp(string path, string file_name)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _cat(file_name);
        set_path(old_dir);
    }
}

void _write(string file_name, string buff, int wmode)
{
    File *tmp = NULL;
    for (auto iter = cur_dir->f->begin(); iter != cur_dir->f->end(); iter++)
    {
        if ((*iter)->name == file_name)
        {
            tmp = (*iter);
            break;
        }
    }
    if (tmp != NULL)
    {
        if (tmp->opened != 1)
        {
            cout << "file does not open" << endl;
        }
        else if (permission_check(tmp, WRITE) == 0)
        {
            cout << "permission denied" << endl;
        }
        else
        {
            switch (wmode)
            {
            case 0:
                tmp->content = tmp->content + buff;
                tmp->size = tmp->content.size();
                break;
            case 1:
                if (buff.size() + tmp->pos < tmp->size)
                    tmp->content = tmp->content.substr(0, tmp->pos) + buff + tmp->content.substr(buff.size() + tmp->pos);
                else
                    tmp->content = tmp->content.substr(0, tmp->pos) + buff;
                tmp->size = tmp->content.size();
                break;
            case 2:
                tmp->content = tmp->content.substr(0, tmp->pos) + buff + tmp->content.substr(tmp->pos);
                tmp->size = tmp->content.size();
                break;
            }
            tmp->pos = tmp->size;
            cout << "write ok" << endl;
        }
    }
    else
    {
        cout << "file does not exist" << endl;
    }
}

void _writep(string path, string file_name, string buff, int wmode)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _write(file_name, buff, wmode);
        set_path(old_dir);
    }
}

void _reposition(string file_name, int pos)
{
    File *tmp = NULL;
    for (auto iter = cur_dir->f->begin(); iter != cur_dir->f->end(); iter++)
    {
        if ((*iter)->name == file_name)
        {
            tmp = (*iter);
            break;
        }
    }
    if (tmp != NULL)
    {
        if (tmp->size >= pos)
        {
            tmp->pos = pos;
            cout << "reposition ok" << endl;
        }
        else
        {
            cout << "reposition failed" << endl;
        }
    }
    else
    {
        cout << "file does not exist" << endl;
    }
}

void _repositionp(string path, string file_name, int pos)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _reposition(file_name, pos);
        set_path(old_dir);
    }
}

void _truncate(string file_name, int cnt)
{
    File *tmp = NULL;
    for (auto iter = cur_dir->f->begin(); iter != cur_dir->f->end(); iter++)
    {
        if ((*iter)->name == file_name)
        {
            tmp = (*iter);
            break;
        }
    }
    if (tmp != NULL)
    {
        tmp->content = tmp->content.substr(0, cnt);
        tmp->size = tmp->content.size();
        tmp->pos = tmp->size;
    }
    else
    {
        cout << "file does not exist" << endl;
    }
}

void _truncatep(string path, string file_name, int cnt)
{
    Dir *old_dir = _cdp(path);
    if (old_dir != NULL)
    {
        _truncate(file_name, cnt);
        set_path(old_dir);
    }
}

void _lsfile()
{
    for (auto iter = opened_files.begin(); iter != opened_files.end(); iter++)
    {
        cout << _pwd((*iter)->location) << SPACE
             << (*iter)->name << SPACE
             << (*iter)->creater->name << SPACE
             << (*iter)->opener->name << SPACE
             << (*iter)->mode << endl;
    }
}

void _search(string file_name)
{
    Dir *tmp_dir;
    vector<File *> all_file;
    queue<Dir *> sub_dir;
    sub_dir.push(root_dir);
    while (!sub_dir.empty())
    {
        tmp_dir = sub_dir.front();
        for (auto iter = tmp_dir->f->begin(); iter != tmp_dir->f->end(); iter++)
        {
            all_file.push_back(*iter);
        }
        for (auto iter = tmp_dir->d->begin(); iter != tmp_dir->d->end(); iter++)
        {
            sub_dir.push(*iter);
        }
        sub_dir.pop();
    }
    for (auto iter = all_file.begin(); iter != all_file.end(); iter++)
    {
        if ((*iter)->name == file_name)
        {
            cout << _pwd((*iter)->location) << file_name << endl;
        }
    }
}

void _reguser(string username, string password)
{
    bool exist = false;
    for (auto iter = users_list.begin(); iter != users_list.end(); iter++)
    {
        if ((*iter)->name == username)
        {
            exist = true;
            break;
        }
    }
    if (!exist)
    {
        User *tmp = new User;
        tmp->name = username;
        tmp->password = password;
        users_list.push_back(tmp);
        cout << "register ok" << endl;
    }
    else
    {
        cout << "user already exists" << endl;
    }
}

void _deluser(string username)
{
    if (cur_usr->name != "root")
    {
        cout << "permission denied" << endl;
    }
    else
    {
        bool exist = false;
        User *tmp = NULL;
        for (auto iter = users_list.begin(); iter != users_list.end(); iter++)
        {
            if ((*iter)->name == username)
            {
                exist = true;
                tmp = (*iter);
                iter = users_list.erase(iter);
                break;
            }
        }
        if (!exist)
            cout << "user does not exist" << endl;
        else
            delete tmp;
    }
}

void _login(string username)
{
    User *tmp = NULL;
    for (auto iter = users_list.begin(); iter != users_list.end(); iter++)
    {
        if ((*iter)->name == username)
        {
            tmp = (*iter);
            break;
        }
    }
    if (tmp != NULL)
    {
        cout << "password:";
        string paaswd;
        cin >> paaswd;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (paaswd == tmp->password)
        {
            cout << "login ok" << endl;
            cur_usr = tmp;
        }
        else
            cout << "password wrong" << endl;
    }
    else
        cout << "user does not exist" << endl;
}

void _disuser()
{
    if (cur_usr->name != "root")
    {
        cout << "permission denied" << endl;
    }
    else
    {
        for (auto iter = users_list.begin(); iter != users_list.end(); iter++)
        {
            cout << (*iter)->name << endl;
        }
    }
}

void print_head(string usr, string path)
{
    cout << usr << "$" << path << "%:";
}

string read_cmd()
{
    string response;
    getline(cin, response);
    return response;
}

Dir *prase_path(string path)
{
    vector<string> pt = split(path, SLASH);
    int count = pt.size();
    Dir *rt = root_dir;
    for (int i = 1; i < count - 1; i++)
    {
        Dir *sub = find_dir(pt[i], rt->d);
        if (NULL != sub)
        {
            rt = sub;
        }
        else
        {
            return NULL;
        }
    }
    return rt;
}

Dir *find_dir(string path, vector<Dir *> *d)
{
    Dir *out = NULL;
    int count = (*d).size();
    for (int i = 0; i < count; i++)
    {
        if ((*d)[i]->name == path)
        {
            out = (*d)[i];
        }
    }
    return out;
}

Dir *set_path(Dir *path)
{
    Dir *old_dir = cur_dir;
    cur_dir = path;
    return old_dir;
}

int permission_check(File *f, int mode)
{
    return f->mode && mode;
}

void init()
{
    root_dir = new dir;
    root_dir->name = INIT_PATH;
    root_dir->parent = NULL;
    root_dir->f = new vector<File *>;
    root_dir->d = new vector<Dir *>;

    cur_dir = root_dir;

    root_usr = new User;
    root_usr->name = "root";
    root_usr->password = "111";

    cur_usr = root_usr;

    users_list.push_back(root_usr);
}

int cmd_switch(vector<string> ret)
{
    string cmd = ret[0];
    if (cmd == "man")
        _man();
    else if (cmd == "pwd")
        cout << _pwd(cur_dir) << endl;
    else if (cmd == "mkdir")
        _mkdir(ret[1]);
    else if (cmd == "mkdirp")
        _mkdirp(ret[1], ret[2]);
    else if (cmd == "dedir")
        _dedir(ret[1]);
    else if (cmd == "dedirp")
        _dedirp(ret[1], ret[2]);
    else if (cmd == "search")
        _search(ret[1]);
    else if (cmd == "create")
        _create(ret[1], stoi(ret[2]));
    else if (cmd == "createp")
        _createp(ret[1], ret[2], stoi(ret[3]));
    else if (cmd == "cat")
        _cat(ret[1]);
    else if (cmd == "catp")
        _catp(ret[1], ret[2]);
    else if (cmd == "delete")
        _delete(ret[1]);
    else if (cmd == "deletep")
        _deletep(ret[1], ret[2]);
    else if (cmd == "write")
        _write(ret[1], ret[2], stoi(ret[3]));
    else if (cmd == "writep")
        _writep(ret[1], ret[2], ret[3], stoi(ret[4]));
    else if (cmd == "reposition")
        _reposition(ret[1], stoi(ret[2]));
    else if (cmd == "repositionp")
        _repositionp(ret[1], ret[2], stoi(ret[3]));
    else if (cmd == "truncate")
        _truncate(ret[1], stoi(ret[2]));
    else if (cmd == "truncatep")
        _truncatep(ret[1], ret[2], stoi(ret[3]));
    else if (cmd == "ls")
        _ls();
    else if (cmd == "lsp")
        _lsp(ret[1]);
    else if (cmd == "open")
        _open(ret[1]);
    else if (cmd == "openp")
        _openp(ret[1], ret[2]);
    else if (cmd == "close")
        _close(ret[1]);
    else if (cmd == "closep")
        _closep(ret[1], ret[2]);
    else if (cmd == "regusr")
        _reguser(ret[1], ret[2]);
    else if (cmd == "delusr")
        _deluser(ret[1]);
    else if (cmd == "disusr")
        _disuser();
    else if (cmd == "login")
        _login(ret[1]);
    else if (cmd == "lsfile")
        _lsfile();
    else if (cmd == "chmod")
        _chmod(ret[1], stoi(ret[2]));
    else if (cmd == "chmodp")
        _chmodp(ret[1], ret[2], stoi(ret[3]));
    else if (cmd == "cd")
        _cd(ret[1]);
    else if (cmd == "cdp")
        _cdp(ret[1]);
    else if (cmd == "exit")
        exit(0);
}

int main(int argc, char const *argv[])
{
    init();

    while (1)
    {
        print_head(cur_usr->name, _pwd(cur_dir));
        string act = read_cmd();
        act = trim(act);
        vector<string> ret = split(act, SPACE);
        cmd_switch(ret);
    }
    return 0;
}
