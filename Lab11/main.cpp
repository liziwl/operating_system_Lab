#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

/*
--实现功能--
man:显示功能目录man目录显示参数命令后加p为提供路径选项。
pwd:显示当前工作目录的路径➢ls:显示当前工作目录下的所有子目录与文件➢
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