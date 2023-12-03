from collections import defaultdict
grid = defaultdict(lambda: defaultdict(lambda: '.'))
for y, row in enumerate(open('in.txt').readlines()):
    for x, c in enumerate(row):
        grid[y][x] = c
X = len(grid[0])
Y = len(grid)

adjacent = {(0, 1),(0, -1),(-1, 0),(1, 0),(1, 1),(1, -1),(-1, 1),(-1, -1)}

def adjecent_to(x, y):
    for dx, dy in adjacent:
        c = grid[y+dy][x+dx]
        if c != '.\r\n':
            yield c, (x+dx, y+dy)

parts = defaultdict(lambda: defaultdict(set))

for y in range(Y):
    num = ''
    for x in range(X):
        c = grid[y][x]
        if c.isdigit():
            num += c
        else:
            for i, c in enumerate(num):
                for part, pos in adjecent_to(x+i-len(num), y):
                    if part in '.\r\n0123456789':
                        continue
                    # storing (x,y) to handle cases where 2 identical numbers are next to a part.
                    parts[part][pos].add(((x,y),int(num)))
            num = ''

part_nums = set()
for part, near in parts.items():
    for _, nums in near.items():
        part_nums.update(nums)
print(sum([x for  _, x in part_nums]))

p2 = 0
for nums in parts['*'].values():
    if len(nums) == 2:
        (_, a), (_, b) = nums
        p2 += a * b
print(p2)