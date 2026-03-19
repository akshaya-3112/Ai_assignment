#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 5
#define INF 9999

typedef struct {
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
    return abs(x1 - x2) + abs(y1 - y2);
}

void printPath(Node nodes[N][N], int sx, int sy, int gx, int gy) {
    int pathX[100], pathY[100];
    int len = 0;

    int x = gx, y = gy;

    while (!(x == sx && y == sy)) {
        pathX[len] = x;
        pathY[len] = y;

        int px = nodes[x][y].parentX;
        int py = nodes[x][y].parentY;

        x = px;
        y = py;
        len++;
    }

    pathX[len] = sx;
    pathY[len] = sy;

    printf("Path: ");
    for (int i = len; i >= 0; i--) {
        printf("(%d,%d) ", pathX[i], pathY[i]);
    }
    printf("\n");
}

int aStar(int grid[N][N], int sx, int sy, int gx, int gy, Node nodes[N][N]) {
    int closed[N][N] = {0};

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            nodes[i][j].f = INF;

    nodes[sx][sy].g = 0;
    nodes[sx][sy].h = heuristic(sx, sy, gx, gy);
    nodes[sx][sy].f = nodes[sx][sy].h;
    nodes[sx][sy].parentX = sx;
    nodes[sx][sy].parentY = sy;

    while (1) {
        int minF = INF, x = -1, y = -1;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (!closed[i][j] && nodes[i][j].f < minF) {
                    minF = nodes[i][j].f;
                    x = i;
                    y = j;
                }
            }
        }

        if (x == -1) return 0;

        if (x == gx && y == gy) {
            printPath(nodes, sx, sy, gx, gy);
            return 1;
        }

        closed[x][y] = 1;

        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};

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
    int steps = 0, maxSteps = 100;

    printf("Starting UGV Simulation...\n");

    while (!(currentX == gx && currentY == gy) && steps < maxSteps) {

        Node nodes[N][N];

        printf("\nPlanning from (%d,%d)...\n", currentX, currentY);

        if (!aStar(grid, currentX, currentY, gx, gy, nodes)) {
            printf("❌ No path available!\n");
            return;
        }

        int nextX = gx;
        int nextY = gy;

        int safety = 0;

        while (!(nodes[nextX][nextY].parentX == currentX &&
                 nodes[nextX][nextY].parentY == currentY)) {

            int px = nodes[nextX][nextY].parentX;
            int py = nodes[nextX][nextY].parentY;

            if (px == nextX && py == nextY) {
                printf("⚠️ Path reconstruction failed. Stopping.\n");
                return;
            }

            nextX = px;
            nextY = py;

            safety++;
            if (safety > 50) {
                printf("⚠️ Infinite loop avoided. Stopping.\n");
                return;
            }
        }

        if (nextX == currentX && nextY == currentY) {
            printf("⚠️ UGV stuck! Stopping.\n");
            return;
        }

        currentX = nextX;
        currentY = nextY;

        printf("Moved to (%d,%d)\n", currentX, currentY);

        // Dynamic obstacle example
        if (steps == 2) {
            printf("⚠️ Dynamic obstacle added at (2,2)\n");
            grid[2][2] = 1;
        }

        steps++;
    }

    if (currentX == gx && currentY == gy)
        printf("\n🎯 Goal Reached in %d steps!\n", steps);
    else
        printf("\n⛔ Stopped (limit reached)\n");
}

int main() {
    int grid[N][N] = {
        {0,0,0,0,0},
        {0,1,1,0,0},
        {0,0,0,0,0},
        {0,1,0,1,0},
        {0,0,0,0,0}
    };

    int startX = 0, startY = 0;
    int goalX = 4, goalY = 4;

    simulateUGV(grid, startX, startY, goalX, goalY);

    return 0;
}
