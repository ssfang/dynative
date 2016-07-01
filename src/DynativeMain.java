import java.lang.reflect.Method;

import ss.Jn;

public class DynativeMain {

	public static void main(String[] args) throws Exception {
		Methods ms = new Methods(Jn.class);
		
		Method m = ms.getMethod("FromReflectedField");
		Jn.registerNativeMethodStub(m);
		
		m = ms.getMethod("ReleaseStringUTFChars");
		Jn.registerNativeMethodStub(ms.getMethod("ReleaseStringUTFChars"));
		
		long mid = Jn.FromReflectedField(Methods.class.getDeclaredField("methods"));
		Jn.ReleaseStringUTFChars(null, 0L);
		
		System.out.println("mid = " + mid);
		
		Jn.registerNativeMethodStub(ms.getMethod("test"));
		Jn.test(22, 26, 28, 66, 68);
		Jn.test(22, 26, 28, 66, 68);
		Jn.test(22, 26, 28, 66, 68);
	}

	static {
		// -Djava.library.path=lib
		String lib_path = System.getProperty("java.library.path");
		System.out.println("java.library.path = " + lib_path);
		System.out.println("os.arch = " + System.getProperty("os.arch"));

		// // java -version
		// // Sun has a Java System property to determine the bitness of the
		// JVM: 32 or 64:
		// //
		// http://www.oracle.com/technetwork/java/hotspotfaq-138619.html#64bit_detection
		// // the system property sun.arch.data.model has the value "32", "64",
		// or "unknown"
		// // sun.arch.data.model=32 // 32 bit JVM
		// // sun.arch.data.model=64 // 64 bit JVM
		// System.out.println("sun.arch.data.model = " +
		// System.getProperty("sun.arch.data.model"));
		//
		try {
			// jn.dll (Windows) or libjn.so (Unixes)
			System.loadLibrary("jn"); // Notice lack of lib prefix
		} catch (UnsatisfiedLinkError e) {
			// e.printStackTrace();
			throw e;
		}
	}

	public static class Methods {
		private final Method[] methods;

		public Methods(Class<?> cls) {
			this.methods = cls.getDeclaredMethods();
		}

		public Methods(Method... methods) {
			this.methods = methods;
		}

		public Method[] getMethods() {
			return methods;
		}

		/**
		 * Returns the index of the first method with the specified name.
		 * <p/>
		 * Inner comparation is {@link Method#getName()} == {@link String#intern()}, which is efficient for
		 * a const string
		 * 
		 * @throws NoSuchMethodException
		 * @see Class#searchMethods
		 */
		public int getIndex(String methodName) throws NoSuchMethodException {
			Method[] methods = this.methods;
			String internedName = methodName.intern();
			for (int i = 0, n = methods.length; i < n; i++)
				if (methods[i].getName() == internedName)
					return i;
			throw new NoSuchMethodException(methodName);
		}

		public Method getMethod(String methodName) throws NoSuchMethodException {
			return methods[getIndex(methodName)];
		}
	}
}
