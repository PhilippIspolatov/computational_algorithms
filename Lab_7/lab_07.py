from prettytable import PrettyTable
from math import *

def scan_float(string):
    right = 0
    while (right == 0):
        try:
            num = float(input(string))
            right = 1
        except:
            print("Некорректные данные, введите вещественное число.\n")
    return num

def scan_int(string):
    right = 0
    while (right == 0):
        try:
            num = int(input(string))
            right = 1
        except:
            print("Некорректные данные, введите целое число.\n")
    return num

def scan_positive_int(string):
    num = -1
    while (num <= 0):
        num = scan_int(string)
        if (num <= 0):
            print("Некорректные данные, введите положительное целое число.\n")      
    return num

def y(x):
    y = (a0 * x) / (a1 + a2 * x)
    return y
    return x*x
    return log(x)

def get_nodes(N, x0, h):
    x = x0
    X = []
    Y = []
    for i in range(N):
        X.append(x)
        Y.append(y(x))
        x += h
    return X, Y

def formating(A, N):
    for i in range(N):
        A[i] = "{:10.4f}".format(A[i])
    return A

def print_table(N, X, Y, ONE, CENTRE, RUNGE, EQUAL):
    table = PrettyTable()
    table.add_column("X", formating(X, N))
    table.add_column("Y", formating(Y, N))
    table.add_column("Односторонняя", formating(ONE, N))
    table.add_column("Центральная + y(0)' и y(N)'", formating(CENTRE, N))
    table.add_column("Формула Рунге", formating(RUNGE, N))
    table.add_column("Выравнивание переменных", formating(EQUAL, N)) 
    print(table)            

# односторонняя производная
def oneSided(y_cur, y_prev, h):
    return (y_cur - y_prev) / h

def get_ONE(Y, N, h):
    ONE = [0] * N
    ONE[0] = oneSided(Y[1], Y[0], h) 
    for i in range(1, N):
        ONE[i] = oneSided(Y[i], Y[i - 1], h)
    return ONE

def get_ONE_2h(Y, N, h):
    ONE = [0] * N
    ONE[0] = oneSided(Y[2], Y[0], 2 * h)
    ONE[1] = oneSided(Y[3], Y[1], 2 * h) 
    for i in range(2, N):
        ONE[i] = oneSided(Y[i], Y[i - 2], 2 * h)
    return ONE

# центральная производная
def centreSided(y_next, y_prev, h):
    return (y_next - y_prev) / (2 * h)

def get_CENTRE(Y, N, h):
    CENTRE = [0] * N
    for i in range(1, N - 1):
        CENTRE[i] = centreSided(Y[i + 1], Y[i - 1], h)
    return CENTRE

# крайние узлы
def get_y0_d(Y, h):
    return (-3 * Y[0] + 4 * Y[1] - Y[2]) / (2 * h)

def get_yN_d(Y, h):
    return (3 * Y[N - 1] - 4 * Y[N - 2] + Y[N - 3]) / (2 * h)
        
# 2 формула Рунге
def get_RUNGE(ONE, ONE_2h, N):
    RUNGE = [0] * N
    for i in range(N):
        RUNGE[i] = ONE[i] + (ONE[i] - ONE_2h[i])
    return RUNGE

# выравнивание переменных
def equal_var(X, Y):
    EQUAL = [0] * N
    for i in range(N):
        EQUAL[i] = a1 / a0 * Y[i]**2 / X[i]**2
    return EQUAL

a0 = 100
a1 = 10
a2 = 5
while (True):
    #N = 12
    #x0 = 0.5
    #h = 0.5
    N = scan_positive_int("\n\nКоличество узлов: ")
    x0 = scan_float("Начальное значение : ")
    h = scan_float("Шаг: ")
    X, Y = get_nodes(N, x0, h)
    ONE = get_ONE(Y, N, h)
    ONE_2h = get_ONE_2h(Y, N, h)
    CENTRE = get_CENTRE(Y, N, h)

    y0_d = get_y0_d(Y, h)
    yN_d = get_yN_d(Y, h)
    CENTRE[0] = y0_d
    CENTRE[N - 1] = yN_d

    RUNGE = get_RUNGE(ONE, ONE_2h, N)
    EQUAL = equal_var(X, Y)

    print_table(N, X, Y, ONE, CENTRE, RUNGE, EQUAL)

    input("\n\nEnter...\n")
        
