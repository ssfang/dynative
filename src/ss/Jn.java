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

	public native static void test(int a, int b, int c, int d, int f);

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
			registerNativeMethodStub(method, method.getDeclaringClass(), method.getName(), getSignature(method));
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
			String methodSignature);

	/**
	 * @param method
	 * @param receiver
	 *            if the method is a static method, it just denotes a class, otherwise, the method is an
	 *            instance method and it represents an instance or is called "this" pointer.
	 * @return arguments size to maintain the stack.
	 */
	static int resolveNativeMethod(Method method, Object methodClass) {

		int size = 0;
		Class<?>[] parameterTypes = method.getParameterTypes();
		for (int idx = parameterTypes.length; --idx >= 2;) {
			size += MappedType.of(parameterTypes[idx]).getType().size();
		}
		System.out.println("resolveNativeMethod(" + method + ") `add %rax, %rsp` with rax = " + size);
		return size;
	}

	private static native void init(Method methods);

	static {
		try {
			init(Jn.class.getDeclaredMethod("resolveNativeMethod", Method.class, Object.class));
		} catch (NoSuchMethodException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}