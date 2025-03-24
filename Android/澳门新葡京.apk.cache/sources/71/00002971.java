package x4;

import android.os.Bundle;
import android.os.IBinder;
import android.os.Parcel;
import android.os.Parcelable;
import androidx.annotation.NonNull;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.util.ArrayList;

/* compiled from: com.google.android.gms:play-services-basement@@18.1.0 */
/* loaded from: classes.dex */
public class a {

    /* compiled from: com.google.android.gms:play-services-basement@@18.1.0 */
    /* renamed from: x4.a$a  reason: collision with other inner class name */
    /* loaded from: classes.dex */
    public static class C0300a extends RuntimeException {
        /* JADX WARN: Illegal instructions before constructor call */
        /*
            Code decompiled incorrectly, please refer to instructions dump.
            To view partially-correct code enable 'Show inconsistent code' option in preferences
        */
        public C0300a(@androidx.annotation.NonNull java.lang.String r3, @androidx.annotation.NonNull android.os.Parcel r4) {
            /*
                r2 = this;
                int r0 = r4.dataPosition()
                int r4 = r4.dataSize()
                java.lang.StringBuilder r1 = new java.lang.StringBuilder
                r1.<init>()
                r1.append(r3)
                java.lang.String r3 = " Parcel: pos="
                r1.append(r3)
                r1.append(r0)
                java.lang.String r3 = " size="
                r1.append(r3)
                r1.append(r4)
                java.lang.String r3 = r1.toString()
                r2.<init>(r3)
                return
            */
            throw new UnsupportedOperationException("Method not decompiled: x4.a.C0300a.<init>(java.lang.String, android.os.Parcel):void");
        }
    }

    @NonNull
    public static Float A(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        if (G == 0) {
            return null;
        }
        J(parcel, i10, G, 4);
        return Float.valueOf(parcel.readFloat());
    }

    public static int B(@NonNull Parcel parcel) {
        return parcel.readInt();
    }

    @NonNull
    public static IBinder C(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        IBinder readStrongBinder = parcel.readStrongBinder();
        parcel.setDataPosition(dataPosition + G);
        return readStrongBinder;
    }

    public static int D(@NonNull Parcel parcel, int i10) {
        K(parcel, i10, 4);
        return parcel.readInt();
    }

    public static long E(@NonNull Parcel parcel, int i10) {
        K(parcel, i10, 8);
        return parcel.readLong();
    }

    @NonNull
    public static Long F(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        if (G == 0) {
            return null;
        }
        J(parcel, i10, G, 8);
        return Long.valueOf(parcel.readLong());
    }

    public static int G(@NonNull Parcel parcel, int i10) {
        return (i10 & (-65536)) != -65536 ? (char) (i10 >> 16) : parcel.readInt();
    }

    public static void H(@NonNull Parcel parcel, int i10) {
        parcel.setDataPosition(parcel.dataPosition() + G(parcel, i10));
    }

    public static int I(@NonNull Parcel parcel) {
        int B = B(parcel);
        int G = G(parcel, B);
        int dataPosition = parcel.dataPosition();
        if (u(B) == 20293) {
            int i10 = G + dataPosition;
            if (i10 < dataPosition || i10 > parcel.dataSize()) {
                throw new C0300a("Size read is invalid start=" + dataPosition + " end=" + i10, parcel);
            }
            return i10;
        }
        throw new C0300a("Expected object header. Got 0x".concat(String.valueOf(Integer.toHexString(B))), parcel);
    }

    private static void J(Parcel parcel, int i10, int i11, int i12) {
        if (i11 == i12) {
            return;
        }
        String hexString = Integer.toHexString(i11);
        throw new C0300a("Expected size " + i12 + " got " + i11 + " (0x" + hexString + ")", parcel);
    }

    private static void K(Parcel parcel, int i10, int i11) {
        int G = G(parcel, i10);
        if (G == i11) {
            return;
        }
        String hexString = Integer.toHexString(G);
        throw new C0300a("Expected size " + i11 + " got " + G + " (0x" + hexString + ")", parcel);
    }

    @NonNull
    public static BigDecimal a(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        byte[] createByteArray = parcel.createByteArray();
        int readInt = parcel.readInt();
        parcel.setDataPosition(dataPosition + G);
        return new BigDecimal(new BigInteger(createByteArray), readInt);
    }

    @NonNull
    public static BigDecimal[] b(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        int readInt = parcel.readInt();
        BigDecimal[] bigDecimalArr = new BigDecimal[readInt];
        for (int i11 = 0; i11 < readInt; i11++) {
            byte[] createByteArray = parcel.createByteArray();
            bigDecimalArr[i11] = new BigDecimal(new BigInteger(createByteArray), parcel.readInt());
        }
        parcel.setDataPosition(dataPosition + G);
        return bigDecimalArr;
    }

    @NonNull
    public static BigInteger c(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        byte[] createByteArray = parcel.createByteArray();
        parcel.setDataPosition(dataPosition + G);
        return new BigInteger(createByteArray);
    }

    @NonNull
    public static BigInteger[] d(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        int readInt = parcel.readInt();
        BigInteger[] bigIntegerArr = new BigInteger[readInt];
        for (int i11 = 0; i11 < readInt; i11++) {
            bigIntegerArr[i11] = new BigInteger(parcel.createByteArray());
        }
        parcel.setDataPosition(dataPosition + G);
        return bigIntegerArr;
    }

    @NonNull
    public static boolean[] e(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        boolean[] createBooleanArray = parcel.createBooleanArray();
        parcel.setDataPosition(dataPosition + G);
        return createBooleanArray;
    }

    @NonNull
    public static Bundle f(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        Bundle readBundle = parcel.readBundle();
        parcel.setDataPosition(dataPosition + G);
        return readBundle;
    }

    @NonNull
    public static byte[] g(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        byte[] createByteArray = parcel.createByteArray();
        parcel.setDataPosition(dataPosition + G);
        return createByteArray;
    }

    @NonNull
    public static double[] h(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        double[] createDoubleArray = parcel.createDoubleArray();
        parcel.setDataPosition(dataPosition + G);
        return createDoubleArray;
    }

    @NonNull
    public static float[] i(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        float[] createFloatArray = parcel.createFloatArray();
        parcel.setDataPosition(dataPosition + G);
        return createFloatArray;
    }

    @NonNull
    public static int[] j(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        int[] createIntArray = parcel.createIntArray();
        parcel.setDataPosition(dataPosition + G);
        return createIntArray;
    }

    @NonNull
    public static long[] k(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        long[] createLongArray = parcel.createLongArray();
        parcel.setDataPosition(dataPosition + G);
        return createLongArray;
    }

    @NonNull
    public static Parcel l(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        Parcel obtain = Parcel.obtain();
        obtain.appendFrom(parcel, dataPosition, G);
        parcel.setDataPosition(dataPosition + G);
        return obtain;
    }

    @NonNull
    public static Parcel[] m(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        int readInt = parcel.readInt();
        Parcel[] parcelArr = new Parcel[readInt];
        for (int i11 = 0; i11 < readInt; i11++) {
            int readInt2 = parcel.readInt();
            if (readInt2 != 0) {
                int dataPosition2 = parcel.dataPosition();
                Parcel obtain = Parcel.obtain();
                obtain.appendFrom(parcel, dataPosition2, readInt2);
                parcelArr[i11] = obtain;
                parcel.setDataPosition(dataPosition2 + readInt2);
            } else {
                parcelArr[i11] = null;
            }
        }
        parcel.setDataPosition(dataPosition + G);
        return parcelArr;
    }

    @NonNull
    public static <T extends Parcelable> T n(@NonNull Parcel parcel, int i10, @NonNull Parcelable.Creator<T> creator) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        T createFromParcel = creator.createFromParcel(parcel);
        parcel.setDataPosition(dataPosition + G);
        return createFromParcel;
    }

    @NonNull
    public static String o(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        String readString = parcel.readString();
        parcel.setDataPosition(dataPosition + G);
        return readString;
    }

    @NonNull
    public static String[] p(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        String[] createStringArray = parcel.createStringArray();
        parcel.setDataPosition(dataPosition + G);
        return createStringArray;
    }

    @NonNull
    public static ArrayList<String> q(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        ArrayList<String> createStringArrayList = parcel.createStringArrayList();
        parcel.setDataPosition(dataPosition + G);
        return createStringArrayList;
    }

    @NonNull
    public static <T> T[] r(@NonNull Parcel parcel, int i10, @NonNull Parcelable.Creator<T> creator) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        T[] tArr = (T[]) parcel.createTypedArray(creator);
        parcel.setDataPosition(dataPosition + G);
        return tArr;
    }

    @NonNull
    public static <T> ArrayList<T> s(@NonNull Parcel parcel, int i10, @NonNull Parcelable.Creator<T> creator) {
        int G = G(parcel, i10);
        int dataPosition = parcel.dataPosition();
        if (G == 0) {
            return null;
        }
        ArrayList<T> createTypedArrayList = parcel.createTypedArrayList(creator);
        parcel.setDataPosition(dataPosition + G);
        return createTypedArrayList;
    }

    public static void t(@NonNull Parcel parcel, int i10) {
        if (parcel.dataPosition() == i10) {
            return;
        }
        throw new C0300a("Overread allowed size end=" + i10, parcel);
    }

    public static int u(int i10) {
        return (char) i10;
    }

    public static boolean v(@NonNull Parcel parcel, int i10) {
        K(parcel, i10, 4);
        return parcel.readInt() != 0;
    }

    @NonNull
    public static Boolean w(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        if (G == 0) {
            return null;
        }
        J(parcel, i10, G, 4);
        return Boolean.valueOf(parcel.readInt() != 0);
    }

    public static double x(@NonNull Parcel parcel, int i10) {
        K(parcel, i10, 8);
        return parcel.readDouble();
    }

    @NonNull
    public static Double y(@NonNull Parcel parcel, int i10) {
        int G = G(parcel, i10);
        if (G == 0) {
            return null;
        }
        J(parcel, i10, G, 8);
        return Double.valueOf(parcel.readDouble());
    }

    public static float z(@NonNull Parcel parcel, int i10) {
        K(parcel, i10, 4);
        return parcel.readFloat();
    }
}