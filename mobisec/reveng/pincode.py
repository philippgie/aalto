import hashlib
import itertools
import multiprocessing


#iterates over all 6 digit combinations
def brute_force(r : int):
    MAX_PIN = 1000000
    THREADS = 10
    offset = MAX_PIN//THREADS
    for x in range(r*offset, (r+1)*offset):
        #pin = ''.join(str(x) for x in combination).encode()
        pin = str(x).encode()
        for i in range(0, 25 * 400):
            m = hashlib.sha256()
            m.update(pin)
            pin = m.digest()
    
        if pin.hex() == "cdf62def583aa9877472834dcd555416ef9eb71380d6a2ee7fa66da1a6748931":
            print(x)
            break;

if __name__ == '__main__':
    THREADS = 10
    pool = multiprocessing.Pool(THREADS)
    out1, out2, out3 = zip(*pool.map(brute_force, range(THREADS)))
