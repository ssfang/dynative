package ss;

import java.lang.reflect.AccessibleObject;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

/**
 * <h3>Reflection Support</h3>
 * <p/>
 * Programmers can use the JNI to call Java methods or access Java fields if they know the name and type of
 * the methods or fields. The Java Core Reflection API allows programmers to introspect Java classes at
 * runtime. JNI provides a set of conversion functions between field and method IDs used in the JNI to field
 * and method objects used in the Java Core Reflection API.
 * 
 */
public class Jn {

	public static String getSignature(Method m) {
		return getSignature(m.getReturnType(), m.getParameterTypes());
	}

	/**
	 * Returns the type signature corresponding to the given constructor.
	 * 
	 * @param ctor
	 *            a {@link Constructor Constructor} object.
	 * @return the type signature of the given constructor.
	 */
	public static String getSignature(Constructor<?> ctor) {
		return getSignature("V", ctor.getParameterTypes());
	}

	/**
	 * Returns the type signature of a java method corresponding to the given parameter and return types.
	 * 
	 * @param returnType
	 *            the return type for the method.
	 * @param parameterTypes
	 *            the parameter types for the method.
	 * @return the type signature corresponding to the given parameter and return types.
	 */
	public static String getSignature(Class<?> returnType, Class<?>... parameterTypes) {
		return getSignature(getSignature(returnType), parameterTypes);
	}

	private static String getSignature(String returnTypeName, Class<?>... parameterTypes) {
		StringBuilder sb = new StringBuilder();
		sb.append('(');
		for (Class<?> type : parameterTypes) {
			sb.append(getSignature(type));
		}
		sb.append(')');
		return sb.append(returnTypeName).toString();
	}

	/**
	 * Converts a Java source-language class name into the internal form. The internal name of a class is
	 * its fully qualified name, as returned by Class.getName(), where '.' are replaced by '/'. The
	 * following are examples of strings returned by this method:
	 *
	 * <pre>
	 *     "Ljava/lang/Object;"
	 *     "I"
	 *     "[[B"
	 * </pre>
	 * 
	 * @param c
	 *            an object or array class.
	 * @return the internal name form of the given class
	 * 
	 */
	public static String getSignature(Class<?> c) {
		if (c.isPrimitive()) {
			return MappedType.pof(c).getSig();
		} else {
			// Void.class: public Void return_Void() { return null; }
			String internalName = c.getName().replace('.', '/');
			if (c.isArray()) {
				/* Already in the correct array style. */
				return internalName;
			} else {
				return 'L' + internalName + ';';
			}
		}
	}

	/**
	 * Converts a java.lang.reflect.Field to a field ID.
	 * 
	 * @param field
	 * @return a field ID
	 * @linkage LINKAGE: Index 8 in the JNIEnv interface function table.
	 */
	public native static long FromReflectedField(Field field);

	/**
	 * Converts a java.lang.reflect.Method or java.lang.reflect.Constructor object to a method ID.
	 * 
	 * @param a
	 *            method or constructor
	 * @return a method ID
	 * @linkage LINKAGE: Index 7 in the JNIEnv interface function table.
	 */
	private native static long FromReflectedMethod(AccessibleObject ctorOrMethod);

	/**
	 * Converts a java.lang.reflect.Method object to a method ID.
	 * 
	 * @param method
	 * @return a method ID
	 */
	public static final long FromReflectedMethod(Method method) {
		return FromReflectedMethod((AccessibleObject) method);
	}

	/**
	 * Converts a java.lang.reflect.Method object to a method ID.
	 * 
	 * @param ctor
	 * @return a method ID
	 */
	public static final long FromReflectedConstructor(Constructor<?> ctor) {
		return FromReflectedMethod((AccessibleObject) ctor);
	}

	public native static long GetStringUTFChars(String str);

	public native static void ReleaseStringUTFChars(String field, long charsaddr);

	public static void registerNativeMethodStub(Method method) {
		if (null != method && Modifier.isNative(method.getModifiers())) {
			int size = 0;
			for (Class<?> type : method.getParameterTypes()) {
				size += MappedType.of(type).getType().size();
			}
			registerNativeMethodStub(method, method.getDeclaringClass(), method.getName(), getSignature(method), size);
		}
	}

	/**
	 * <pre>
	 * jint RegisterNatives(JNIEnv *env, jclass clazz, const JNINativeMethod *methods, jint nMethods);
	 * 
	 * typedef struct { 
	 *  char *name; 
	 *  char *signature; 
	 *  void *fnPtr; 
	 * } JNINativeMethod;
	 * </pre>
	 * 
	 * @param method
	 *            a native method
	 * @param methodClass
	 *            a class declaring this native method
	 * @param methodName
	 *            the name of the method
	 * @param methodSignature
	 * @return Returns 0 on success; returns a negative value on failure.
	 * @throws NoSuchMethodError
	 *             if a specified method cannot be found or if the method is not native.
	 */
	public native static int registerNativeMethodStub(Method method, Class<?> methodClass, String methodName,
			String methodSignature, int argsize);

	static {

	}

	public enum NativeType {
		/** long double */
		LDOUBLE(0),
		/** -6, double 64bits */
		DOUBLE(8), // -6, double 64bits
		/** -5, float 32bits */
		FLOAT(4),

		/** Only apply to the return value */
		VOID(1),

		/** 1, Composite type: an array */
		ARRAY(0), // 1, Composite type
		/** 2, Composite type: a structure */
		STRUCT(0),

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
		private static final int VOID_ORDINAL = 3;

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

	public enum MappedType {
		ARRAY(Object[].class, "[", NativeType.ARRAY), //
		OBJECT(Object.class, "L", NativeType.DEFAULT), //

		PVOID(void.class, "V", NativeType.VOID), //
		PSHORT(short.class, "S", NativeType.INT8), //
		PBOOLEAN(boolean.class, "Z", NativeType.INT8), //
		PBYTE(byte.class, "B", NativeType.INT8), //
		PCHAR(char.class, "C", NativeType.INT16), //
		PDOUBLE(double.class, "D", NativeType.DOUBLE), //
		PFLOAT(float.class, "F", NativeType.FLOAT), //
		PINT(int.class, "I", NativeType.INT32), //
		PLONG(long.class, "J", NativeType.INT64);

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

		final String sig;
		final NativeType ntype;
		final Class<?> clazz;
	}
}