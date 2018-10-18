def toUpper(item):
    return item.upper()
upper_name = map(toUpper,["hao"])

num = [2,-5,9,7]

# positive_num_cnt = 0
# positive_num_sum = 0

# for i in range(len(num)):
   # if num[i] > 0:
       # positive_num_cnt  +=1
       # positive_num_sum  +=num[i]

# if positive_num_cnt > 0:
    # average = positive_num_sum / positive_num_cnt

# print(average)
from functools import reduce
# calc average using functon 
positive_num = list(filter(lambda x: x >0, num))
average = reduce(lambda x,y: x+y, positive_num)/len(positive_num))

print(average)

# print (list(upper_name))

