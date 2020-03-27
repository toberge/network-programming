from collections import deque

operators = {
    '+': lambda x, y: y + x,
    '-': lambda x, y: y - x,
    '*': lambda x, y: y * x,
    '/': lambda x, y: y / x
}

def process(expression):
    """Parse and calculate an expression in RPN"""
    if len(expression) == 0: raise Exception('Empty expression')
    stack = deque()
    for item in expression:
        # kinda not the way you wanna do it but ok
        if isinstance(item, int) or isinstance(item, float):
            stack.append(int(item))
        elif item in operators:
            if len(stack) > 1:
                stack.append(operators[item](stack.pop(), stack.pop()))
            else:
                raise Exception('Too few operands, at least one missing')
        else:
            raise Exception('Invalid character')
    res = stack.pop()
    if len(stack) > 0:
        raise Exception(f'Too few operators, {len(stack)} missing')
    else:
        return res

if __name__ == '__main__':
    print(process([2, 3, '+', 4, '-']))
