# /usr/bin/env python3
# -*- encoding:UTF-8 -*-
import functools

def mod(m, key=2):
    return m % key == 0

mod_to_2 = functools.partial(mod, key=2)

print('A__3___使用原函数的默认关键字参数对2进行求余：')
print(mod(3))  # 对2进行求余-- 原函数 使用默认参数

print('B__3___使用偏函数对2进行求余：')
print(mod_to_2(3))  # 对2进行求余-- 新函数 --偏函数产生

mod_to_5 = functools.partial(mod, key=5)

print('C__25___使用原函数的关键字参数对5进行求余：')
print(mod(25, key=5))  # 对5进行求余 -- 原函数

print('D__25___使用偏函数对5进行求余：')
print(mod_to_5(25))  # 对5进行求余 -- 新函数--偏函数产生
