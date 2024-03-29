import hashlib
import itertools
import multiprocessing
from Crypto.Cipher import AES
from tqdm import tqdm
from Crypto.Util.Padding import pad, unpad




#iterates over all 6 digit combinations
def brute_force(i):
    THREADS = 1

    MAX_KEY = 0x8e09ae
    RANGE = MAX_KEY//THREADS
    
    for k in range(i*RANGE+RANGE-1, (i+1)*RANGE):
        ct = b"\x0e\xef\x68\xc5\xef\x95\xb6\x74\x28\xc1\x78\xf0\x45\xe6\xfc\x83\x89\xb3\x6a\x67\xbb\xbd\x80\x01\x48\xf7\xc2\x85\xf9\x38\xa2\x4e\x69\x6e\xe2\x92\x5e\x12\xec\xf7\xc1\x1f\x35\xa3\x45\xa2\xa1\x42\x63\x9f\xe8\x7a\xb2\xdd\x75\x30\xb2\x9d\xb8\x7c\xa7\x1f\xfd\xa2\xaf\x55\x81\x31\xd7\xda\x61\x5b\x69\x66\xfb\x03\x60\xd5\x82\x3b\x79\xc2\x66\x08\x77\x25\x80\xbf\x14\x55\x8e\x6b\x75\x00\x18\x3e\xd7\xdf\xd4\x1d\xbb\x56\x86\xea\x92\x11\x16\x67\xfd\x1e\xff\x9c\xec\x8d\xc2\x9f\x0c\xfe\x01\xe0\x92\x60\x7d\xa9\xf7\xc2\x60\x2f\x54\x63\xa3\x61\xce\x5c\x83\x92\x2c\xb6\xc3\xf5\xb8\x72\xdc\xc0\x88\xeb\x85\xdf\x80\x50\x3c\x92\x23\x2b\xf0\x3f\xee\xd3\x04\xd6\x69\xdd\xd5\xed\x19\x92\xa2\x66\x74\xec\xf2\x51\x3a\xb2\x5c\x20\xf9\x5a\x5d\xb4\x9f\xdf\x61\x67\xfd\xa3\x46\x5a\x74\xe0\x41\x8b\x2e\xa9\x9e\xb2\x67\x3d\x4c\x7e\x1f\xf7\xc4\x92\x1c\x4e\x2d\x7b"
        keys = [None] * 10
        keys[0] = k.to_bytes(3, 'big') 
        m = hashlib.md5()
        m.update(keys[0])
        keys[0] = m.digest()
        #print(key)
        #print((hex(key[2])))
        for i in range(1, 10):
            m = hashlib.md5()
            m.update(keys[i-1])
            keys[i] = m.digest()
    
        for i in range(10):
            cipher = AES.new(keys[9-i], AES.MODE_ECB)
            ct = cipher.decrypt(ct)
            ct = unpad(ct, 16, style='pkcs7')

        print(ct)
        ct = str(ct)
        if ct.startswith("MOBISEC"):
            print(ct)

if __name__ == '__main__':
    THREADS = 1
    
    pool = multiprocessing.Pool(THREADS)
    pool.map(brute_force, range(THREADS))
