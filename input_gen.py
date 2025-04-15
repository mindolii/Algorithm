import random
import argparse

def arg():
    parser = argparse.ArgumentParser()

    parser.add_argument('--type', choices=['random', 'sorted', 'partial', 'test'], required=True)
    parser.add_argument('--lower', type = int, default = 1)
    parser.add_argument('--upper', type = int, default = 5000000)
    parser.add_argument('--order', choices=['inc', 'dec'], default='inc')
    parser.add_argument('--percent', type=float, default=0.5)
    parser.add_argument('--size', type=int, default=100000)
    parser.add_argument('--output', type=str, default='input.txt')

    args = parser.parse_args()

    return args

def rand_input(n, lower, upper):
    nums = []
    for _ in range(n):
        nums.append(str(random.randint(lower, upper)))

    nums = ','.join(nums)
    return nums

def sorted_input(n, inc = True):
    nums =list(range(1, n+1))
    if not inc:
        nums = nums[::-1]
    return ','.join(map(str, nums))

def partially_sorted_input(n, percent_sorted=0.5):
    assert 0 <= percent_sorted <= 1, "percent_sorted must be between 0 and 1"

    sorted_count = int(n * percent_sorted)

    all_data = [random.randint(1, 5000000) for _ in range(n)]
    partially_sorted = sorted(all_data[:sorted_count]) + all_data[sorted_count:]

    return ','.join(map(str, partially_sorted))

if __name__ == '__main__':
    args = arg()

    if args.type ==  'random':
        data = rand_input(args.size, args.lower, args.upper)
    elif args.type == 'sorted':
        data = sorted_input(args.size, inc = (args.order=='inc'))
    elif args.type == 'partial':
        data = partially_sorted_input(args.size, args.percent)
    elif args.type == 'test':
        data = rand_input(args.size, args.lower, args.upper)

    print(len(data.split(',')))
    
    if args.type == 'sorted':
        with open(f"input_data/sorted_{args.order}.txt", 'w') as file:
            file.write(data)
    else:
        with open(f"input_data/{args.type}.txt", 'w') as file:
            file.write(data)