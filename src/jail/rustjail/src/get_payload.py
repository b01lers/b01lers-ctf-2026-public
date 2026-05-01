import sys

FORBIDDEN = [
    'unsafe',
    'path',
    'flag',
    'std',
    'alloc',
    'core',
    'include',
    'impl',
    'concat',
    'macro',
    '<',
    '>',
    '\'',
]

def check_payload(payload: str):
    for s in FORBIDDEN:
        if s in payload:
            print(f'nice try, `{s}` is not allowed in your code')
            sys.exit(1)


def main():
    print('Enter your payload, terminate with 3 newlines in a row:')

    payload = ''
    newline_count = 0

    while True:
        line = input() + '\n'
        payload += line

        if line == '\n':
            newline_count += 1
            if newline_count == 3:
                break
        else:
            newline_count = 0

    check_payload(payload)

    with open('src/main.rs', 'w') as f:
        f.write(payload)

if __name__ == '__main__':
    main()
