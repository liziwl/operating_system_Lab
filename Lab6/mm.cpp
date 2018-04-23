#include<bits/stdc++.h>
#include<unistd.h>

using namespace std;

#define PROCESS_NAME_LEN 32 //进程名最大长度
#define MIN_SLICE 10 //内碎片最大大小
#define DEFAULT_MEM_SIZE 1024  //总内存大小
#define DEFAULT_MEM_START 0  //内存开始分配时的起始地址
/* 内存分配算法 */
#define MA_FF 1
#define MA_BF 2
#define MA_WF 3

typedef pair<int, string> My_algo;

int mem_size = DEFAULT_MEM_SIZE;
bool flag = 0; //当内存已经被分配了之后，不允许更改总内存大小的flag
static int pid = 0;
// My_algo algo[123];
// algo[0] = make_pair(1,"233");
int algo_type = MA_FF;

struct free_block{	//空闲数据块
	int size;
	int start_addr;
	free_block *next;
};

struct allocated_block{ //已分配的数据块
	int pid;
	int size;
	int start_addr;
	char process_name[PROCESS_NAME_LEN];
	int *data;
	allocated_block *next;
};

free_block *free_block_head; //空闲数据块首指针
allocated_block *allocated_block_head = NULL; //分配块首指针

allocated_block *find_process(int id); //寻找pid为id的分配块
free_block *init_free_block(int mem_size); //空闲块初始化
void display_menu(); //显示选项菜单
void set_mem_size(); //设置内存大小
int allocate_mem(allocated_block *ab); //为制定块分配内存
int create_new_process(); //创建新的进程
void swap(int *p, int *q); //交换地址
void rearrange_sp(bool (*func)(free_block, free_block)); // 对块进行重新分配
void rearrange();
int free_mem(allocated_block *ab); //释放分配块
int dispose(allocated_block *ab); //释放分配块结构体
void display_mem_usage(); //显示内存情况
void kill_process(); //杀死对应进程并释放其空间与结构体
void set_algo();
void Usemy_algo(int id); //使用对应的分配算法
bool is_small_addr(free_block *p, free_block *q);
bool is_small_size(free_block *p, free_block *q);
bool is_large_size(free_block *p, free_block *q);

//主函数
int main(){
	int op = 0;
	pid = 0;
	free_block_head = init_free_block(mem_size); //初始化一个可以使用的内存块，类似与操作系统可用的总存储空间
	for(;;){
		sleep(1);
		display_menu();
		op = 0;		
		// cerr << "--" << op << endl;
		fflush(stdin);
		scanf("%d", &op);
		getchar();
		// cerr << "-+" << op << endl;
		switch (op){
			case 1:{ set_mem_size(); break; }
			case 2:{ set_algo(); break; }
			case 3:{ create_new_process(); break; }
			case 4:{ kill_process(); break; }
			case 5:{ display_mem_usage(); break; }
			case 233:{ puts("bye...."); sleep(1); return 0; }
			defaut: break;
		}
	}
}

// TODO
allocated_block *find_process(int id){ //循环遍历分配块链表，寻找pid=id的进程所对应的块
	allocated_block *abb = NULL;
	if (allocated_block_head)
	{
		for (abb = allocated_block_head->next; abb && abb->pid != pid; abb = abb->next)
			;
	}
	return abb;
}

free_block *init_free_block(int mem_size){ //初始化空闲块，这里的mem_size表示允许的最大虚拟内存大小
	free_block *p;
	p = (free_block *)malloc(sizeof(free_block));
	if (p == NULL){
		puts("No memory left");
		return NULL;
	}
	p->size = mem_size;
	p->start_addr = DEFAULT_MEM_START;
	p->next = NULL;
	return p;
}

void display_menu(){
	puts("\n\n******************menu*******************");
	printf("1) Set memory size (default = %d)\n", DEFAULT_MEM_SIZE);
	printf("2) Set memory allocation algorithm\n");
	printf("3) Create a new process\n");
	printf("4) Kill a process\n");
	printf("5) Display memory usage\n");
	printf("233) Exit\n");
}

// TODO
void set_mem_size(){ //更改最大内存大小
	int size = 0;
	if (flag != false)
	{ //防止重复设置
		printf("Cannot set memory size again.\n");
	}
	else{
		printf("Total memory size =");
		scanf("%d", &size);
		getchar();
		if (size > 0)
		{
			mem_size = size;
			free_block_head->size = mem_size;
			flag = true;
		}
		else{
			printf("Invalid size.\n");
		}
	}
}

// TODO
int allocate_mem(allocated_block *ab){ //为块分配内存，真正的操作系统会在这里进行置换等操作
	free_block *p = NULL;
	free_block *q = NULL;
	free_block *temp = NULL;
	int sum = 0;
	if (free_block_head == NULL)
	{
		printf("no  memory");
		return -1;
	}
	rearrange();
	if (algo_type == MA_FF || algo_type == MA_BF)
	{
		p = free_block_head;
		while (p != NULL)
		{
			if (p->size >= ab->size)
			{
				q = p;
				break;
			}
			p = p->next;
		}
		if (q)
		{
			if ((q->size - ab->size) <= MIN_SLICE)
			{
				ab->start_addr = q->start_addr;
				ab->size = q->size;
				p->next = q->next;
				free(q);
			}
			else
			{
				ab->start_addr = q->start_addr;
				q->start_addr = q->start_addr + ab->size;
				q->size = q->size - ab->size;
			}
		}
		else
			return -1;
	}
	else if (algo_type == MA_WF)
	{
		q = free_block_head;
		if (q && q->size >= ab->size)
		{
			if ((q->size - ab->size) <= MIN_SLICE)
			{
				ab->start_addr = q->start_addr;
				ab->size = q->size;
				p->next = q->next;
				free(q);
			}
			else
			{
				ab->start_addr = q->start_addr;
				q->start_addr = q->start_addr + ab->size;
				q->size = q->size - ab->size;
			}
		}
		else
			return -1;
	}
	return 1;
}

// TODO
int create_new_process(){ //创建新进程
	flag = true;
	allocated_block *ab;
	allocated_block *tmp;
	int size = 0;
	int ret;
	ab = (allocated_block *)malloc(sizeof(allocated_block));
	if (!ab)
		exit(-5);
	ab->next = NULL;
	pid++;
	sprintf(ab->process_name, "PROCESS-%02d", pid);
	ab->pid = pid;
	printf("Memory for %s:", ab->process_name);
	scanf("%d", &size);
	getchar();

	if (size <= 0){
		printf("Memory size is invaild.\n");
		printf("Allocation fail\n");
		free(ab);
		return -2;
	}
	ab->size = size;
	ret = allocate_mem(ab); // 从空闲区分配内存，ret==1表示分配ok
	if ((1 == ret) && (allocated_block_head == NULL))
	{
		allocated_block_head = ab;
		return 1;
	}
	else if (1 == ret)
	{
		tmp = allocated_block_head;
		while(tmp->next){
			tmp = tmp->next;
		}
		tmp->next = ab;
		return 2;
	}
	else if (ret == -1)
	{
		// 分配不成功
		printf("Allocation fail\n");
		free(ab);
		return -1;
	}
	return 3;
}

void swap(int *p, int *q){
	int tmp = *p;
	*p = *q;
	*q = tmp;
	return;
}

/*
根据当前算法在空闲分区链表中搜索合适空闲分区进行分配，分配时注意以下情况：
1. 找到可满足空闲分区且分配后剩余空间足够大，则分割
2. 找到可满足空闲分区且但分配后剩余空间（最小碎片化）比较小，则一起分配
3. 每次对连续空闲块合并
4. 在成功分配内存后，应保持空闲分区按照相应算法有序
*/

void rearrange_sp(bool (*func)(free_block*, free_block*)){ //将块按照地址大小进行排序
	free_block *tmp, *tmpx;
	puts("Rearrange begins...");
	puts("Rearrange by address...");
	tmp = free_block_head;
	// 冒泡排序
	while(tmp != NULL){
		tmpx = tmp->next;
		while (tmpx != NULL){
			if (is_small_addr(tmpx, tmp)){
				// 根据以上条件排序
				swap(&tmp->start_addr, &tmpx->start_addr);
				swap(&tmp->size, &tmpx->size);
			}
			tmpx = tmpx->next;
		}
		tmp = tmp->next;
	}
	// 合并相邻的空闲块
	tmp = free_block_head;
	while(tmp != NULL){
		tmpx = tmp->next;
		if (tmpx){
			if (tmp->start_addr+tmp->size==tmpx->start_addr){
				tmp->size = tmp->size + tmpx->size;
				tmp->next = tmpx->next;
				free(tmpx);
			}
		}
		tmp = tmp->next;
	}

	tmp = free_block_head;
	// 冒泡排序
	while(tmp != NULL){
		tmpx = tmp->next;
		while (tmpx != NULL){
			if (func(tmpx, tmp)){
				// 根据以上条件排序
				swap(&tmp->start_addr, &tmpx->start_addr);
				swap(&tmp->size, &tmpx->size);
			}
			tmpx = tmpx->next;
		}
		tmp = tmp->next;
	}
	usleep(500);
	puts("Rearrange Done.");
}

void rearrange(){
	switch(algo_type){
		case MA_FF:
			rearrange_sp(is_small_addr);
			break;
		case MA_BF:
			rearrange_sp(is_small_size);
			break;
		case MA_WF:
			rearrange_sp(is_large_size);
			break;
	}
}

// TODO
int free_mem(allocated_block *ab){ //释放某一块的内存
	free_block *p;
	p = (free_block *)malloc(sizeof(free_block));
	p->next = free_block_head->next; //将 p 插入 free_block 头
	free_block_head->next = p;
	p->size = ab->size;
	p->start_addr = ab->start_addr;
	return 1;
}

int dispose(allocated_block *fab){ //释放结构体所占的内存
	allocated_block *pre, *ab;
	if (fab == allocated_block_head){
		allocated_block_head = allocated_block_head->next;
		free(fab);
		return 1;
	}
	pre = allocated_block_head;
	ab = allocated_block_head->next;
	while (ab != fab){ pre = ab; ab = ab->next;}
	pre->next = ab->next;
	free(ab);
	return 2;
}

void display_mem_usage(){
	free_block *fb = free_block_head;
	allocated_block *ab = allocated_block_head;
	puts("*********************Free Memory*********************");
	printf("%20s %20s\n", "start_addr", "size");
	int cnt = 0;
	while (fb != NULL){
		cnt++;
		printf("%20d %20d\n", fb->start_addr, fb->size);
		fb = fb->next;
	}
	if (!cnt) puts("No Free Memory");
	else printf("Totaly %d free blocks\n", cnt);
	puts("");
	puts("*******************Used Memory*********************");
	printf("%10s %20s %10s %20s\n", "PID", "ProcessName", "start_addr", "size");
	cnt = 0;
	while (ab != NULL){
		cnt++;
		printf("%10d %20s %10d %20d\n", ab->pid, ab->process_name, ab->start_addr, ab->size);
		ab = ab->next;
	}
	if (!cnt) puts("No allocated block");
	else printf("Totaly %d allocated blocks\n", cnt);
	return;
}

void kill_process(){ //杀死某个进程
	display_mem_usage();
	allocated_block *ab;
	int pid = -1;
	puts("Please input the pid of Killed process");
	scanf("%d", &pid);
	getchar();
	if (pid < 0)
		cout << "Invalid pid." << endl;
	else
	{
		ab = find_process(pid);
		if (ab != NULL)
		{
			free_mem(ab);
			dispose(ab);
		}
		else
		{
			cout << "pid: " << pid << " don't exist." << endl;
		}
	}
}

void set_algo(){
	cout << "\t1) First Fit" << endl;
	cout << "\t2) Best Fit" << endl;
	cout << "\t3) Worst Fit" << endl;
	int choice = -1;
	scanf("%d", &choice);
	getchar();
	if (choice < 0)
		cout << "Invalid choice." << endl;
	else
		Usemy_algo(choice);
}

void Usemy_algo(int id){
	flag = true;
	algo_type = id;
}

bool is_small_addr(free_block *p, free_block *q){
	if (p->start_addr < q->start_addr)
		return true;
	else
		return false;
}

bool is_small_size(free_block *p, free_block *q){
	if (p->size < q->size)
		return true;
	else
		return false;
}

bool is_large_size(free_block *p, free_block *q){
	if (p->size > q->size)
		return true;
	else
		return false;
}
