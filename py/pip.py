from functools import reduce
nums = [1,2,3,4,5,6]
def even(nums):
   for num in nums:
       if num % 2 == 0:
           yield num
def mul3(nums):
    for num in nums:
        yield num*3

def to_string(nums):
    for num in nums:
        yield str(num)

pipline = to_string(mul3(even(nums)))

# #function pipline:
# def even_func(nums):
    # return filter(lambda x: x%2==0,nums)

# def mul3_func(nums):
    # return map(lambda x: x*3,nums)
# def toStr_func(nums):
    # return map(lambda x: str(x),nums)
# def pipline_func(data, fns):
    # return reduce(lambda a,x:x(a),fns,data)

# pipline2 = pipline_func(nums, [even_func,mul3_func,toStr_func]) 


# print(list(pipline2))

class Pipe(object):
    def __init__(self,func):
        self.func = func
    def __ror__(self,other):
        def generator():
              yield self.func(other)
        return generator()
@Pipe
def even_filter(num):
    return num if num%2 ==0 else None
@Pipe
def mul_by_3(num):
    return num*3

@Pipe
def to_str(num):
    return str(num)
@Pipe
def echo(item):
    for num in item:
        for x in num:
            print(x)
@Pipe
def even_func(nums):
    return filter(lambda x: x%2==0,nums)

@Pipe
def mul3_func(nums):
    return map(lambda x: x*3,nums)
@Pipe
def toStr_func(nums):
    return map(lambda x: str(x),nums)
def force(sqs):
    for item in sqs: pass

# ,force(nums|even_func|mul3_func|toStr_func|echo)
# pipline3 = nums|even_func|mul3_func|toStr_func
force(nums|even_func|mul3_func|toStr_func|echo)






