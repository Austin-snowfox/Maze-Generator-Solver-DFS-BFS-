# Maze Generator & Solver (DFS + BFS)
> 依照輸入大小隨機生成迷宮、自動尋找最遠終點、動畫演示解謎路徑

## 題目敘述
請讓使用者可以自由選擇迷宮大小(最大80x80)，並且生成一個迷宮，秀出起點和終點，其中終點的位置是距離起點最遠的點，最後印出起點到終點的路徑

## 專案簡介
這是一個用 C++ 撰寫的迷宮模擬，程式裡結合了三種演算法邏輯：
- **隨機生成迷宮**：使用 DFS (Depth-First Search) 遞迴搭配 random_shuffle 函式挖出通道，確保每次執行都是不同的迷宮
- **自動選定終點**：使用 BFS (Breadth-First Search) 從起點找出距離最遠的格子作為終點
- **動態解謎演示**：再使用一次 DFS，並在終端機動畫呈現尋路過程，走錯路會自動回溯

## 使用技術
- **語言**：C++11/14
- **核心演算法**：DFS（迷宮生成 / 路徑搜尋）、BFS（最遠點搜尋）
- **技巧**：遞迴回溯 (recursive backtracking)、二維陣列邏輯、Windows Console API（游標控制做動畫效果）

## 關於 `random_shuffle`
本專案撰寫時使用 `std::random_shuffle` 來打亂迷宮生成的方向順序，這在當時的 C++ 標準（C++11/14）下是正確寫法。但此函式已於 C++17 中被移除，若在較新的編譯器環境下建置失敗，可改用 `std::shuffle` 搭配 `<random>` 提供的隨機引擎：

```cpp
#include <random>

static std::mt19937 rng(std::random_device{}());
std::shuffle(dirs.begin(), dirs.end(), rng);
```

## 執行方式
執行後依提示輸入迷宮的長寬 (例如 `20 20`)，即可看到迷宮自動生成與解謎過程。

## 開發資訊
- 課程：程式設計與實習
- 開發者：Austin 詹育綸
