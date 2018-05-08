#include <bits/stdc++.h>
#include <unistd.h>

using namespace std;

const double eps = 1e-8;

// compare a float number x to eps
int dcmp(const double &x)
{
    if (fabs(x) < eps)
        return 0;
    return x < 0 ? -1 : 1;
}

#define DEFAULT_CACHE_SIZE 1024
#define DEFAULT_ALGORITHM 0
#define SLEEP_TIME 1

typedef pair<int, string> _algorithm;

int cache_size, working_algorithm, hit, miss;
vector<_algorithm> algorithms;
vector<int> pages;

struct Page_item
{
    int value;
    struct Page_item *next;
};

void init();
void print_menu();
void set_cache_size();
void print_algorithms();
void set_algorithm();
void add_an_algorithm();
void generate_input();
void generate_randomly(int n);
void generate_by_hand(int n);
void print_pages();
void run_algorithm();
void read_pages();
void FIFO_algorithm();
void LRU_algorithm();
void MIN_algorithm();
void CLOCK_algorithm();
int in_ref(const int array[], int size, int start, int page_num);
void SC_algorithm();

int main()
{
    //freopen("1.in", "r", stdin);
    init();
    //generate_input();
    //return 0;
    int op;
    while (true)
    {
        op = 0;
        print_menu();
        cin >> op;
        switch (op)
        {
        case 1:
            set_cache_size();
            break;
        case 2:
            set_algorithm();
            break;
        case 3:
            add_an_algorithm();
            break;
        case 4:
            run_algorithm();
            break;
        case 5:
            read_pages();
            break;
        case 233:
            cout << "Bye..." << endl;
            sleep(SLEEP_TIME);
            return 0;
        default:
            break;
        }
        sleep(SLEEP_TIME);
    }
}

void print_pages()
{
    cout << "pages number is: ";
    cout << pages.size() << endl;
    cout << "pages are: ";
    for (auto &x : pages)
    {
        cout << x << " ";
    }
    cout << endl;
}

void generate_randomly(int n)
{
    cout << n << endl;
    for (int i = 0; i < n; i++)
    {
        cout << rand() * 233 % cache_size << " ";
    }
    cout << endl;
}

void generate_by_hand(int n)
{
    cout << n << endl;
    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        cout << x << " ";
    }
    cout << endl;
}

void generate_input()
{
    pages.clear();
    cout << "Please input the number of pages: ";
    int n, op;
    cin >> n;
    cout << "1) Generate randomly" << endl;
    cout << "2) Generate by hand" << endl;
    cin >> op;
    freopen("1.in", "w", stdout);
    switch (op)
    {
    case 1:
        generate_randomly(n);
        break;
    case 2:
        generate_by_hand(n);
        break;
    default:
        break;
    }
}

void init()
{
    srand((unsigned int)time(0));
    algorithms.clear();
    pages.clear();
    cout << "Setting the default cache size..." << endl;
    cache_size = DEFAULT_CACHE_SIZE;
    sleep(SLEEP_TIME);
    cout << "Setting the default algorithm..." << endl;

    string s = "FIFO";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM, s));

    s = "LRU";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 1, s));

    s = "MIN";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 2, s));

    s = "CLOCK";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 3, s));

    s = "SC";
    algorithms.push_back(make_pair(DEFAULT_ALGORITHM + 4, s));

    working_algorithm = 0;
    sleep(SLEEP_TIME);
    cout << "Setting the environment..." << endl;
    sleep(SLEEP_TIME);
    cout << "Setting successfully" << endl;
}

void read_pages()
{
    pages.clear();
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
    {
        int x;
        cin >> x;
        pages.push_back(x);
    }
    cout << "Read successfully" << endl;
}

void print_menu()
{
    cout << "**********MENU**********" << endl;
    cout << "1) set cache size" << endl;
    cout << "2) set algorithm" << endl;
    cout << "3) add an algorithm" << endl;
    cout << "4) run algorithm" << endl;
    cout << "5) read pages" << endl;
    cout << "233) quit" << endl;
}

void set_cache_size()
{
    cout << "Please input the new cache size: ";
    int x;
    cin >> x;
    if (x <= 0)
    {
        cerr << "Invalid cache size" << endl;
        return;
    }
    cache_size = x;
    cout << "set cache size successfully, new cache size is: " << cache_size << endl;
}

void print_algorithms()
{
    cout << "All algorithms are: " << endl;
    for (auto &x : algorithms)
    {
        cout << x.first << " : " << x.second << endl;
    }
}

void set_algorithm()
{
    print_algorithms();
    cout << "Please input the index of algorithm" << endl;
    unsigned int id;
    cin >> id;
    if (id >= algorithms.size())
    {
        cerr << "Algorithm does not exists" << endl;
        return;
    }
    working_algorithm = id;
    cout << "Set algorithm successfully" << endl;
    cout << "Now working algorithm is: " << algorithms[id].first << ":" << algorithms[id].second << endl;
}

void add_an_algorithm()
{
    cout << "Please input the name of new algorithm: ";
    string algo;
    cin >> algo;
    algorithms.push_back(make_pair(algorithms.size(), algo));
    cout << "Algorithm added!" << endl;
    print_algorithms();
}

void FIFO_algorithm()
{
    hit = miss = 0;
    list<int> l;
    l.clear();
    for (auto &x : pages)
    {
        if (l.empty())
        {
            miss++;
            l.push_front(x);
            continue;
        }
        bool flag = false;
        for (auto &y : l)
        {
            if (y == x)
            {
                flag = true;
                break;
            }
        }
        if (flag)
        {
            hit++;
            continue;
        }
        miss++;
        if (l.size() == cache_size)
        {
            l.pop_back();
        }
        l.push_front(x);
    }
}

void LRU_algorithm()
{
    hit = miss = 0;
    list<int> l;
    l.clear();
    for (auto &x : pages)
    {
        if (l.empty())
        {
            miss++;
            l.push_front(x);
            continue;
        }
        bool flag = false;
        for (auto &y : l)
        {
            if (y == x)
            {
                flag = true;
                l.remove(y);
                l.push_front(x);
                break;
            }
        }
        if (flag)
        {
            hit++;
            continue;
        }
        miss++;
        if (l.size() == cache_size)
        {
            l.pop_back();
        }
        l.push_front(x);
    }
}

void MIN_algorithm()
{
    int i, tmp;
    hit = miss = 0;
    int addr_range = 0;
    for (i = 0; i < pages.size(); i++)
    {
        if (pages[i] > addr_range)
        {
            addr_range = pages[i];
        }
    }

    vector<vector<int>> array(addr_range + 1);
    vector<unsigned int> cur(addr_range + 1, 0);
    for (i = 0; i < array.size(); i++)
    {
        array[i].reserve(10);
    }
    for (i = 0; i < pages.size(); i++)
    {
        tmp = pages[i];
        array[tmp].push_back(i);
    }
    list<int> l;
    l.clear();
    i = -1;
    for (auto &x : pages)
    {
        i++;
        if (l.empty())
        {
            miss++;
            l.push_front(x);
            cur[x]++;
            continue;
        }
        bool flag = false;
        for (auto &y : l)
        {
            if (y == x)
            {
                flag = true;
                break;
            }
        }
        if (flag)
        {
            hit++;
            cur[x]++;
            continue;
        }
        miss++;
        if (l.size() == cache_size)
        {
            int max = -1;
            int max_index = -1;
            for (auto &y : l)
            {
                if (cur[y] >= array[y].size())
                {
                    max_index = y;
                    break;
                }
                if (array[y][cur[y]] - i > max)
                {
                    max = array[y][cur[y]] - i;
                    max_index = y;
                }
            }
            l.remove(max_index);
        }
        l.push_front(x);
        cur[x]++;
    }
}

void CLOCK_algorithm()
{
    hit = miss = 0;
    int ref[cache_size];
    int valid[cache_size];
    int pointer = 0;
    int i, j, tmp;
    int full = 0;
    // init array
    for (i = 0; i < cache_size; i++)
    {
        ref[i] = -1;
        valid[i] = 0;
    }

    for (auto &x : pages)
    {
        tmp = pointer;
        i = in_ref(ref, cache_size, tmp, x);
        if (i != -1)
        {
            // page x is already in the cache
            valid[i] = 1;
            pointer = i;
            hit++;
            pointer = (++pointer) % cache_size;
        }
        else
        {
            // page x is not in the cache
            miss++;
            if (full < cache_size)
            {
                assert(ref[full] == -1);
                ref[full] = x;
                valid[full] = 0;
                full++;
                pointer = full % cache_size;
            }
            else
            {
                for (j = 0; j < cache_size; j++)
                {
                    if (valid[tmp] == 0)
                    {
                        ref[tmp] = x;
                        tmp = (++tmp) % cache_size;
                        break;
                    }
                    else
                    {
                        valid[tmp] = 0;
                    }
                    tmp = (++tmp) % cache_size;
                }
                pointer = tmp;
            }
        }
    }
}

int in_ref(const int array[], int size, int start, int page_num)
{
    int index = -1;
    int i = start;
    int count = 0;
    while (count < size)
    {
        count++;
        if (array[i] == page_num)
        {
            index = i;
            break;
        }
        i = (++i) % size;
    }
    return index;
}

void SC_algorithm()
{
    hit = miss = 0;
    list<int> l;
    l.clear();
    int sp_index = cache_size / 2;
    int i;
    for (auto &x : pages)
    {
        if (l.empty())
        {
            miss++;
            l.push_front(x);
            continue;
        }
        bool flag = false;
        i = 0;
        for (auto &y : l)
        {
            if (y == x)
            {
                flag = true;
                break;
            }
            i++;
        }
        if (flag)
        {
            hit++;
            if (i >= sp_index)
            {
                l.remove(x);
                l.push_front(x);
            }
            continue;
        }
        miss++;
        if (l.size() == cache_size)
        {
            l.pop_back();
        }
        l.push_front(x);
    }
}

void print_status()
{
    cout << "cache size is: " << cache_size << endl;
    //print_pages();
    cout << "working algorithm is " << working_algorithm << ":" << algorithms[working_algorithm].second << endl;
    cout << "hit times = " << hit << endl
         << "miss times = " << miss << endl;
    cout << "hit percentage is " << (double)hit * 100 / (miss + hit) << "%" << endl;
}

void run_algorithm()
{
    switch (working_algorithm)
    {
    case 0:
        FIFO_algorithm();
        break;
    case 1:
        LRU_algorithm();
        break;
    case 2:
        MIN_algorithm();
        break;
    case 3:
        CLOCK_algorithm();
        break;
    case 4:
        SC_algorithm();
        break;
    default:
        break;
    }
    print_status();
}
