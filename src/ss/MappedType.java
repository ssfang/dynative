package ss;

public enum MappedType {
	ARRAY(Object[].class, "[", NativeType.ARRAY), // jarray -> (jobject) -> void*
	OBJECT(Object.class, "L", NativeType.INTPTR), // jobject -> void*

	PVOID(void.class, "V", NativeType.VOID), // void
	PSHORT(short.class, "S", NativeType.INT8), // jshort
	PBOOLEAN(boolean.class, "Z", NativeType.INT8), // jboolean
	PBYTE(byte.class, "B", NativeType.INT8), // jbyte
	PCHAR(char.class, "C", NativeType.INT16), // jchar
	PDOUBLE(double.class, "D", NativeType.DOUBLE), // jdouble
	PFLOAT(float.class, "F", NativeType.FLOAT), // jfloat
	PINT(int.class, "I", NativeType.INT32), // jint
	PLONG(long.class, "J", NativeType.INT64); // jlong

	private MappedType(Class<?> c, String jsign, NativeType ntype) {
		clazz = c;
		this.sig = jsign;
		this.ntype = ntype;
	}

	public static MappedType pof(Class<?> t) {
		if (int.class == t) {
			return MappedType.PINT;
		} else if (long.class == t) {
			return MappedType.PLONG;
		} else if (float.class == t) {
			return MappedType.PFLOAT;
		} else if (double.class == t) {
			return MappedType.PDOUBLE;
		} else if (char.class == t) {
			return MappedType.PCHAR;
		} else if (short.class == t) {
			return MappedType.PSHORT;
		} else if (byte.class == t) {
			return MappedType.PBYTE;
		} else if (boolean.class == t) {
			return MappedType.PBOOLEAN;
		} else if (void.class == t) {
			return MappedType.PVOID;
		} else {
			throw new IllegalArgumentException("Should never reach here");
		}
	}

	public static MappedType of(Class<?> t) {
		if (t.isPrimitive()) {
			return pof(t);
		} else if (t.isArray()) {
			return MappedType.ARRAY;
		} else {
			return MappedType.OBJECT;
		}
	}

	public Class<?> getClazz() {
		return clazz;
	}

	public String getSig() {
		return sig;
	}

	public NativeType getType() {
		return ntype;
	}

	/** @return sizeof(jobject), sizeof(jint), ... */
	public int getNativeSize() {
		return ntype.size();
	}

	final String sig;
	final NativeType ntype;
	final Class<?> clazz;
}
