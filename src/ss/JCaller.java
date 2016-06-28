package ss;

/**
 * This class extends the {@code Class} {@link SecurityManager} so as to expose its method
 * {@link #getClassContext(String)}, which is a protected method of super {@code Class}.
 */
public class JCaller extends SecurityManager {

	/** An instance */
	public static final JCaller INSTANCE = new JCaller();

	/**
	 * The index of the class of the currently executing method.
	 * 
	 * @see #getClassStack()
	 */
	public static final int CURRENT_CLASS_INDEX = 1;

	/**
	 * The index of the class of that method's caller.
	 * 
	 * @see #getClassStack()
	 */
	public static final int CALLING_CLASS_INDEX = 2;

	/**
	 * Returns the class of the currently executing method's caller in the current execution stack. Returns
	 * null if this method {@link #getCallingClass()} is executed in a Java {@code main} method like
	 * {@code public static void main (String[] args) throws java.lang.Exception}.
	 * 
	 * @return the class of that calling current method's caller
	 */
	@SuppressWarnings("rawtypes")
	public static final Class getCallingClass() {
		final Class[] cs = INSTANCE.getClassContext();
		return cs.length > 2 ? cs[CALLING_CLASS_INDEX] : null;
	}

	/**
	 * Returns the current execution stack as an array of classes.
	 * <p>
	 * The length of the array is the number of methods on the execution stack. The element at index
	 * <code>0</code> is the class {@link JCaller}, the element at index <code>1</code> is the class of the
	 * currently executing method, the element at index <code>2</code> is the class of that method's caller,
	 * and so on.
	 * <p>
	 * e.g.
	 * 
	 * <pre>
	 * public class Fang {
	 * 	static void test() {
	 * 		System.out.println(Arrays.toString(Caller.INSTANCE.getClassStack()));
	 * 	}
	 * 
	 * 	public static void test2() {
	 * 		test();
	 * 	}
	 * }
	 * 
	 * public class JMain {
	 * 	public static void main(String[] args) throws java.lang.Exception {
	 * 		Fang.test();
	 * 		Fang.class.getMethod("test").invoke(null);
	 * 
	 * 		Fang.test2();
	 * 
	 * 		System.out.println(Arrays.toString(Caller.INSTANCE.getClassStack()));
	 * 		System.out.println(Caller.getCallingClass());
	 * 	}
	 * }
	 * </pre>
	 * 
	 * Output: <code> [class Caller, class Fang, class JMain] </code>
	 * 
	 * @return the execution stack.
	 */
	@SuppressWarnings("rawtypes")
	public Class[] getClassStack() {
		return super.getClassContext();
	}
}