# Solution

## Description of the problem

The `gnirts` challenge requires to reverse engineer an application to obtain a flag.

## Solution

I started by decompiling the application with *jadx*. First, I examined the seeming intersting `FlagChecker` class. The code is obfuscated, hence, I started replacing calls, such as `getString()` or `m2gs() ` by their concrete values. I iteratively applied this approach until the code became a lot more readable.

With the deobfuscated code, I can see that the different parts are hashed and compared to hashes. Using additional information that restricts the search space I am able to write the following script to brute force the parts themselves and such acquire the flag:

```python

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
    
    p0 = check_part(string.ascii_lowercase, "6e9a4d130a9b316e9201238844dd5124", 5)
    p1 = check_part(string.digits, "7c51a5e6ea3214af970a86df89793b19", 7)
    p2 = check_part(string.ascii_uppercase, "e5f20324ae520a11a86c7602e29ecbb8", 5)
    p3 = check_part(string.printable, "1885eca5a40bc32d5e1bca61fcd308a5", 4)
    p4 = check_part(string.digits, "da5062d64347e5e020c5419cebd149a2", 7)
    
    core = f"{p0}-{p1}-{p2}-{p3}-{p4}"
    
    print(core)

```

### Deobfuscated code 

```java

package com.mobisec.gnirts;

import android.content.Context;
import android.util.Base64;
import android.util.Log;
import java.lang.reflect.Method;
import java.security.MessageDigest;
import java.util.HashSet;
import java.util.Set;

/* loaded from: classes2.dex */
class FlagChecker {
    FlagChecker() {
    }

    public static boolean checkFlag(Context ctx, String flag) {
        if (flag.startsWith("MOBISEC{") && flag.endsWith("}")) {
            String core = flag.substring(8, 40);
            if (core.length() != 32) {
                return false;
            }
            String[] ps = core.split("-");
            if (ps.length == 5 && bim(ps[0]) && bum(ps[2]) && bam(ps[4])) {
                String reduced = core.replaceAll("[A-Z]", "X").replaceAll("[a-z]", "x").replaceAll("[0-9]", " ");
                if (reduced.matches("[A-Za-z0-9]+.       .[A-Za-z0-9]+.[Xx ]+.[A-Za-z0-9 ]+")) {
                    char[] syms = new char[4];
                    int[] idxs = {13, 21, 27, 32};
                    Set<Character> chars = new HashSet<>();
                    for (int i = 0; i < syms.length; i++) {
                        syms[i] = flag.charAt(idxs[i]);
                        chars.add(Character.valueOf(syms[i]));
                    }
                    int sum = 0;
                    for (char c : syms) {
                        sum += c;
                    }
                    return sum == 180 && chars.size() == 1 && m1me(ctx, m3dh("MD5", ps[0]), ctx.getString(C0267R.string.t1)) && m1me(ctx, m3dh("MD5", ps[1]), ctx.getString(C0267R.string.t2)) && m1me(ctx, m3dh("MD5", ps[2]), ctx.getString(C0267R.string.t3)) && m1me(ctx, m3dh("MD5", ps[3]), ctx.getString(C0267R.string.t4)) && m1me(ctx, m3dh("MD5", ps[4]), ctx.getString(C0267R.string.t5)) && m1me(ctx, m3dh("SHA-256", flag), ctx.getString(C0267R.string.t6));
                }
                return false;
            }
            return false;
        }
        return false;
    }

    private static boolean bim(String s) {
        return s.matches("^[a-z]+$");
    }

    private static boolean bum(String s) {
        return s.matches("^[A-Z]+$");
    }

    private static boolean bam(String s) {
        return s.matches("^[0-9]+$");
    }

    /* renamed from: dh */
    private static String m3dh(String hash, String s) {
        try {
            MessageDigest md = MessageDigest.getInstance(hash);
            md.update(s.getBytes());
            byte[] digest = md.digest();
            return toHexString(digest);
        } catch (Exception e) {
            return null;
        }
    }

    private static String toHexString(byte[] bytes) {
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


    /* renamed from: gs */
    private static String m2gs(String a, String b) {
        String s = BuildConfig.FLAVOR;
        for (int i = 0; i < a.length(); i++) {
            s = s + Character.toString((char) (a.charAt(i) ^ b.charAt(i % b.length())));
        }
        return s;
    }

    /* renamed from: me */
    private static boolean m1me(Context ctx, String s1, String s2) {
        try {
            Class c = s1.getClass();
            Method m = c.getMethod(m0r("slauqe"), Object.class);
            boolean res = ((Boolean) m.invoke(s1, s2)).booleanValue();
            return res;
        } catch (Exception e) {
            Log.e("MOBISEC", "Exception: " + Log.getStackTraceString(e));
            return false;
        }
    }

    /* renamed from: r */
    public static String m0r(String s) {
        return new StringBuffer(s).reverse().toString();
    }
}

```

## Optional Feedback

N/A
