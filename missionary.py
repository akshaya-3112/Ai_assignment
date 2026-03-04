from collections import deque
import time

TOTAL_M = 3
TOTAL_C = 3

# Check if a state is valid
def is_valid(m, c):
    if m < 0 or c < 0 or m > TOTAL_M or c > TOTAL_C:
        return False

    if m > 0 and m < c:
        return False

    m_right = TOTAL_M - m
    c_right = TOTAL_C - c

    if m_right > 0 and m_right < c_right:
        return False

    return True


# Generate next possible states
def get_successors(state):
    m, c, boat = state
    moves = [(1,0),(2,0),(0,1),(0,2),(1,1)]
    successors = []

    for move in moves:
        dm, dc = move

        if boat == 'L':
            new_state = (m-dm, c-dc, 'R')
        else:
            new_state = (m+dm, c+dc, 'L')

        if is_valid(new_state[0], new_state[1]):
            successors.append(new_state)

    return successors


# BFS Search
def bfs(start, goal):
    start_time = time.time()

    queue = deque([[start]])
    visited = set()
    nodes_explored = 0

    while queue:
        path = queue.popleft()
        state = path[-1]
        nodes_explored += 1

        if state == goal:
            return path, nodes_explored, time.time() - start_time

        if state not in visited:
            visited.add(state)

            for next_state in get_successors(state):
                new_path = list(path)
                new_path.append(next_state)
                queue.append(new_path)

    return None


# DFS Search
def dfs(start, goal):
    start_time = time.time()

    stack = [[start]]
    visited = set()
    nodes_explored = 0

    while stack:
        path = stack.pop()
        state = path[-1]
        nodes_explored += 1

        if state == goal:
            return path, nodes_explored, time.time() - start_time

        if state not in visited:
            visited.add(state)

            for next_state in get_successors(state):
                new_path = list(path)
                new_path.append(next_state)
                stack.append(new_path)

    return None


start = (3,3,'L')
goal = (0,0,'R')

print("----- BFS Search -----")
bfs_path, bfs_nodes, bfs_time = bfs(start, goal)

print("Solution Path:")
for step in bfs_path:
    print(step)

print("Nodes explored:", bfs_nodes)
print("Time:", bfs_time)


print("\n----- DFS Search -----")
dfs_path, dfs_nodes, dfs_time = dfs(start, goal)

print("Solution Path:")
for step in dfs_path:
    print(step)

print("Nodes explored:", dfs_nodes)
print("Time:", dfs_time)
