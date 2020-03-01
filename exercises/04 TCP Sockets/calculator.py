from collections import deque

# TODO throw errors and handle them

operators = {
    '+': lambda x, y: y + x,
    '-': lambda x, y: y - x,
    '*': lambda x, y: y * x,
    '/': lambda x, y: y / x
}

def process(expression):
    """Parse and calculate an expression in RPN"""
    stack = deque()
    for item in expression.split():
        if item.isdigit():
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
