import math

epsila = 10**(-6)
error_phi = epsila / 1.68
error_g = epsila / 2.625
error_f = epsila / 3.75

def arctan(x) :
    result = 0
    n = 0
    term = x
    while abs(term) > error_phi:
        result += term
        n += 1
        term = (-1)**n * (x**(2*n + 1)) / (2*n + 1)
    return result

def cos(x):
    result = 0
    term = 1
    n = 0
    while abs(term) > error_g:
        term = ((-1) ** n) * (x ** (2 * n)) / math.factorial(2 * n)
        result += term
        n += 1
    return result

def sqroot(number, eps):
    a = float(number) # из какого числа извлекаем
    x0=0 #предыдущее значение
    x1=a #начальное приближение
    while abs(x0-x1)>eps:
        x0=x1
        x1 = 0.5 * (x1 + a / x1) # x_(n+1) = 0.5 * (x_n + a / x_n)
    return x1

x = 0.01
for m in range (0, 9, 1):
    arc_phi = 1 + arctan(16.7*x+0.1);
    cos_g = cos(7*x + 0.3);
    sqrt_ans = sqroot(arc_phi, error_f)
    ans_my = sqrt_ans / cos_g
    ans_func = math.sqrt(1 + math.atan(16.7*x+0.1))/math.cos(7*x+0.3)
    delta_z = abs(ans_func - ans_my)
    print(str(x) + " | " + str(ans_func) + " | " + str(ans_my) + " | " + str(delta_z) + " | " + str(10**(-6) > delta_z >= 10**(-10)))
    x += 0.005