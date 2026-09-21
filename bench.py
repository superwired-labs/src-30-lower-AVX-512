import random
import time

N = 1_000_000
rng = random.Random(42)
letters = "abcdefghijklmnopqrstuvwxyz"


def random_word():
    return "".join(rng.choice(letters) for _ in range(rng.randint(8, 15)))


def random_case(s):
    return "".join(c.upper() if rng.random() < 0.5 else c for c in s)


as_, bs = [], []
for _ in range(N):
    w = random_word()
    as_.append(random_case(w))
    if rng.random() < 0.5:
        bs.append(random_case(w))  # meme mot, autre casse
    else:
        bs.append(random_case(random_word()))



def bench(as_, bs):
    start = time.perf_counter()
    for i in range(N):
        as_[i].lower() == bs[i].lower()
    return (time.perf_counter() - start) * 1000


ms = bench(as_, bs)

print("Python :")
print("a.lower() == b.lower() :", f"{ms:.2f}".replace(".", ","), "ms")
