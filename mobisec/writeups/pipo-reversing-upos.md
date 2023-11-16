# Solution

## Description of the problem

The `upos` challenge requires to reverse engineer an application to obtain a flag.

## Solution

I started by decompiling the application with *jadx*. In the main activity I saw some print statements for detecting various analysis techniques, such as frida or a debugger. Upon executing the application in the emulator I was denied with an error message indicating that I modified the apk, even though I didn't(?). Through backtracing the variables indiciating whether an analysis technique is used, I found the `initActivity` method that checks them. I attempted to hook this method with frida, however, it wasn't sucessful since the method is executed before frida can work. I went back to static analysis. There is only one line which allows to return *true* from the `checkFlag` method. I attempted to understand when that would ocurr, which is the case if in the preceeding loop, the 30 values in the `fs` boolean array are true. This condition is satisfied by providing a flag input, that is presumably hashed or similar by a seemingly one-way function and compared with some values from a file called `lotto.dat`. I attempted to debug the application in order to find the values at the time of comparison. While some progress was made by patching out the intructions which for calling `initActivity`. However, there were further checks inside the checkFlag method. I attempted to remove those as well but this seemed to corrupt the apk somehow. Instead, I realized that due to the one way function this approach would not help anyway. I extraced the relvant code to brute the flag two bytes at a time. However, the app uses an lfsr, where it is unknown how many steps are executed. I did a frequency count for "step" in the FlagChecker class, where it was 20 times. Since I did not know how many times it is acutally executed I brute-forced for all possible steps between 0 and 20.


## Optional Feedback

N/A


# Solving script

```java

package com.mobisec.upos;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.support.p000v4.internal.view.SupportMenu;
import android.support.p000v4.view.MotionEventCompat;
import android.util.Base64;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Array;
import java.lang.reflect.Method;
import java.security.GeneralSecurityException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateEncodingException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.sql.BatchUpdateException;
import java.util.IllegalFormatCodePointException;
import java.util.IllformedLocaleException;
import java.util.Iterator;
import java.util.List;
import java.util.concurrent.RejectedExecutionException;

/* renamed from: com.mobisec.upos.FC */
/* loaded from: classes2.dex */
public class C0265FC {
    public static Context ctx = null;

    /* renamed from: m */
    public static long[][] f21m = (long[][]) Array.newInstance(long.class, 256, 256);

    /* JADX WARN: Can't wrap try/catch for region: R(10:(4:(15:35|36|37|38|39|(1:41)(1:332)|42|43|44|45|(1:(2:47|(2:50|51)(1:49))(1:310))|52|53|54|(7:56|57|58|59|60|61|(1:63)(9:64|65|(2:66|(2:68|(2:70|71))(1:265))|72|73|74|75|76|(1:78)(2:79|(2:223|224)(2:83|(1:85)(32:86|(1:88)(1:222)|89|90|91|92|93|94|95|96|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|(2:119|(1:176)(8:123|(8:126|127|(3:129|(1:131)|132)|133|134|(5:136|137|138|139|140)(2:161|162)|141|124)|163|164|(2:166|(2:169|170)(1:168))|171|172|(1:174)(1:175)))(2:178|179))))))(2:287|288))|53|54|(0)(0))|38|39|(0)(0)|42|43|44|45|(2:(0)(0)|49)|52) */
    /* JADX WARN: Can't wrap try/catch for region: R(4:(15:35|36|37|38|39|(1:41)(1:332)|42|43|44|45|(1:(2:47|(2:50|51)(1:49))(1:310))|52|53|54|(7:56|57|58|59|60|61|(1:63)(9:64|65|(2:66|(2:68|(2:70|71))(1:265))|72|73|74|75|76|(1:78)(2:79|(2:223|224)(2:83|(1:85)(32:86|(1:88)(1:222)|89|90|91|92|93|94|95|96|97|98|99|100|101|102|103|104|105|106|107|108|109|110|111|112|113|114|115|116|117|(2:119|(1:176)(8:123|(8:126|127|(3:129|(1:131)|132)|133|134|(5:136|137|138|139|140)(2:161|162)|141|124)|163|164|(2:166|(2:169|170)(1:168))|171|172|(1:174)(1:175)))(2:178|179))))))(2:287|288))|53|54|(0)(0)) */
    /* JADX WARN: Code restructure failed: missing block: B:111:0x024c, code lost:
        r9 = r2;
     */
    /* JADX WARN: Code restructure failed: missing block: B:119:0x025f, code lost:
        r3 = true;
     */
    /* JADX WARN: Code restructure failed: missing block: B:123:0x026b, code lost:
        r3 = true;
     */
    /* JADX WARN: Code restructure failed: missing block: B:398:?, code lost:
        return false;
     */
    /* JADX WARN: Code restructure failed: missing block: B:402:?, code lost:
        return false;
     */
    /* JADX WARN: Code restructure failed: missing block: B:403:?, code lost:
        return false;
     */
    /* JADX WARN: Code restructure failed: missing block: B:409:?, code lost:
        return false;
     */
    /* JADX WARN: Code restructure failed: missing block: B:421:?, code lost:
        return false;
     */
    /* JADX WARN: Removed duplicated region for block: B:106:0x0240 A[Catch: NullPointerException -> 0x0246, IllformedLocaleException -> 0x024b, IllegalFormatCodePointException -> 0x0250, Exception -> 0x04d6, GeneralSecurityException -> 0x04da, RejectedExecutionException -> 0x04de, CertificateEncodingException -> 0x04e3, BatchUpdateException -> 0x04e8, TRY_ENTER, TryCatch #60 {IllegalFormatCodePointException -> 0x0250, blocks: (B:44:0x013d, B:46:0x0145, B:50:0x015e, B:54:0x0173, B:106:0x0240, B:107:0x0245), top: B:360:0x013d }] */
    /* JADX WARN: Removed duplicated region for block: B:178:0x0321 A[RETURN] */
    /* JADX WARN: Removed duplicated region for block: B:180:0x0323 A[Catch: RejectedExecutionException -> 0x052c, CertificateEncodingException -> 0x0531, BatchUpdateException -> 0x0536, Exception -> 0x053b, GeneralSecurityException -> 0x0540, TryCatch #64 {GeneralSecurityException -> 0x0540, Exception -> 0x053b, blocks: (B:3:0x001e, B:4:0x0020, B:20:0x0061, B:31:0x0088, B:35:0x0099, B:180:0x0323, B:184:0x033d, B:240:0x0406, B:244:0x0438, B:245:0x043e, B:265:0x0499, B:269:0x049f, B:270:0x04a2, B:161:0x02bc, B:163:0x02d3, B:165:0x02d7, B:167:0x02dc, B:174:0x0315, B:300:0x04ed, B:301:0x04f2, B:173:0x0313, B:302:0x04f3, B:303:0x04f8), top: B:368:0x001e }] */
    /* JADX WARN: Removed duplicated region for block: B:231:0x03f1 A[Catch: Exception -> 0x04ba, GeneralSecurityException -> 0x04bf, RejectedExecutionException -> 0x04c4, CertificateEncodingException -> 0x04ca, BatchUpdateException -> 0x04d0, TryCatch #32 {CertificateEncodingException -> 0x04ca, GeneralSecurityException -> 0x04bf, BatchUpdateException -> 0x04d0, RejectedExecutionException -> 0x04c4, Exception -> 0x04ba, blocks: (B:229:0x03e1, B:231:0x03f1, B:233:0x03f6, B:278:0x04b4, B:279:0x04b9), top: B:377:0x03e1 }] */
    /* JADX WARN: Removed duplicated region for block: B:278:0x04b4 A[Catch: Exception -> 0x04ba, GeneralSecurityException -> 0x04bf, RejectedExecutionException -> 0x04c4, CertificateEncodingException -> 0x04ca, BatchUpdateException -> 0x04d0, TRY_ENTER, TryCatch #32 {CertificateEncodingException -> 0x04ca, GeneralSecurityException -> 0x04bf, BatchUpdateException -> 0x04d0, RejectedExecutionException -> 0x04c4, Exception -> 0x04ba, blocks: (B:229:0x03e1, B:231:0x03f1, B:233:0x03f6, B:278:0x04b4, B:279:0x04b9), top: B:377:0x03e1 }] */
    /* JADX WARN: Removed duplicated region for block: B:33:0x0096  */
    /* JADX WARN: Removed duplicated region for block: B:34:0x0098  */
    /* JADX WARN: Removed duplicated region for block: B:386:0x013b A[EDGE_INSN: B:386:0x013b->B:43:0x013b ?: BREAK  , SYNTHETIC] */
    /* JADX WARN: Removed duplicated region for block: B:39:0x00df A[Catch: Exception -> 0x0254, GeneralSecurityException -> 0x0259, RejectedExecutionException -> 0x025e, CertificateEncodingException -> 0x0264, BatchUpdateException -> 0x026a, NullPointerException -> 0x0270, IllformedLocaleException -> 0x0274, IllegalFormatCodePointException -> 0x0278, TRY_LEAVE, TryCatch #27 {NullPointerException -> 0x0270, CertificateEncodingException -> 0x0264, GeneralSecurityException -> 0x0259, BatchUpdateException -> 0x026a, IllegalFormatCodePointException -> 0x0278, IllformedLocaleException -> 0x0274, RejectedExecutionException -> 0x025e, Exception -> 0x0254, blocks: (B:37:0x00a0, B:39:0x00df), top: B:384:0x00a0 }] */
    /* JADX WARN: Removed duplicated region for block: B:46:0x0145 A[Catch: NullPointerException -> 0x0246, IllformedLocaleException -> 0x024b, IllegalFormatCodePointException -> 0x0250, Exception -> 0x04d6, GeneralSecurityException -> 0x04da, RejectedExecutionException -> 0x04de, CertificateEncodingException -> 0x04e3, BatchUpdateException -> 0x04e8, TRY_LEAVE, TryCatch #60 {IllegalFormatCodePointException -> 0x0250, blocks: (B:44:0x013d, B:46:0x0145, B:50:0x015e, B:54:0x0173, B:106:0x0240, B:107:0x0245), top: B:360:0x013d }] */
    /*
        Code decompiled incorrectly, please refer to instructions dump.
    */
    public static boolean checkFlag(Context ctx2, String fl) {
        boolean z;
        boolean z2;
        int idx;
        int idx2;
        CertificateEncodingException e;
        NoSuchAlgorithmException e1;
        String hexString;
        int idx3;
        int idx4;
        boolean z3;
        int length;
        int i;
        int idx5;
        int idx6;
        MainActivity mainActivity = (MainActivity) ctx2;
        ctx = ctx2;
        boolean[] fs = new boolean[200];
        Streamer s = new Streamer();
        try {
            try {
                try {
                    m4lm(f21m);
                    if (fl.length() != 69) {
                        return false;
                    }
                    int idx7 = 0 + 1;
                    try {
                        fs[0] = fl.startsWith("MOBISEC{");
                        String core = fl.substring(8);
                        int idx8 = idx7 + 1;
                        try {
                            fs[idx7] = core.endsWith("}");
                            boolean f = true;
                            try {
                                s.step();
                            } catch (NullPointerException e2) {
                                return false;
                            } catch (IllformedLocaleException e3) {
                                idx = idx8;
                            }
                            if (MainActivity.f23g2) {
                                return false;
                            }
                            s.step();
                            s.step();
                            int idx9 = idx8 + 1;
                            try {
                                fs[idx8] = core.startsWith("this_is_");
                                idx4 = idx9 + 1;
                                try {
                                    fs[idx9] = core.endsWith("upos");
                                    idx8 = idx4 + 1;
                                } catch (NullPointerException e4) {
                                    return false;
                                } catch (IllformedLocaleException e5) {
                                    idx = idx4;
                                }
                            } catch (NullPointerException e6) {
                                return false;
                            } catch (CertificateEncodingException e7) {
                                return false;
                            } catch (GeneralSecurityException e8) {
                            } catch (BatchUpdateException e9) {
                                z2 = true;
                            } catch (IllformedLocaleException e10) {
                                idx = idx9;
                            } catch (RejectedExecutionException e11) {
                                z = true;
                            } catch (Exception e12) {
                                return false;
                            }
                            try {
                                try {
                                    try {
                                        try {
                                            try {
                                                if (core.charAt(10) != 'c' && core.charAt(13) != 'q') {
                                                    z3 = false;
                                                    fs[idx4] = z3;
                                                    idx4 = idx8 + 1;
                                                    fs[idx8] = core.charAt(3) + core.charAt(7) != 114;
                                                    s.step();
                                                    idx = idx4 + 1;
                                                    fs[idx4] = core.contains("really_");
                                                    boolean found = false;
                                                    String[] lines = m7ec(dec(ctx2.getString(C0268R.string.s1)) + " " + dec(ctx2.getString(C0268R.string.s2))).split("\n");
                                                    length = lines.length;
                                                    i = 0;
                                                    while (true) {
                                                        if (i < length) {
                                                            break;
                                                        }
                                                        String line = lines[i];
                                                        String maps = dec(ctx2.getString(C0268R.string.s3)) + " " + dec(ctx2.getString(C0268R.string.s2)) + "/" + line + "/maps";
                                                        String out = m7ec(maps);
                                                        if (out.contains(dec(ctx2.getString(C0268R.string.s4)))) {
                                                            found = true;
                                                            break;
                                                        }
                                                        i++;
                                                    }
                                                    idx5 = idx + 1;
                                                    fs[idx] = found;
                                                    if (!fs[idx5 - 1]) {
                                                        throw new BatchUpdateException();
                                                    }
                                                    s.step();
                                                    int idx10 = idx5 + 1;
                                                    try {
                                                        fs[idx5] = core.substring(14).endsWith("_evil");
                                                        idx6 = idx10 + 1;
                                                        fs[idx10] = core.substring(9, 13).endsWith("bam");
                                                        s.step();
                                                    } catch (NullPointerException e13) {
                                                        return false;
                                                    } catch (CertificateEncodingException e14) {
                                                        return false;
                                                    } catch (GeneralSecurityException e15) {
                                                    } catch (BatchUpdateException e16) {
                                                        z2 = true;
                                                    } catch (IllegalFormatCodePointException e17) {
                                                        return false;
                                                    } catch (IllformedLocaleException e18) {
                                                        idx = idx10;
                                                    } catch (RejectedExecutionException e19) {
                                                        z = true;
                                                    } catch (Exception e20) {
                                                        return false;
                                                    }
                                                    if (MainActivity.f25g4) {
                                                        return false;
                                                    }
                                                    s.step();
                                                    List<ApplicationInfo> packages = ctx2.getPackageManager().getInstalledApplications(128);
                                                    boolean found2 = false;
                                                    Iterator<ApplicationInfo> it = packages.iterator();
                                                    while (true) {
                                                        if (!it.hasNext()) {
                                                            break;
                                                        }
                                                        ApplicationInfo info = it.next();
                                                        if (info.packageName.equals(dec(ctx2.getString(C0268R.string.s5)))) {
                                                            found2 = true;
                                                            break;
                                                        }
                                                    }
                                                    int idx11 = idx6 + 1;
                                                    try {
                                                        fs[idx6] = found2;
                                                        s.step();
                                                        idx2 = idx11 + 1;
                                                        fs[idx11] = core.substring(4, 10).toLowerCase().equals("incred");
                                                    } catch (NullPointerException e21) {
                                                        return false;
                                                    } catch (CertificateEncodingException e22) {
                                                        return false;
                                                    } catch (GeneralSecurityException e23) {
                                                        MainActivity.f25g4 = true;
                                                        return false;
                                                    } catch (BatchUpdateException e24) {
                                                        z2 = true;
                                                        MainActivity.f23g2 = z2;
                                                        return false;
                                                    } catch (IllformedLocaleException e25) {
                                                        idx = idx11;
                                                        int idx12 = idx + 1;
                                                        fs[idx] = core.substring(22).toUpperCase().startsWith("mayb");
                                                        s.step();
                                                        if (!fs[idx12 - 3]) {
                                                            throw new RejectedExecutionException();
                                                        }
                                                        if (MainActivity.f24g3) {
                                                            s.step();
                                                            return false;
                                                        }
                                                        s.step();
                                                        try {
                                                            Class c = Class.forName(dec(ctx2.getString(C0268R.string.s7)));
                                                            Method m = c.getMethod(dec(ctx2.getString(C0268R.string.s8)), new Class[0]);
                                                            boolean res = ((Boolean) m.invoke(null, new Object[0])).booleanValue();
                                                            fs[idx12] = res;
                                                        } catch (Exception e26) {
                                                            fs[idx12] = false;
                                                        }
                                                        int idx13 = idx12 + 1;
                                                        if (fs[idx13 - 1]) {
                                                            throw new CertificateEncodingException();
                                                        }
                                                        f = false;
                                                        idx2 = idx13;
                                                        if (!f) {
                                                        }
                                                    } catch (RejectedExecutionException e27) {
                                                        z = true;
                                                        MainActivity.f24g3 = z;
                                                        return false;
                                                    } catch (Exception e28) {
                                                        return false;
                                                    }
                                                    if (MainActivity.f22g1) {
                                                        return false;
                                                    }
                                                    s.step();
                                                    if (s.step() > 0 || !MainActivity.f22g1) {
                                                        throw new IllformedLocaleException();
                                                    }
                                                    s.step();
                                                    s.step();
                                                    s.step();
                                                    s.step();
                                                    s.step();
                                                    if (!f) {
                                                        return false;
                                                    }
                                                    int idx14 = idx2 + 1;
                                                    fs[idx2] = core.toLowerCase().substring(11, 14).charAt(1) == '4';
                                                    int idx15 = idx14 + 1;
                                                    fs[idx14] = core.substring(22).toUpperCase().startsWith("mayb");
                                                    PackageManager pm = ctx2.getPackageManager();
                                                    String packageName = ctx2.getPackageName();
                                                    PackageInfo packageInfo = null;
                                                    try {
                                                        packageInfo = pm.getPackageInfo(packageName, 64);
                                                    } catch (PackageManager.NameNotFoundException e29) {
                                                        e29.printStackTrace();
                                                    }
                                                    Signature[] signatures = packageInfo.signatures;
                                                    byte[] cert2 = signatures[0].toByteArray();
                                                    InputStream input = new ByteArrayInputStream(cert2);
                                                    CertificateFactory cf = null;
                                                    try {
                                                        cf = CertificateFactory.getInstance("X509");
                                                    } catch (CertificateException e30) {
                                                        e30.printStackTrace();
                                                    }
                                                    X509Certificate c2 = null;
                                                    try {
                                                        c2 = (X509Certificate) cf.generateCertificate(input);
                                                    } catch (CertificateException e31) {
                                                        e31.printStackTrace();
                                                    }
                                                    try {
                                                        MessageDigest md = MessageDigest.getInstance("SHA1");
                                                        try {
                                                        } catch (NoSuchAlgorithmException e32) {
                                                            e1 = e32;
                                                        } catch (CertificateEncodingException e33) {
                                                            e = e33;
                                                        }
                                                        try {
                                                            byte[] publicKey = md.digest(c2.getEncoded());
                                                            String hexString2 = m1th(publicKey);
                                                            hexString = hexString2;
                                                        } catch (NoSuchAlgorithmException e34) {
                                                            e1 = e34;
                                                            e1.printStackTrace();
                                                            hexString = null;
                                                            idx3 = idx15 + 1;
                                                            fs[idx15] = hexString.equals(ctx2.getString(C0268R.string.s6));
                                                            if (fs[idx3 - 1]) {
                                                            }
                                                        } catch (CertificateEncodingException e35) {
                                                            e = e35;
                                                            e.printStackTrace();
                                                            hexString = null;
                                                            idx3 = idx15 + 1;
                                                            fs[idx15] = hexString.equals(ctx2.getString(C0268R.string.s6));
                                                            if (fs[idx3 - 1]) {
                                                            }
                                                        }
                                                    } catch (NoSuchAlgorithmException e36) {
                                                        e1 = e36;
                                                    } catch (CertificateEncodingException e37) {
                                                        e = e37;
                                                    }
                                                    idx3 = idx15 + 1;
                                                    try {
                                                        fs[idx15] = hexString.equals(ctx2.getString(C0268R.string.s6));
                                                        if (fs[idx3 - 1]) {
                                                            if (fs[0] && fs[1]) {
                                                                int idx16 = 100;
                                                                for (int i2 = 0; i2 < 30; i2++) {
                                                                    fs[idx16] = true;
                                                                    String curr = Character.toString(core.charAt(i2 * 2)) + Character.toString(core.charAt((i2 * 2) + 1));
                                                                    if (m5ip(i2)) {
                                                                        for (int j = 0; j < i2; j++) {
                                                                            s.step();
                                                                        }
                                                                    }
                                                                    int j2 = s.m0g2();
                                                                    int mX = j2 & 255;
                                                                    int mY = (s.m0g2() & MotionEventCompat.ACTION_POINTER_INDEX_MASK) >> 8;
                                                                    if (m2sq(m3r(curr)) != f21m[mX][mY]) {
                                                                        int idx17 = idx16 + 1;
                                                                        try {
                                                                            fs[idx16] = false;
                                                                            idx16 = idx17;
                                                                        } catch (CertificateEncodingException e38) {
                                                                            return false;
                                                                        } catch (GeneralSecurityException e39) {
                                                                            MainActivity.f25g4 = true;
                                                                            return false;
                                                                        } catch (BatchUpdateException e40) {
                                                                            z2 = true;
                                                                            MainActivity.f23g2 = z2;
                                                                            return false;
                                                                        } catch (RejectedExecutionException e41) {
                                                                            z = true;
                                                                            MainActivity.f24g3 = z;
                                                                            return false;
                                                                        } catch (Exception e42) {
                                                                            return false;
                                                                        }
                                                                    } else {
                                                                        idx16++;
                                                                    }
                                                                }
                                                                for (int i3 = idx16 - 30; i3 < idx16; i3++) {
                                                                    if (!fs[i3]) {
                                                                        return false;
                                                                    }
                                                                }
                                                                return m6h(fl).equals("4193d9b72a5c4805e9a5cc739f8a8fc23b2890e13b83bb887d96f86c30654a12");
                                                            }
                                                            return false;
                                                        }
                                                        throw new GeneralSecurityException();
                                                    } catch (CertificateEncodingException e43) {
                                                        return false;
                                                    } catch (GeneralSecurityException e44) {
                                                    } catch (BatchUpdateException e45) {
                                                        z2 = true;
                                                    } catch (RejectedExecutionException e46) {
                                                        z = true;
                                                    } catch (Exception e47) {
                                                        return false;
                                                    }
                                                }
                                                fs[idx] = found;
                                                if (!fs[idx5 - 1]) {
                                                }
                                            } catch (GeneralSecurityException e48) {
                                            } catch (BatchUpdateException e49) {
                                                z2 = true;
                                            } catch (RejectedExecutionException e50) {
                                                z = true;
                                            } catch (Exception e51) {
                                                return false;
                                            }
                                        } catch (CertificateEncodingException e52) {
                                            return false;
                                        }
                                    } catch (IllegalFormatCodePointException e53) {
                                        return false;
                                    }
                                    fs[idx8] = core.charAt(3) + core.charAt(7) != 114;
                                    s.step();
                                    idx = idx4 + 1;
                                    fs[idx4] = core.contains("really_");
                                    boolean found3 = false;
                                    String[] lines2 = m7ec(dec(ctx2.getString(C0268R.string.s1)) + " " + dec(ctx2.getString(C0268R.string.s2))).split("\n");
                                    length = lines2.length;
                                    i = 0;
                                    while (true) {
                                        if (i < length) {
                                        }
                                        i++;
                                    }
                                    idx5 = idx + 1;
                                } catch (IllegalFormatCodePointException e54) {
                                    return false;
                                }
                                z3 = true;
                                fs[idx4] = z3;
                                idx4 = idx8 + 1;
                            } catch (IllegalFormatCodePointException e55) {
                                return false;
                            }
                        } catch (CertificateEncodingException e56) {
                            return false;
                        } catch (GeneralSecurityException e57) {
                        } catch (BatchUpdateException e58) {
                            z2 = true;
                        } catch (RejectedExecutionException e59) {
                            z = true;
                        } catch (Exception e60) {
                            return false;
                        }
                    } catch (CertificateEncodingException e61) {
                        return false;
                    } catch (GeneralSecurityException e62) {
                    } catch (BatchUpdateException e63) {
                        z2 = true;
                    } catch (RejectedExecutionException e64) {
                        z = true;
                    } catch (Exception e65) {
                        return false;
                    }
                } catch (CertificateEncodingException e66) {
                    return false;
                } catch (BatchUpdateException e67) {
                    z2 = true;
                } catch (RejectedExecutionException e68) {
                    z = true;
                }
            } catch (CertificateEncodingException e69) {
                return false;
            } catch (BatchUpdateException e70) {
                z2 = true;
            } catch (RejectedExecutionException e71) {
                z = true;
            }
        } catch (GeneralSecurityException e72) {
        } catch (Exception e73) {
            return false;
        }
    }

    /* renamed from: h */
    private static String m6h(String flag) {
        try {
            MessageDigest md = MessageDigest.getInstance("SHA-256");
            md.update(flag.getBytes());
            byte[] digest = md.digest();
            return m1th(digest);
        } catch (Exception e) {
            return null;
        }
    }

    /* renamed from: th */
    public static String m1th(byte[] bytes) {
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

    /* renamed from: lm */
    private static void m4lm(long[][] matrix) throws Exception {
        BufferedReader reader = null;
        try {
            BufferedReader reader2 = new BufferedReader(new InputStreamReader(ctx.getAssets().open("lotto.dat")));
            int rowIdx = 0;
            while (true) {
                String row = reader2.readLine();
                if (row != null) {
                    String[] elems = row.split(" ");
                    int colIdx = 0;
                    for (String elem : elems) {
                        long e = Long.parseLong(elem);
                        matrix[rowIdx][colIdx] = e;
                        colIdx++;
                    }
                    if (colIdx != 256) {
                        throw new Exception("error");
                    }
                    rowIdx++;
                } else if (rowIdx != 256) {
                    throw new Exception("error");
                } else {
                    reader2.close();
                    return;
                }
            }
        } catch (Throwable th) {
            if (0 != 0) {
                reader.close();
            }
            throw th;
        }
    }

    /* renamed from: ip */
    public static boolean m5ip(int x) {
        for (int i = 2; i < x; i++) {
            if (x % i == 0) {
                return false;
            }
        }
        return true;
    }

    /* renamed from: r */
    public static String m3r(String s) {
        String out = BuildConfig.FLAVOR;
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            if (c >= 'a' && c <= 's') {
                c = (char) (c + 7);
            } else if (c >= 'A' && c <= 'S') {
                c = (char) (c + 7);
            } else if (c >= 't' && c <= 'z') {
                c = (char) (c - 19);
            } else if (c >= 'T' && c <= 'Z') {
                c = (char) (c - 19);
            }
            out = out + c;
        }
        return out;
    }

    /* renamed from: sq */
    public static long m2sq(String a) {
        int n = (a.charAt(0) + (a.charAt(1) << '\b')) & SupportMenu.USER_MASK;
        long n2 = (long) Math.pow(n, 2.0d);
        return n2;
    }

    public static String dec(String x) {
        return new String(Base64.decode(x, 0));
    }

    /* renamed from: ec */
    public static String m7ec(String cmd) {
        String out = new String();
        try {
            Process p = Runtime.getRuntime().exec(cmd);
            InputStream stdout = p.getInputStream();
            byte[] buffer = new byte[102400];
            while (true) {
                int read = stdout.read(buffer);
                if (read > 0 && read <= 102400) {
                    String line = new String(buffer, 0, read);
                    out = out + line;
                } else if (stdout.available() <= 0) {
                    break;
                }
            }
            stdout.close();
        } catch (Exception e) {
        }
        return out;
    }
}

```

