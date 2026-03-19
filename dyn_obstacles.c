#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 10
#define INF 9999

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

void printPath(Node nodes[N][N], int gx, int gy) {
    int pathX[100], pathY[100];
    int len = 0;

    int x = gx, y = gy;

    while (!(nodes[x][y].parentX == x && nodes[x][y].parentY == y)) {
        pathX[len] = x;
        pathY[len] = y;
        int tempX = nodes[x][y].parentX;
        int tempY = nodes[x][y].parentY;
        x = tempX;
        y = tempY;
        len++;
    }

    pathX[len] = x;
    pathY[len] = y;

    printf("Path:\n");
    for (int i = len; i >= 0; i--) {
        printf("(%d,%d) ", pathX[i], pathY[i]);
    }
    printf("\n");
}

int aStar(int grid[N][N], int sx, int sy, int gx, int gy, Node nodes[N][N]) {
    int closed[N][N] = {0};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            nodes[i][j].f = INF;
        }
    }

    nodes[sx][sy].g = 0;
    nodes[sx][sy].h = heuristic(sx, sy, gx, gy);
    nodes[sx][sy].f = nodes[sx][sy].h;
    nodes[sx][sy].parentX = sx;
    nodes[sx][sy].parentY = sy;

    while (1) {
        int minF = INF, x = -1, y = -1;

        // Find lowest f
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (!closed[i][j] && nodes[i][j].f < minF) {
                    minF = nodes[i][j].f;
                    x = i; y = j;
                }
            }
        }

        if (x == -1) return 0;

        if (x == gx && y == gy) {
            printPath(nodes, gx, gy);
            return 1;
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
}

void simulateUGV(int grid[N][N], int sx, int sy, int gx, int gy) {
    int currentX = sx, currentY = sy;

    printf("Starting UGV Simulation...\n");

    while (!(currentX == gx && currentY == gy)) {

        Node nodes[N][N];

        printf("\nPlanning from (%d,%d)...\n", currentX, currentY);

        if (!aStar(grid, currentX, currentY, gx, gy, nodes)) {
            printf("No path available!\n");
            return;
        }

        // Move ONE STEP along path
        int nextX = nodes[gx][gy].parentX;
        int nextY = nodes[gx][gy].parentY;

        currentX = nextX;
        currentY = nextY;

        printf("Moved to (%d,%d)\n", currentX, currentY);

        // 🔴 Dynamic obstacle appears
        if (currentX == 2 && currentY == 0) {
            printf("Dynamic obstacle detected at (3,0)! Replanning...\n");
            grid[3][0] = 1;
        }
    }

    printf("\n🎯 Goal Reached!\n");
}

int main() {
    int grid[N][N] = {0};

    // Static obstacles
    grid[1][2] = 1;
    grid[2][2] = 1;
    grid[3][2] = 1;

    int startX = 0, startY = 0;
    int goalX = 7, goalY = 7;

    simulateUGV(grid, startX, startY, goalX, goalY);

    return 0;
}
