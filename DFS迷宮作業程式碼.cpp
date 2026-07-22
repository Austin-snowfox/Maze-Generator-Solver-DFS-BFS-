#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <windows.h>
using namespace std;

void moveCursorToTop() {
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


int side_x, side_y;
int start_x, start_y;
int goal_x, goal_y;
int step_count = 0, way = 0;
int max_depth=0;

int maze[80][80];           // 0=通道, 1=牆, 2=路徑
bool visited[80][80];       // DFS 記錄用
bool found = false;

// 分別代表xy座標的{右, 左, 下, 上}
int dx[4] = { 0, 0, 2, -2 };
int dy[4] = { 2, -2, 0, 0 };

// 迷宮顯示
void print_maze() {
    moveCursorToTop();
    for (int i = 0; i < side_x; i++) {
        for (int j = 0; j < side_y; j++) {
            if (i == start_x && j == start_y) cout << "S";
            else if (i == goal_x && j == goal_y) cout << "E";
            else if (maze[i][j] == 1) cout << "#";
            else if (maze[i][j] == 2) cout << "*";
            else cout << " ";
        }
        cout << endl;
    }
    Sleep(50); // 延遲以產生動畫效果
}

// 隨機 DFS 挖迷宮
void generate_maze(int x, int y) {
    maze[x][y] = 0;
    vector<int> dirs = { 0, 1, 2, 3 };
    random_shuffle(dirs.begin(), dirs.end()); //隨機打亂dirs中元素順序

    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[dirs[i]];
        int ny = y + dy[dirs[i]];
        if ((nx > 0 && nx < side_x - 1) && (ny > 0 && ny < side_y - 1) && maze[nx][ny] == 1) {
            maze[x + dx[dirs[i]] / 2][y + dy[dirs[i]] / 2] = 0; // 打通中間的牆
            generate_maze(nx, ny);
        }
    }
}

//邊界隨機挖洞
void border_hole_x() {
    for (int i = 1; i < side_y - 1; i++) {
        int percent = rand() % 10 + 1; //設定一半的機率會挖洞
        if (maze[side_x - 3][i] == 0 && percent > 5 && maze[side_x - 2][i - 1] == 1) {
            maze[side_x - 2][i] = 0;
        }
    }
}
void border_hole_y() {
    for (int i = 1; i < side_x - 1; i++) {
        int percent = rand() % 10 + 1; //設定一半的機率會挖洞
        if (maze[i][side_y - 3] == 0 && percent > 5 && maze[i - 1][side_y - 2] == 1) {
            maze[i][side_y - 2] = 0;
        }
    }
}

// 從起點 BFS 找最遠的點
pair<int, int> find_furthest_point(int sx, int sy) {
    queue<pair<int, int>> q;
    bool visited_bfs[80][80] = { false };
    q.push({ sx, sy });
    visited_bfs[sx][sy] = true;

    pair<int, int> furthest = { sx, sy };

    //檢查順序也依序為{右, 左, 下, 上}
    int mx[4] = { 0, 0, 1, -1 };
    int my[4] = { 1, -1, 0, 0 };

    while (!q.empty()) {
        pair<int, int> current = q.front(); q.pop();
        int x = current.first;
        int y = current.second;
        furthest = { x, y }; //覆蓋先前最遠距離

        for (int i = 0; i < 4; ++i) {
            int nx = x + mx[i];
            int ny = y + my[i];
            if (nx >= 0 && nx < side_x && ny >= 0 && ny < side_y && maze[nx][ny] == 0 && !visited_bfs[nx][ny]) {
                visited_bfs[nx][ny] = true;
                q.push({ nx, ny });
            }
        }
    }
    return furthest;
}

// DFS 顯示路徑
void dfs(int x, int y) {
    if (found || x < 0 || y < 0 || x >= side_x || y >= side_y) return;
    if (maze[x][y] == 1 || visited[x][y]) return;

    step_count++;
    visited[x][y] = true;
    maze[x][y] = 2;
    print_maze();

    if (x == goal_x && y == goal_y) {
        found = true;
        return;
    }

    dfs(x, y + 1); // 右
    dfs(x, y - 1); // 左
    dfs(x + 1, y); // 下
    dfs(x - 1, y); // 上

    //將錯誤路徑刪除
    if (found) return;
    maze[x][y] = 0;

}

int main() {
    cout << "請輸入迷宮大小 (x y)"<<endl<<"最大為  80x80 最小為 5x5（建議輸入奇數）"<<endl<<"您的輸入：";
    cin >> side_x >> side_y;
    if (side_x > 80) side_x = 80;
    if (side_y > 80) side_y = 80;
    system("cls");
    srand(time(NULL));

    // 初始化迷宮為牆
    for (int i = 0; i < side_x; i++) {
        for (int j = 0; j < side_y; j++) {
            maze[i][j] = 1;
        }
    }

    // 從(1,1)開始挖迷宮
    generate_maze(1, 1);
    if (side_x % 2 == 0) border_hole_y();
    if (side_y % 2 == 0) border_hole_x();

    // 隨機從通道中選起點
    int S_x[6400], S_y[6400], num = 0, num_start = 0;
    for (int i = 1; i < side_x - 1; i++) {
        for (int j = 1; j < side_y - 1; j++) {
            if (maze[i][j] == 0) {
                S_x[num] = i;
                S_y[num] = j;
                num++;
            }
        }
    }
    num_start = rand() % num;
    start_x = S_x[num_start];
    start_y = S_y[num_start];

    // 找最遠的終點
    pair<int, int> p = find_furthest_point(start_x, start_y);
    goal_x = p.first;
    goal_y = p.second;

    // 尋找路徑
    dfs(start_x, start_y);
    print_maze();

    for (int i = 1; i < side_x-1; i++) {
        for (int j = 1; j < side_y; j++) {
            if (maze[i][j] == 2) way++;
        }
    }
    cout << "完成！" << endl;
    cout << "搜尋次數：" << step_count - 1 << " 路徑長：" << way << endl;

    return 0;
}
