#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
using namespace std;
double eps = 1e-9;
double gamma = 0.9;
double a[30][30];
int n = 25;
map<string, pair<int, int>> d;
int mp[6][6] = {
    {0, 0, 0, 0, 0, 0},
    {0, 1, 3, 2, 3, 1},
    {0, 2, 4, 4, 4, 2},
    {0, 2, 4, 4, 4, 2},
    {0, 2, 4, 4, 4, 2},
    {0, 1, 2, 2, 2, 1}};
string oper[4] = {"up", "down", "left", "right"};
int getmask(int x, int y)
{
    return 5 * (x - 1) + (y - 1);
}
int gauss()
{
    int column, row;
    for (column = 0, row = 0; column < n; column++)
    {
        int t = row;
        for (int i = row; i < n; i++)
            if (fabs(a[i][column]) > fabs(a[t][column])) // 找到当前元最大的一行
                t = i;
        if (fabs(a[t][column]) < eps) // 当前列所有数都是0
            continue;
        for (int i = column; i < n + 1; i++) // 交换两行
            swap(a[t][i], a[row][i]);
        for (int i = n; i >= column; i--) // 把主元变成1
            a[row][i] /= a[row][column];
        for (int i = row + 1; i < n; i++)
            if (fabs(a[i][column]) > eps)
                for (int j = n; j >= column; j--) // 消元，乘以-a[i][column]倍
                    a[i][j] -= a[row][j] * a[i][column];
        row++;
    }
    if (row < n)
    {
        for (int i = row; i < n; i++)
            if (fabs(a[i][n]) > eps)
                return -1; // 0!=0
        return n - row;    // rank(a)
    }
    for (int i = n - 1; i >= 0; i--)
        for (int j = i + 1; j < n; j++)
            a[i][n] -= a[j][n] * a[i][j]; // 回代。
    return 0;
}

int main()
{
    freopen("result.csv", "w", stdout);
    d["up"] = {-1, 0};
    d["down"] = {1, 0};
    d["left"] = {0, -1};
    d["right"] = {0, 1};
    for (int i = 1; i <= 5; i++)
    {
        for (int j = 1; j <= 5; j++)
        {
            if (mp[i][j] == 1)
            {
                a[getmask(i, j)][getmask(i, j)] = (2 - gamma) / 2;
                a[getmask(i, j)][25] = -0.5;
                for (int k = 0; k < 4; k++)
                {
                    int newi = i + d[oper[k]].first;
                    int newj = j + d[oper[k]].second;
                    if (newi < 1 || newi > 5 || newj < 1 || newj > 5)
                        continue;
                    a[getmask(i, j)][getmask(newi, newj)] = -1.0 * gamma / 4;
                }
            }
            if (mp[i][j] == 2)
            {
                a[getmask(i, j)][getmask(i, j)] = (4 - gamma) / 4;
                a[getmask(i, j)][25] = -0.25;
                for (int k = 0; k < 4; k++)
                {
                    int newi = i + d[oper[k]].first;
                    int newj = j + d[oper[k]].second;
                    if (newi < 1 || newi > 5 || newj < 1 || newj > 5)
                        continue;
                    a[getmask(i, j)][getmask(newi, newj)] = -1.0 * gamma / 4;
                }
            }
            if (mp[i][j] == 3)
            {
                a[getmask(i, j)][getmask(i, j)] = 1;
                if (getmask(i, j) == 1)
                {
                    a[getmask(i, j)][25] = 10;
                    a[getmask(i, j)][21] = -1.0 * gamma;
                }
                if (getmask(i, j) == 3)
                {
                    a[getmask(i, j)][25] = 5;
                    a[getmask(i, j)][13] = -1.0 * gamma;
                }
            }
            if (mp[i][j] == 4)
            {
                a[getmask(i, j)][getmask(i, j)] = 1;
                a[getmask(i, j)][25] = 0;
                for (int k = 0; k < 4; k++)
                {
                    int newi = i + d[oper[k]].first;
                    int newj = j + d[oper[k]].second;
                    if (newi < 1 || newi > 5 || newj < 1 || newj > 5)
                        continue;
                    a[getmask(i, j)][getmask(newi, newj)] = -1.0 * gamma / 4;
                }
            }
        }
    }
    gauss();
    for (int i = 0; i < 25; i++)
    {
        cout << fixed << setprecision(2) << a[i][25] << ',';
        if (i % 5 == 4)
            cout << endl;
    }
    return 0;
}