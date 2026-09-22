import random, time
import numpy as np
N = 1_000_000
rng = random.Random(42)
letters = "abcdefghijklmnopqrstuvwxyz"
def random_word(): return "".join(rng.choice(letters) for _ in range(rng.randint(8, 15)))
def random_case(s): return "".join(c.upper() if rng.random() < 0.5 else c for c in s)
as_, bs = [], []
for _ in range(N):
    w = random_word(); as_.append(random_case(w))
    bs.append(random_case(w) if rng.random() < 0.5 else random_case(random_word()))

def bench(as_, bs):
    t = time.perf_counter()
    for i in range(N): as_[i].lower() == bs[i].lower()
    return (time.perf_counter() - t) * 1000

t = time.perf_counter(); A = np.array(as_); B = np.array(bs); conv = (time.perf_counter() - t) * 1000
t = time.perf_counter(); r1 = np.char.lower(A) == np.char.lower(B); c1 = (time.perf_counter() - t) * 1000
has_strings = hasattr(np, "strings")
if has_strings:
    t = time.perf_counter(); r2 = np.strings.lower(A) == np.strings.lower(B); c2 = (time.perf_counter() - t) * 1000
print("numpy", np.__version__, A.dtype)
print("boucle lower()          : %.2f ms" % bench(as_, bs))
print("conversion list->array  : %.2f ms" % conv)
print("np.char.lower ==        : %.2f ms" % c1)
if has_strings: print("np.strings.lower ==     : %.2f ms" % c2)
print("memoire A: %.1f Mo" % (A.nbytes/1e6))
