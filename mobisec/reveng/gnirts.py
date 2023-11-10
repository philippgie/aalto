import hashlib
import string
import itertools

def check_part(alphabet, hash_val, length):
    for combination in itertools.product(alphabet, repeat=length):
        candidate = "".join(combination).encode()
        m = hashlib.md5()
        m.update(candidate)
        d = m.digest().hex()
        if d == hash_val:
            print(candidate)
            return candidate
    print(f"failed: {hash_val}")

def check_flag(i, flag):
    if i == len(flag)-1:
        m = hashlib.sha256()
        m.update(f"MOBISEC{{{flag}}}".encode())
        d = m.digest().hex()
        if d == "1c4d1410a4071880411f02ff46370e46b464ab2f87e8a487a09e13040d64e396":
            print(flag)
        return
    if(flag[i].isdigit() or flag[i] == '-'):
        check_flag(i + 1, flag)
    else:
        check_flag(i + 1, flag)
        s = list(flag)
        s[i] = chr(ord(s[i])+32)
        check_flag(i + 1, "".join(s))
    

p0 = check_part(string.ascii_lowercase, "6e9a4d130a9b316e9201238844dd5124", 5)
p1 = check_part(string.digits, "7c51a5e6ea3214af970a86df89793b19", 7)
p2 = check_part(string.ascii_uppercase, "e5f20324ae520a11a86c7602e29ecbb8", 5)
p3 = check_part(string.printable, "1885eca5a40bc32d5e1bca61fcd308a5", 4)
p4 = check_part(string.digits, "da5062d64347e5e020c5419cebd149a2", 7)

core = f"{p0}-{p1}-{p2}-{p3}-{p4}"

check_flag(0, core)




