from collections import deque, defaultdict


def solve_puzzle(initial_tiles: dict[int, frozenset[int]]) -> str | None:
    start_state_tuple = tuple(sorted((k, v) for k, v in initial_tiles.items()))
    queue = deque([(initial_tiles, "")])
    seen = {start_state_tuple}

    while queue:
        state, moves = queue.popleft()

        # Win condition: car 0 reaches the exit at index 17
        if 17 in state[0]:
            return moves

        used = set(v for s in state.values() for v in s)

        for i in state:
            # Directions: 0:L, 1:R for 0..7; 2:U, 3:D for 8..15
            if i < 8:
                # left (0)
                new_indices = frozenset(v - 1 for v in state[i])
                if all(v >= 0 and v % 6 != 5 and (v not in used or v in state[i]) for v in new_indices):
                    new_state = {**state, i: new_indices}
                    m = tuple(sorted((k, v) for k, v in new_state.items()))
                    if m not in seen:
                        seen.add(m)
                        queue.append((new_state, moves + f"{i:x}0"))

                # right (1)
                new_indices = frozenset(v + 1 for v in state[i])
                if all(v < 36 and v % 6 != 0 and (v not in used or v in state[i]) for v in new_indices):
                    new_state = {**state, i: new_indices}
                    m = tuple(sorted((k, v) for k, v in new_state.items()))
                    if m not in seen:
                        seen.add(m)
                        queue.append((new_state, moves + f"{i:x}1"))
            else:
                # up (2)
                new_indices = frozenset(v - 6 for v in state[i])
                if all(v >= 0 and (v not in used or v in state[i]) for v in new_indices):
                    new_state = {**state, i: new_indices}
                    m = tuple(sorted((k, v) for k, v in new_state.items()))
                    if m not in seen:
                        seen.add(m)
                        queue.append((new_state, moves + f"{i:x}2"))

                # down (3)
                new_indices = frozenset(v + 6 for v in state[i])
                if all(v < 36 and (v not in used or v in state[i]) for v in new_indices):
                    new_state = {**state, i: new_indices}
                    m = tuple(sorted((k, v) for k, v in new_state.items()))
                    if m not in seen:
                        seen.add(m)
                        queue.append((new_state, moves + f"{i:x}3"))
    return None


def parse(pzl: str) -> dict:
    pzl = pzl.replace("\n", "")
    tiles = defaultdict(set)
    for i, c in enumerate(pzl):
        if c == ".":
            continue
        tiles[int(c, 16)].add(i)
    return {k: frozenset(v) for k, v in tiles.items()}


def main():
    data = open("src/puzzles.txt").read()
    puzzles = [parse(pzl) for pzl in data.strip().split("\n\n")]

    for i, p in enumerate(puzzles):
        solution = solve_puzzle(p)
        if solution is not None:
            print(solution)
        else:
            print(f"No solution for puzzle {i}")


if __name__ == "__main__":
    main()
