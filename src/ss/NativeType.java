package ss;

public enum NativeType {
	/** long double */
	LDOUBLE(12),
	/** -6, double 64bits */
	DOUBLE(8), // -6, double 64bits
	/** -5, float 32bits */
	FLOAT(4),

	/** Only apply to the return value */
	VOID(1),

	/** 1, Composite type: an array */
	ARRAY(8), // 1, Composite type
	/** 2, Composite type: a structure */
	STRUCT(8),

	/** 3(8=2^3), char, int8_t, uint8_t */
	INT8(1),
	/** 4(2bytes = 16bits = 2^4bits) */
	INT16(2),
	/**
	 * 5(64=2^5), int, long, char*, T* on 32bits machine, int32_t, uint32_t;
	 */
	INT32(4),
	/** 6(64=2^6), char*, type* on 64bits machine, int64_t, uint64_t; */
	INT64(8);
	/**
	 * only for the annotation, deduce the default type id from the type declaration of a field or a
	 * parameter.
	 */
	public static final NativeType DEFAULT = null;
	static final NativeType INTPTR;
	private static final int VOID_ORDINAL = 3;

	static {
		boolean is64bits = System.getProperty("sun.arch.data.model").endsWith("64");
		INTPTR = is64bits ? INT64 : INT32;
	}

	private final short size;
	private final short alignment;

	private NativeType(int size) {
		this.size = (short) size;
		this.alignment = (short) size;
	}

	public int size() {
		return size;
	}

	public int typeid() {
		return ordinal() - VOID_ORDINAL;
	}

	public short alignment() {
		return alignment;
	}
}