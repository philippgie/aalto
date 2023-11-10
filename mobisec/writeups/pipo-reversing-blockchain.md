# Solution

## Description of the problem

The `blockchain` challenge requires to reverse engineer an application to obtain a flag.

## Solution

I started by decompiling the application with *jadx*. First, I examined the seeming intersting `FlagChecker` class. The class takes a *key* String and a *flag* string. The key string get hashed, whereafter the first, middle, and last byte are hashed again. Now, the flag is encrypted, and the previous key is hashed, repeatedly for 10 times. Hence, we can interpret this as a 10 round encryption, where the round key is the hash of the previous round key.  Since the first hash is done by using only three bytes, the whole security of the algortihm is reduced to a 3 bytes long key. Thus, we can bruteforce it. I used the decompiled code and modified it to accomplish this

```java

    package com.mobisec.blockchain;

    import java.io.ByteArrayOutputStream;
    import java.security.Key;
    import java.security.MessageDigest;
    import javax.crypto.Cipher;
    import javax.crypto.CipherOutputStream;
    import javax.crypto.spec.SecretKeySpec;
    import java.util.HexFormat;
    
    /* loaded from: classes2.dex */
    class FlagChecker {
    
        FlagChecker() {
        }
        public static void main(String[] args) throws Exception {
            for(int k = 0; k < 0x1000000; k++) {
                byte[][] keys = new byte[10][16];
                byte[] ct = HexFormat.of().parseHex("0eef68c5ef95b67428c178f045e6fc8389b36a67bbbd800148f7c285f938a24e696ee2925e12ecf7c11f35a345a2a142639fe87ab2dd7530b29db87ca71ffda2af558131d7da615b6966fb0360d5823b79c26608772580bf14558e6b7500183ed7dfd41dbb5686ea92111667fd1eff9cec8dc29f0cfe01e092607da9f7c2602f5463a361ce5c83922cb6c3f5b872dcc088eb85df80503c92232bf03feed304d669ddd5ed1992a26674ecf2513ab25c20f95a5db49fdf6167fda3465a74e0418b2ea99eb2673d4c7e1ff7c4921c4e2d7b");
                byte[] key = {(byte) (k >> 16), (byte) ((k >> 8) & 0xff), (byte) k};
                keys[0] = hash(key);
    
                byte[] currPt = ct;
    
                for (int i = 1; i < 10; i++) {
                    keys[i] = hash(keys[i-1]);
                }
    
                for (int i = 0; i < 10; i++) {
                    currPt = decrypt(currPt, keys[9-i]);
                }
    
                String pt = new String(currPt);
                if(pt.startsWith("MOBISEC")) {
                    System.out.println(new String(currPt));
                }
            }
        }
    
        public static byte[] decrypt(byte[] in, byte[] key) throws Exception {
            Key aesKey = new SecretKeySpec(key, "AES");
            Cipher encryptCipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
            encryptCipher.init(2, aesKey);
            ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
            CipherOutputStream cipherOutputStream = new CipherOutputStream(outputStream, encryptCipher);
            cipherOutputStream.write(in);
            cipherOutputStream.flush();
            cipherOutputStream.close();
            byte[] out = outputStream.toByteArray();
            return out;
        }
    
        public static byte[] hash(byte[] in) throws Exception {
            MessageDigest md = MessageDigest.getInstance("MD5");
            md.update(in);
            return md.digest();
        }
    
        public static String toHex(byte[] bytes) {
            StringBuilder hexString = new StringBuilder();
            for (byte b : bytes) {
                String hex = Integer.toHexString(b & 255);
                if (hex.length() == 1) {
                    hexString.append('0');
                }
                hexString.append(hex);
            }
            return hexString.toString();
        }
    }

```

## Optional Feedback

N/A
