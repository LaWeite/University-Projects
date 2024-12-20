import time
import random
import itertools


def lsd_radix_sort(A, m, k):
    # Вспомогательная функция для извлечения j-го разряда числа x в системе счисления с основанием k.
    # Разряды считаются с нуля, начиная с младшего.
    def get_digit(x, j):
        return (x // (k ** j)) % k

    n = len(A)
    B = [0] * n  # Временный массив для сборки отсортированных данных на текущем разряде

    # Проходим по каждому разряду от младшего к старшему (LSD - least significant digit)
    for digit_pos in range(m):
        # Создаём массив счётчиков для каждого возможного значения разряда: [0, 1, ..., k-1]
        C = [0] * k

        # Подсчитываем, сколько элементов имеет каждое значение разряда на позиции digit_pos
        for x in A:
            d = get_digit(x, digit_pos)
            C[d] += 1

        # Преобразуем счётчики в префиксные суммы, чтобы получить позиции в выходном массиве B
        for i in range(1, k):
            C[i] += C[i - 1]

        # Раскладываем элементы в массив B в соответствии с их разрядами,
        # идём с конца, чтобы обеспечить стабильность сортировки
        for i in range(n - 1, -1, -1):
            d = get_digit(A[i], digit_pos)
            C[d] -= 1
            B[C[d]] = A[i]

        # Копируем результаты назад в A для следующей итерации по следующему разряду
        A[:] = B[:]

    return A


def generate_data(n, m, k):
    max_val = k ** m - 1 if k ** m > 0 else 0
    return [random.randint(0, max_val) for _ in range(n)]


def measure_time(n, m, k, repeat=3):
    times = []
    for _ in range(repeat):
        A = generate_data(n, m, k)
        start = time.perf_counter()
        lsd_radix_sort(A, m, k)
        end = time.perf_counter()
        times.append(end - start)
    return sum(times) / len(times)


def theoretical_time(m, n, k, scenario):
    if scenario == "k_fixed":
        # k фиксировано => O(mn)
        return m * n
    elif scenario == "m_fixed":
        # m фиксировано => O(n + k)
        return n + k
    elif scenario == "both_fixed":
        # m и k фиксированы => O(n)
        return n
    else:
        # general => O(m(n + k))
        return m * (n + k)


def compare_ratios(results, scenario, var_param):
    """
    results: dict[(k,m,n)] = (real_t, theor_t)
    scenario: один из "k_fixed", "m_fixed", "both_fixed", "general"
    var_param: параметр ("n", "m", "k"), по которому мы смотрим изменение
    """
    if var_param == "n":
        grouped = {}
        for (k, m, n), (real_t, theor_t) in results.items():
            grouped.setdefault((k, m), []).append((n, real_t, theor_t))
        for key, arr in grouped.items():
            arr.sort(key=lambda x: x[0])
            if len(arr) > 1:
                print(f"\nСравнение при фикс. (k,m)={key}, изменение n:")
                for i in range(len(arr) - 1):
                    n1, real_t1, theor_t1 = arr[i]
                    n2, real_t2, theor_t2 = arr[i + 1]
                    ratio_real = real_t2 / real_t1 if real_t1 else float('inf')
                    ratio_theor = theor_t2 / theor_t1 if theor_t1 else float('inf')
                    print(
                        f"n: {n1}->{n2}, ratio_real={ratio_real:.4f}, ratio_theor={ratio_theor:.4f}, ratio_real/ratio_theor={ratio_real / ratio_theor if ratio_theor else float('inf'):.4f}")

    if var_param == "m":
        grouped = {}
        for (k, m, n), (real_t, theor_t) in results.items():
            grouped.setdefault((k, n), []).append((m, real_t, theor_t))
        for key, arr in grouped.items():
            arr.sort(key=lambda x: x[0])
            if len(arr) > 1:
                print(f"\nСравнение при фикс. (k,n)={key}, изменение m:")
                for i in range(len(arr) - 1):
                    m1, real_t1, theor_t1 = arr[i]
                    m2, real_t2, theor_t2 = arr[i + 1]
                    ratio_real = real_t2 / real_t1 if real_t1 else float('inf')
                    ratio_theor = theor_t2 / theor_t1 if theor_t1 else float('inf')
                    print(
                        f"m: {m1}->{m2}, ratio_real={ratio_real:.4f}, ratio_theor={ratio_theor:.4f}, ratio_real/ratio_theor={ratio_real / ratio_theor if ratio_theor else float('inf'):.4f}")

    if var_param == "k":
        grouped = {}
        for (k, m, n), (real_t, theor_t) in results.items():
            grouped.setdefault((m, n), []).append((k, real_t, theor_t))
        for key, arr in grouped.items():
            arr.sort(key=lambda x: x[0])
            if len(arr) > 1:
                print(f"\nСравнение при фикс. (m,n)={key}, изменение k:")
                for i in range(len(arr) - 1):
                    k1, real_t1, theor_t1 = arr[i]
                    k2, real_t2, theor_t2 = arr[i + 1]
                    ratio_real = real_t2 / real_t1 if real_t1 else float('inf')
                    ratio_theor = theor_t2 / theor_t1 if theor_t1 else float('inf')
                    print(
                        f"k: {k1}->{k2}, ratio_real={ratio_real:.4f}, ratio_theor={ratio_theor:.4f}, ratio_real/ratio_theor={ratio_real / ratio_theor if ratio_theor else float('inf'):.4f}")


def run_scenario(scenario, k_values, m_values, n_values):
    """
    Запускаем эксперимент под конкретный сценарий и анализируем.
    """
    results = {}
    for k in k_values:
        for m in m_values:
            for n in n_values:
                real_t = measure_time(n, m, k, repeat=20)
                theor_t = theoretical_time(m, n, k, scenario)
                results[(k, m, n)] = (real_t, theor_t)

    print(f"\n\nСценарий: {scenario}")
    for (k, m, n), (real_t, theor_t) in results.items():
        ratio = real_t / theor_t if theor_t != 0 else float('inf')
        print(f"(k={k}, m={m}, n={n}): real={real_t:.6f}, theor={theor_t:.6f}, ratio={ratio:.6f}")

    # Анализируем зависимости для каждого сценария
    if scenario == "k_fixed":
        # O(mn)
        # Смотрим изменение n (при фикс m), и изменение m (при фикс n).
        if len(m_values) > 1:
            compare_ratios(results, scenario, var_param="m")
        if len(n_values) > 1:
            compare_ratios(results, scenario, var_param="n")

    elif scenario == "m_fixed":
        # O(n + k) ~ O(n)
        # Смотрим изменение n
        compare_ratios(results, scenario, var_param="n")

    elif scenario == "both_fixed":
        # O(n)
        # Смотрим изменение n
        compare_ratios(results, scenario, var_param="n")

    else:
        # general: O(m(n + k))
        # Смотрим изменение n, m, k
        if len(n_values) > 1:
            compare_ratios(results, scenario, var_param="n")
        if len(m_values) > 1:
            compare_ratios(results, scenario, var_param="m")
        if len(k_values) > 1:
            compare_ratios(results, scenario, var_param="k")


def run_general_no_fixed(k_values, m_values, n_values):
    # Полностью общий сценарий: берем множество точек без предположений о фиксации.
    points = []
    for k, m, n in itertools.product(k_values, m_values, n_values):
        real_t = measure_time(n, m, k, repeat=20)
        theor_t = m * (n + k)  # O(m(n+k))
        points.append((k, m, n, real_t, theor_t))

    print("\n=== General Scenario: No parameters fixed ===")
    for (k, m, n, real_t, theor_t) in points:
        ratio = real_t / theor_t if theor_t != 0 else float('inf')
        print(f"(k={k}, m={m}, n={n}): real={real_t:.6f}, theor={theor_t:.6f}, ratio={ratio:.6f}")

    # Сравним попарно некоторые точки
    print("\nСравнение пар точек (без фиксации параметров):")
    pairs = list(itertools.combinations(points, 2))
    pairs = pairs[:10]  # ограничимся 10 парами для компактности

    for p1, p2 in pairs:
        k1, m1, n1, real_t1, theor_t1 = p1
        k2, m2, n2, real_t2, theor_t2 = p2

        ratio_real = real_t2 / real_t1 if real_t1 != 0 else float('inf')
        ratio_theor = theor_t2 / theor_t1 if theor_t1 != 0 else float('inf')
        ratio_comp = ratio_real / ratio_theor if ratio_theor != 0 else float('inf')

        print(f"\nСравнение двух точек:")
        print(f" Точка1: (k={k1}, m={m1}, n={n1}), real={real_t1:.6f}, theor={theor_t1:.6f}")
        print(f" Точка2: (k={k2}, m={m2}, n={n2}), real={real_t2:.6f}, theor={theor_t2:.6f}")
        print(f"ratio_real={ratio_real:.4f}, ratio_theor={ratio_theor:.4f}, ratio_real/ratio_theor={ratio_comp:.4f}")


# Демонстрация всех сценариев:
# 1. k_fixed: k фиксировано => O(mn)
print("=== Сценарий k_fixed ===")
run_scenario("k_fixed", k_values=[3, 6], m_values=[2, 4], n_values=[1000, 2000, 4000])

# 2. m_fixed: m фиксировано (и k), => O(n + k) ~ O(n)
print("\n=== Сценарий m_fixed ===")
run_scenario("m_fixed", k_values=[3, 6], m_values=[2, 4], n_values=[1000, 2000, 4000])

# 3. both_fixed: m и k фиксированы, => O(n)
print("\n=== Сценарий both_fixed ===")
run_scenario("both_fixed", k_values=[3, 6], m_values=[2, 4], n_values=[1000, 2000, 4000])

# 4. general: все меняется => O(m(n+k))
print("\n=== Сценарий general ===")
run_scenario("general", k_values=[3, 6], m_values=[2, 4], n_values=[1000, 2000, 4000])

# Дополнительно - general без фиксации параметров,
# случайным образом сравниваем пары точек.
run_general_no_fixed(k_values=[2, 4, 8], m_values=[1, 2, 3], n_values=[1000, 2000, 4000])
