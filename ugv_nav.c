#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 10

typedef struct {
    int x, y;
    int g, h, f;
    int parentX, parentY;
} Node;

int isValid(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N);
}

int isUnblocked(int grid[N][N], int x, int y) {
    return grid[x][y] == 0;
}

int heuristic(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2); // Manhattan
}

void aStar(int grid[N][N], int sx, int sy, int gx, int gy) {
    int closed[N][N] = {0};
    Node nodes[N][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            nodes[i][j].f = 9999;
        }
    }

    nodes[sx][sy].g = 0;
    nodes[sx][sy].h = heuristic(sx, sy, gx, gy);
    nodes[sx][sy].f = nodes[sx][sy].h;

    while (1) {
        int minF = 9999, x = -1, y = -1;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (!closed[i][j] && nodes[i][j].f < minF) {
                    minF = nodes[i][j].f;
                    x = i; y = j;
                }
            }
        }

        if (x == -1) break;
        if (x == gx && y == gy) {
            printf("Goal reached!\n");
            return;
        }

        closed[x][y] = 1;

        int dx[] = {1,-1,0,0};
        int dy[] = {0,0,1,-1};

        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if (isValid(nx, ny) && isUnblocked(grid, nx, ny)) {
                int gNew = nodes[x][y].g + 1;
                int hNew = heuristic(nx, ny, gx, gy);
                int fNew = gNew + hNew;

                if (nodes[nx][ny].f > fNew) {
                    nodes[nx][ny].f = fNew;
                    nodes[nx][ny].g = gNew;
                    nodes[nx][ny].h = hNew;
                    nodes[nx][ny].parentX = x;
                    nodes[nx][ny].parentY = y;
                }
            }
        }
    }

    printf("No path found\n");
}

int main() {
    int grid[N][N] = {0};

    // Example obstacles
    grid[1][2] = 1;
    grid[2][2] = 1;
    grid[3][2] = 1;

    aStar(grid, 0, 0, 7, 7);

    return 0;
}
