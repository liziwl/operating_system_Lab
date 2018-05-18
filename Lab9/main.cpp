#include <iostream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <assert.h>

// 第一行包含三个整数 S(0 ≤ S < M), M(0 < M ≤ 65536)和 N(0<N<M),
// S 表示开始磁道的位置,M 表示当前磁盘可以容纳的最大磁道数,N 表示需要访问的磁道数。
// 第二行包含 N 个整数表示需要访问的磁道的编号。
// FCFS, SSTF, SCAN, C-SCAN, LOOK, CLOOK

/*
输入样例
50 200 8
95 180 34 119 11 123 62 64
*/

using namespace std;
int s, m, n, i, tmp;
vector<int> query;

void FCFS();
void SSTF();
void SCAN();
void CSCAN();
void LOOK();
void CLOOK();

int main(int argc, char const *argv[])
{
    query.clear();
    cin >> s >> m >> n;
    // cerr << "s: " << s << endl
    //      << "m: " << m << endl
    //      << "n: " << n << endl;
    for (i = 0; i < n; i++)
    {
        cin >> tmp;
        query.push_back(tmp);
    }
    // test print query
    // cerr << "query: ";
    // for (i = 0; i < n; i++)
    // {
    //     cerr << query[i] << " ";
    // }
    // cerr << endl;
    // test print query

    FCFS();
    SSTF();
    SCAN();
    CSCAN();
    LOOK();
    CLOOK();
    return 0;
}

void FCFS()
{
    cout << "FCFS" << endl;
    int pre = s;
    cout << pre;
    long sum = 0;
    for (i = 0; i < n; i++)
    {
        sum += abs(query[i] - pre);
        pre = query[i];
        cout << " -> " << pre;
    }
    cout << endl
         << sum << endl;
}

void SSTF()
{
    cout << "SSTF" << endl;
    int pre = s;
    cout << pre;
    long sum = 0;
    vector<int> accessed(n, 0);
    vector<int> tem(query);
    sort(tem.begin(), tem.end());
    int p = -1; // left
    int q = -1; // right
    int min = INT32_MAX;
    // init p
    for (i = 0; i < n; i++)
    {
        if (abs(tem[i] - s) < min)
        {
            p = i;
            min = abs(tem[i] - s);
        }
    }
    q = p;
    // the first one
    cout << " -> " << tem[p];
    sum += abs(tem[p] - pre);
    pre = tem[p];
    accessed[p] = 1;
    p--;
    q++;
    for (i = 1; i < n; i++)
    {
        if (p >= 0 && q < n)
        {
            assert(accessed[p] == 0 && accessed[q] == 0);
            if (abs(tem[p] - pre) <= abs(tem[q] - pre))
            {
                cout << " -> " << tem[p];
                sum += abs(tem[p] - pre);
                pre = tem[p];
                accessed[p] = 1;
                p--;
            }
            else
            {
                cout << " -> " << tem[q];
                sum += abs(tem[q] - pre);
                pre = tem[q];
                accessed[q] = 1;
                q++;
            }
        }
        else
        {
            // 这里只会出现一种情况所以不用continue，直接并排写就好。
            assert(accessed[p] == 0 || accessed[q] == 0);
            if (p >= 0)
            {
                cout << " -> " << tem[p];
                sum += abs(tem[p] - pre);
                pre = tem[p];
                accessed[p] = 1;
                p--;
            }
            if (q < n)
            {
                cout << " -> " << tem[q];
                sum += abs(tem[q] - pre);
                pre = tem[q];
                accessed[q] = 1;
                q++;
            }
        }
    }
    cout << endl
         << sum << endl;
}

void SCAN()
{
    cout << "SCAN" << endl;
    int pre = s;
    cout << pre;
    long sum = 0;
    vector<int> tem(query);
    tem.push_back(s);
    sort(tem.begin(), tem.end());
    int direction = abs(s - 0) <= abs(s - (m - 1)) ? -1 : 1;
    int p = -1;
    // init p
    for (i = 0; i < n + 1; i++)
    {
        if (tem[i] == s)
        {
            p = i;
            break;
        }
    }
    int q = p;
    p--;
    q++;
    if (-1 == direction)
    {
        while (p >= 0)
        {
            cout << " -> " << tem[p];
            sum += abs(tem[p] - pre);
            pre = tem[p];
            p--;
        }
        cout << " -> " << 0;
        sum += abs(0 - pre);
        pre = 0;
        while (q < n + 1)
        {
            cout << " -> " << tem[q];
            sum += abs(tem[q] - pre);
            pre = tem[q];
            q++;
        }
    }
    else
    {
        while (q < n + 1)
        {
            cout << " -> " << tem[q];
            sum += abs(tem[q] - pre);
            pre = tem[q];
            q++;
        }
        cout << " -> " << m - 1;
        sum += abs(m - 1 - pre);
        pre = m - 1;
        while (p >= 0)
        {
            cout << " -> " << tem[p];
            sum += abs(tem[p] - pre);
            pre = tem[p];
            p--;
        }
    }
    cout << endl
         << sum << endl;
}

void CSCAN()
{
    cout << "C-SCAN" << endl;
    int pre = s;
    cout << pre;
    long sum = 0;
    vector<int> tem(query);
    tem.push_back(s);
    sort(tem.begin(), tem.end());
    int direction = abs(s - 0) <= abs(s - (m - 1)) ? -1 : 1;
    int p = -1;
    // init p
    for (i = 0; i < n + 1; i++)
    {
        if (tem[i] == s)
        {
            p = i;
            break;
        }
    }
    int q = p;
    p--;
    q++;
    if (-1 == direction)
    {
        while (p >= 0)
        {
            cout << " -> " << tem[p];
            sum += abs(tem[p] - pre);
            pre = tem[p];
            p--;
        }
        cout << " -> " << 0 << " -> " << m - 1;
        sum += abs(0 - pre);
        pre = m - 1;
        i = n;
        while (q <= i)
        {
            cout << " -> " << tem[i];
            sum += abs(tem[i] - pre);
            pre = tem[i];
            i--;
        }
    }
    else
    {
        while (q < n + 1)
        {
            cout << " -> " << tem[q];
            sum += abs(tem[q] - pre);
            pre = tem[q];
            q++;
        }
        cout << " -> " << m - 1 << " -> " << 0;
        sum += abs(m - 1 - pre);
        pre = 0;
        i = 0;
        while (i <= p)
        {
            cout << " -> " << tem[i];
            sum += abs(tem[i] - pre);
            pre = tem[i];
            i++;
        }
    }
    cout << endl
         << sum << endl;
}

void LOOK()
{
    cout << "LOOK" << endl;
    int pre = s;
    cout << pre;
    long sum = 0;
    vector<int> tem(query);
    tem.push_back(s);
    sort(tem.begin(), tem.end());
    int direction = abs(s - 0) <= abs(s - (m - 1)) ? -1 : 1;
    int p = -1;
    // init p
    for (i = 0; i < n + 1; i++)
    {
        if (tem[i] == s)
        {
            p = i;
            break;
        }
    }
    int q = p;
    p--;
    q++;
    if (-1 == direction)
    {
        while (p >= 0)
        {
            cout << " -> " << tem[p];
            sum += abs(tem[p] - pre);
            pre = tem[p];
            p--;
        }
        while (q < n + 1)
        {
            cout << " -> " << tem[q];
            sum += abs(tem[q] - pre);
            pre = tem[q];
            q++;
        }
    }
    else
    {
        while (q < n + 1)
        {
            cout << " -> " << tem[q];
            sum += abs(tem[q] - pre);
            pre = tem[q];
            q++;
        }
        while (p >= 0)
        {
            cout << " -> " << tem[p];
            sum += abs(tem[p] - pre);
            pre = tem[p];
            p--;
        }
    }
    cout << endl
         << sum << endl;
}

void CLOOK()
{
    cout << "C-LOOK" << endl;
    int pre = s;
    cout << pre;
    long sum = 0;
    vector<int> tem(query);
    tem.push_back(s);
    sort(tem.begin(), tem.end());
    int direction = abs(s - 0) <= abs(s - (m - 1)) ? -1 : 1;
    int p = -1;
    // init p
    for (i = 0; i < n + 1; i++)
    {
        if (tem[i] == s)
        {
            p = i;
            break;
        }
    }
    int q = p;
    p--;
    q++;
    if (-1 == direction)
    {
        while (p >= 0)
        {
            cout << " -> " << tem[p];
            sum += abs(tem[p] - pre);
            pre = tem[p];
            p--;
        }
        i = n;
        pre = tem[i];
        while (q <= i)
        {
            cout << " -> " << tem[i];
            sum += abs(tem[i] - pre);
            pre = tem[i];
            i--;
        }
    }
    else
    {
        while (q < n + 1)
        {
            cout << " -> " << tem[q];
            sum += abs(tem[q] - pre);
            pre = tem[q];
            q++;
        }
        i = 0;
        pre = tem[i];
        while (i <= p)
        {
            cout << " -> " << tem[i];
            sum += abs(tem[i] - pre);
            pre = tem[i];
            i++;
        }
    }
    cout << endl
         << sum << endl;
}
