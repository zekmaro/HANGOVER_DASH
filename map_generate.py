import random

terrain_types = ['W', 'A', 'E']
special_positions = ['MM', 'GG']

def generate_map(rows, cols):
    game_map = [['  ' for _ in range(cols)] for _ in range(rows)]

    marvin_pos = (random.randint(0, rows - 1), random.randint(0, cols - 1))
    goal_pos = (random.randint(0, rows - 1), random.randint(0, cols - 1))

    while marvin_pos == goal_pos:
        goal_pos = (random.randint(0, rows - 1), random.randint(0, cols - 1))

    game_map[marvin_pos[0]][marvin_pos[1]] = 'MM'
    game_map[goal_pos[0]][goal_pos[1]] = 'GG'

    for i in range(rows):
        for j in range(cols):
            if game_map[i][j] == '  ':
                terrain = random.choice(terrain_types)
                number = random.randint(1, 9)
                game_map[i][j] = f'{terrain}{number}'

    return game_map

def print_map(game_map):
    for row in game_map:
        print(''.join(row))

rows = 1000
cols = 1000

game_map = generate_map(rows, cols)
print_map(game_map)
