/**

 JNIEnv *e;
 jobject method;
 // e->FromReflectedMethod(e, mid); // in c
 // e->FromReflectedMethod(mid); // in c++

 // I think it's a nice idea to define INLINE functions with a prefix jni to make
 // it unified and simple for c or c++ except all interfaces needing definition.
 //jniFromReflectedMethod(e, mid);

 // javascript
 1. /^[\t ]*(([\w_*]+[\t ]+)+)(\*?)[\t ]*(\w+)\(([^)]*)\)\s*{/gm ==> INLINE \1\3jni\4\(JNIEnv *env, \5\){
 2. functions ==> env
 3. to c: /this[\t ]*(,?)/g ==> env\1   to c++   /this[\t ]*(,?)/g ==>

 e.g.
 #include "jni.h"
 extern "C" {
 // native static long FromReflectedConstructor(Constructor<?> ctor);
 JNIEXPORT jlong JNICALL Java_JnToy_FromReflectedConstructor(JNIEnv *env,
 jclass UNUSED(cls),
 jobject method) {
 // cast jmethodID to jlong
 return REINTERPRET_CAST(jlong, jniFromReflectedMethod(env, method));
 }

 // native static long FromReflectedMethod(Method method);
 JNIEXPORT jlong JNICALL Java_JnToy_FromReflectedMethod(JNIEnv *env,
 jclass UNUSED(cls),
 jobject method) {
 // cast jmethodID to jlong
 return REINTERPRET_CAST(jlong, jniFromReflectedMethod(env, method));
 }
 }
 */

#ifndef _Included_JNI
#define _Included_JNI

#include <jni.h>
//#include <stdarg.h>

#ifndef JNI_ENV_ARG
/*
 e.g.
 JNIEXPORT void JNICALL Java_test_n(JNIEnv *env, jobject obj, jobject o)
 {
 jmethodID jmid = JNI_ENV_PTR(env) -> FromReflectedMethod(JNI_ENV_ARG(env, o));
 JNI_ENV_PTR(env) -> CallVoidMethod(JNI_ENV_ARG(env, obj), jmid, 7);
 return;
 }
 */
#ifdef __cplusplus
# define JNI_ENV_ARG(x, y) y
# define JNI_ENV_PTR(x) x
#else
# define JNI_ENV_ARG(x,y) x, y
# define JNI_ENV_PTR(x) (*x)
# endif
#endif

#define JNI_CALL(FUNC, ENV, ...) ENV->FUNC(ENV, ##args);

#ifndef INLINE
/* Don't vs2013 12.0.21005.1 REL c compiler support c99 inline? */
#if defined _MSC_VER && !defined __cplusplus
# define INLINE __inline
#else
# define INLINE inline
#endif
#endif

typedef jboolean JBoolean;
typedef jbyte JByte;
typedef jchar JChar;
typedef jshort JShort;
typedef jint JInt;
typedef jlong JLong;
typedef jfloat JFloat;
typedef jdouble JDouble;


#ifdef __cplusplus
/**
 Version Information
 GetVersion
 jint GetVersion(JNIEnv *env);

 Returns the version of the native method interface.


 LINKAGE:
 Index 4 in the JNIEnv interface function table.

 PARAMETERS:
 env: the JNI interface pointer.

 RETURNS:
 Returns the major version number in the higher 16 bits and the minor version number in the lower 16 bits.

 In JDK/JRE 1.1, GetVersion() returns 0x00010001.

 In JDK/JRE 1.2, GetVersion() returns 0x00010002.

 In JDK/JRE 1.4, GetVersion() returns 0x00010004.

 In JDK/JRE 1.6, GetVersion() returns 0x00010006.
 */
INLINE jint jniGetVersion(JNIEnv *env) {
	return env->GetVersion();
}
INLINE jclass jniDefineClass(JNIEnv *env, const char *name, jobject loader, const jbyte *buf, jsize len) {
	return env->DefineClass(name, loader, buf, len);
}
INLINE jclass jniFindClass(JNIEnv *env, const char *name) {
	return env->FindClass(name);
}
INLINE jmethodID jniFromReflectedMethod(JNIEnv *env, jobject method) {
	return env->FromReflectedMethod(method);
}
INLINE jfieldID jniFromReflectedField(JNIEnv *env, jobject field) {
	return env->FromReflectedField(field);
}

INLINE jobject jniToReflectedMethod(JNIEnv *env, jclass cls, jmethodID methodID, jboolean isStatic) {
	return env->ToReflectedMethod(cls, methodID, isStatic);
}

INLINE jclass jniGetSuperclass(JNIEnv *env, jclass sub) {
	return env->GetSuperclass(sub);
}
INLINE jboolean jniIsAssignableFrom(JNIEnv *env, jclass sub, jclass sup) {
	return env->IsAssignableFrom(sub, sup);
}

INLINE jobject jniToReflectedField(JNIEnv *env, jclass cls, jfieldID fieldID, jboolean isStatic) {
	return env->ToReflectedField(cls, fieldID, isStatic);
}

INLINE jint jniThrow(JNIEnv *env, jthrowable obj) {
	return env->Throw(obj);
}
INLINE jint jniThrowNew(JNIEnv *env, jclass clazz, const char *msg) {
	return env->ThrowNew(clazz, msg);
}
INLINE jthrowable jniExceptionOccurred(JNIEnv *env) {
	return env->ExceptionOccurred();
}

/*
 *void ExceptionDescribe(JNIEnv *env);

 Prints an exception and a backtrace of the stack to a system error-reporting
 channel, such as stderr. This is a convenience routine provided for debugging.

 LINKAGE:
 Index 16 in the JNIEnv interface function table.

 PARAMETERS:
 env: the JNI interface pointer.
 */
INLINE void jniExceptionDescribe(JNIEnv *env) {
	env->ExceptionDescribe();
}
/*
void ExceptionClear(JNIEnv *env);

Clears any exception that is currently being thrown. If no exception is
currently being thrown, this routine has no effect.

LINKAGE:
Index 17 in the JNIEnv interface function table.

PARAMETERS:
env: the JNI interface pointer.
 */
INLINE void jniExceptionClear(JNIEnv *env) {
	env->ExceptionClear();
}

/*
void FatalError(JNIEnv *env, const char *msg);

Raises a fatal error and does not expect the VM to recover. This function does not return.

LINKAGE:
Index 18 in the JNIEnv interface function table.

PARAMETERS:
env: the JNI interface pointer.

msg: an error message. The string is encoded in modified UTF-8.
 */
INLINE void jniFatalError(JNIEnv *env, const char *msg) {
	env->FatalError(msg);
}

INLINE jint jniPushLocalFrame(JNIEnv *env, jint capacity) {
	return env->PushLocalFrame(capacity);
}
INLINE jobject jniPopLocalFrame(JNIEnv *env, jobject result) {
	return env->PopLocalFrame(result);
}

INLINE jobject jniNewGlobalRef(JNIEnv *env, jobject lobj) {
	return env->NewGlobalRef(lobj);
}
INLINE void jniDeleteGlobalRef(JNIEnv *env, jobject gref) {
	env->DeleteGlobalRef(gref);
}
INLINE void jniDeleteLocalRef(JNIEnv *env, jobject obj) {
	env->DeleteLocalRef(obj);
}

INLINE jboolean jniIsSameObject(JNIEnv *env, jobject obj1, jobject obj2) {
	return env->IsSameObject(obj1, obj2);
}

INLINE jobject jniNewLocalRef(JNIEnv *env, jobject ref) {
	return env->NewLocalRef(ref);
}
INLINE jint jniEnsureLocalCapacity(JNIEnv *env, jint capacity) {
	return env->EnsureLocalCapacity(capacity);
}

/*
 Object Operations
 */

/**
 AllocObject
 jobject AllocObject(JNIEnv *env, jclass clazz);

 Allocates a new Java object without invoking any of the constructors for the object. Returns a reference to the object.

 The clazz argument must not refer to an array class.


 LINKAGE:
 Index 27 in the JNIEnv interface function table.

 PARAMETERS:
 env: the JNI interface pointer.

 clazz: a Java class object.

 RETURNS:
 Returns a Java object, or NULL if the object cannot be constructed.

 THROWS:
 InstantiationException: if the class is an interface or an abstract class.

 OutOfMemoryError: if the system runs out of memory.
 */
INLINE jobject jniAllocObject(JNIEnv *env, jclass clazz) {
	return env->AllocObject(clazz);
}
/*
 NewObject
 NewObjectA
 NewObjectV
 jobject NewObject(JNIEnv *env, jclass clazz,
 jmethodID methodID, ...);

 jobject NewObjectA(JNIEnv *env, jclass clazz,
 jmethodID methodID, const jvalue *args);

 jobject NewObjectV(JNIEnv *env, jclass clazz,
 jmethodID methodID, va_list args);

 Constructs a new Java object. The method ID indicates which constructor method to invoke. This ID must be obtained by calling GetMethodID() with <init> as the method name and void (V) as the return type.

 The clazz argument must not refer to an array class.

 NewObject
 Programmers place all arguments that are to be passed to the constructor immediately following the methodID argument. NewObject() accepts these arguments and passes them to the Java method that the programmer wishes to invoke.


 LINKAGE:
 Index 28 in the JNIEnv interface function table.

 NewObjectA
 Programmers place all arguments that are to be passed to the constructor in an args array of jvalues that immediately follows the methodID argument. NewObjectA() accepts the arguments in this array, and, in turn, passes them to the Java method that the programmer wishes to invoke.


 LINKAGE:
 Index 30 in the JNIEnv interface function table.

 NewObjectV
 Programmers place all arguments that are to be passed to the constructor in an args argument of type va_list that immediately follows the methodID argument. NewObjectV() accepts these arguments, and, in turn, passes them to the Java method that the programmer wishes to invoke.


 LINKAGE:
 Index 29 in the JNIEnv interface function table.

 PARAMETERS:
 env: the JNI interface pointer.

 clazz: a Java class object.

 methodID: the method ID of the constructor.

 Additional Parameter for NewObject:
 arguments to the constructor.

 Additional Parameter for NewObjectA:
 args: an array of arguments to the constructor.

 Additional Parameter for NewObjectV:
 args: a va_list of arguments to the constructor.

 RETURNS:
 Returns a Java object, or NULL if the object cannot be constructed.

 THROWS:
 InstantiationException: if the class is an interface or an abstract class.

 OutOfMemoryError: if the system runs out of memory.

 Any exceptions thrown by the constructor.
 */
INLINE jobject jniNewObject(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jobject result;
	va_start(args, methodID);
	result = env->NewObjectV(clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jobject jniNewObjectV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args) {
	return env->NewObjectV(clazz, methodID, args);
}
INLINE jobject jniNewObjectA(JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args) {
	return env->NewObjectA(clazz, methodID, args);
}

INLINE jclass jniGetObjectClass(JNIEnv *env, jobject obj) {
	return env->GetObjectClass(obj);
}
INLINE jboolean jniIsInstanceOf(JNIEnv *env, jobject obj, jclass clazz) {
	return env->IsInstanceOf(obj, clazz);
}

INLINE jmethodID jniGetMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
	return env->GetMethodID(clazz, name, sig);
}

INLINE jobject jniCallObjectMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jobject result;
	va_start(args, methodID);
	result = env->CallObjectMethodV(obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jobject jniCallObjectMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
	return env->CallObjectMethodV(obj, methodID, args);
}
INLINE jobject jniCallObjectMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args) {
	return env->CallObjectMethodA(obj, methodID, args);
}

INLINE jboolean jniCallBooleanMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jboolean result;
	va_start(args, methodID);
	result = env->CallBooleanMethodV(obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jboolean jniCallBooleanMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
	return env->CallBooleanMethodV(obj, methodID, args);
}
INLINE jboolean jniCallBooleanMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args) {
	return env->CallBooleanMethodA(obj, methodID, args);
}

INLINE jbyte jniCallByteMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jbyte result;
	va_start(args, methodID);
	result = env->CallByteMethodV(obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jbyte jniCallByteMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
	return env->CallByteMethodV(obj, methodID, args);
}
INLINE jbyte jniCallByteMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args) {
	return env->CallByteMethodA(obj, methodID, args);
}

INLINE jchar jniCallCharMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jchar result;
	va_start(args, methodID);
	result = env->CallCharMethodV(obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jchar jniCallCharMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
	return env->CallCharMethodV(obj, methodID, args);
}
INLINE jchar jniCallCharMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args) {
	return env->CallCharMethodA(obj, methodID, args);
}

INLINE jshort jniCallShortMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jshort result;
	va_start(args, methodID);
	result = env->CallShortMethodV(obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jshort jniCallShortMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
	return env->CallShortMethodV(obj, methodID, args);
}
INLINE jshort jniCallShortMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args) {
	return env->CallShortMethodA(obj, methodID, args);
}

INLINE jint jniCallIntMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jint result;
	va_start(args, methodID);
	result = env->CallIntMethodV(obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jint jniCallIntMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
	return env->CallIntMethodV(obj, methodID, args);
}
INLINE jint jniCallIntMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args) {
	return env->CallIntMethodA(obj, methodID, args);
}

INLINE jlong jniCallLongMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jlong result;
	va_start(args, methodID);
	result = env->CallLongMethodV(obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jlong jniCallLongMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
	return env->CallLongMethodV(obj, methodID, args);
}
INLINE jlong jniCallLongMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args) {
	return env->CallLongMethodA(obj, methodID, args);
}

INLINE jfloat jniCallFloatMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jfloat result;
	va_start(args, methodID);
	result = env->CallFloatMethodV(obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jfloat jniCallFloatMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
	return env->CallFloatMethodV(obj, methodID, args);
}
INLINE jfloat jniCallFloatMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args) {
	return env->CallFloatMethodA(obj, methodID, args);
}

INLINE jdouble jniCallDoubleMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jdouble result;
	va_start(args, methodID);
	result = env->CallDoubleMethodV(obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jdouble jniCallDoubleMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
	return env->CallDoubleMethodV(obj, methodID, args);
}
INLINE jdouble jniCallDoubleMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args) {
	return env->CallDoubleMethodA(obj, methodID, args);
}

INLINE void jniCallVoidMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	va_start(args, methodID);
	env->CallVoidMethodV(obj, methodID, args);
	va_end(args);
}
INLINE void jniCallVoidMethodV(JNIEnv *env, jobject obj, jmethodID methodID, va_list args) {
	env->CallVoidMethodV(obj, methodID, args);
}
INLINE void jniCallVoidMethodA(JNIEnv *env, jobject obj, jmethodID methodID, const jvalue * args) {
	env->CallVoidMethodA(obj, methodID, args);
}

INLINE jobject jniCallNonvirtualObjectMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		...) {
	va_list args;
	jobject result;
	va_start(args, methodID);
	result = env->CallNonvirtualObjectMethodV(obj, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jobject jniCallNonvirtualObjectMethodV(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		va_list args) {
	return env->CallNonvirtualObjectMethodV(obj, clazz, methodID, args);
}
INLINE jobject jniCallNonvirtualObjectMethodA(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		const jvalue * args) {
	return env->CallNonvirtualObjectMethodA(obj, clazz, methodID, args);
}

INLINE jboolean jniCallNonvirtualBooleanMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		...) {
	va_list args;
	jboolean result;
	va_start(args, methodID);
	result = env->CallNonvirtualBooleanMethodV(obj, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jboolean jniCallNonvirtualBooleanMethodV(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		va_list args) {
	return env->CallNonvirtualBooleanMethodV(obj, clazz, methodID, args);
}
INLINE jboolean jniCallNonvirtualBooleanMethodA(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		const jvalue * args) {
	return env->CallNonvirtualBooleanMethodA(obj, clazz, methodID, args);
}

INLINE jbyte jniCallNonvirtualByteMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jbyte result;
	va_start(args, methodID);
	result = env->CallNonvirtualByteMethodV(obj, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jbyte jniCallNonvirtualByteMethodV(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		va_list args) {
	return env->CallNonvirtualByteMethodV(obj, clazz, methodID, args);
}
INLINE jbyte jniCallNonvirtualByteMethodA(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		const jvalue * args) {
	return env->CallNonvirtualByteMethodA(obj, clazz, methodID, args);
}

INLINE jchar jniCallNonvirtualCharMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jchar result;
	va_start(args, methodID);
	result = env->CallNonvirtualCharMethodV(obj, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jchar jniCallNonvirtualCharMethodV(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		va_list args) {
	return env->CallNonvirtualCharMethodV(obj, clazz, methodID, args);
}
INLINE jchar jniCallNonvirtualCharMethodA(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		const jvalue * args) {
	return env->CallNonvirtualCharMethodA(obj, clazz, methodID, args);
}

INLINE jshort jniCallNonvirtualShortMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		...) {
	va_list args;
	jshort result;
	va_start(args, methodID);
	result = env->CallNonvirtualShortMethodV(obj, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jshort jniCallNonvirtualShortMethodV(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		va_list args) {
	return env->CallNonvirtualShortMethodV(obj, clazz, methodID, args);
}
INLINE jshort jniCallNonvirtualShortMethodA(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		const jvalue * args) {
	return env->CallNonvirtualShortMethodA(obj, clazz, methodID, args);
}

INLINE jint jniCallNonvirtualIntMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jint result;
	va_start(args, methodID);
	result = env->CallNonvirtualIntMethodV(obj, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jint jniCallNonvirtualIntMethodV(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		va_list args) {
	return env->CallNonvirtualIntMethodV(obj, clazz, methodID, args);
}
INLINE jint jniCallNonvirtualIntMethodA(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		const jvalue * args) {
	return env->CallNonvirtualIntMethodA(obj, clazz, methodID, args);
}

INLINE jlong jniCallNonvirtualLongMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jlong result;
	va_start(args, methodID);
	result = env->CallNonvirtualLongMethodV(obj, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jlong jniCallNonvirtualLongMethodV(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		va_list args) {
	return env->CallNonvirtualLongMethodV(obj, clazz, methodID, args);
}
INLINE jlong jniCallNonvirtualLongMethodA(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		const jvalue * args) {
	return env->CallNonvirtualLongMethodA(obj, clazz, methodID, args);
}

INLINE jfloat jniCallNonvirtualFloatMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		...) {
	va_list args;
	jfloat result;
	va_start(args, methodID);
	result = env->CallNonvirtualFloatMethodV(obj, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jfloat jniCallNonvirtualFloatMethodV(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		va_list args) {
	return env->CallNonvirtualFloatMethodV(obj, clazz, methodID, args);
}
INLINE jfloat jniCallNonvirtualFloatMethodA(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		const jvalue * args) {
	return env->CallNonvirtualFloatMethodA(obj, clazz, methodID, args);
}

INLINE jdouble jniCallNonvirtualDoubleMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		...) {
	va_list args;
	jdouble result;
	va_start(args, methodID);
	result = env->CallNonvirtualDoubleMethodV(obj, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jdouble jniCallNonvirtualDoubleMethodV(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		va_list args) {
	return env->CallNonvirtualDoubleMethodV(obj, clazz, methodID, args);
}
INLINE jdouble jniCallNonvirtualDoubleMethodA(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		const jvalue * args) {
	return env->CallNonvirtualDoubleMethodA(obj, clazz, methodID, args);
}

INLINE void jniCallNonvirtualVoidMethod(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	va_start(args, methodID);
	env->CallNonvirtualVoidMethodV(obj, clazz, methodID, args);
	va_end(args);
}
INLINE void jniCallNonvirtualVoidMethodV(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		va_list args) {
	env->CallNonvirtualVoidMethodV(obj, clazz, methodID, args);
}
INLINE void jniCallNonvirtualVoidMethodA(JNIEnv *env, jobject obj, jclass clazz, jmethodID methodID,
		const jvalue * args) {
	env->CallNonvirtualVoidMethodA(obj, clazz, methodID, args);
}

INLINE jfieldID jniGetFieldID(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
	return env->GetFieldID(clazz, name, sig);
}

INLINE jobject jniGetObjectField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return env->GetObjectField(obj, fieldID);
}
INLINE jboolean jniGetBooleanField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return env->GetBooleanField(obj, fieldID);
}
INLINE jbyte jniGetByteField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return env->GetByteField(obj, fieldID);
}
INLINE jchar jniGetCharField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return env->GetCharField(obj, fieldID);
}
INLINE jshort jniGetShortField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return env->GetShortField(obj, fieldID);
}
INLINE jint jniGetIntField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return env->GetIntField(obj, fieldID);
}
INLINE jlong jniGetLongField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return env->GetLongField(obj, fieldID);
}
INLINE jfloat jniGetFloatField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return env->GetFloatField(obj, fieldID);
}
INLINE jdouble jniGetDoubleField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return env->GetDoubleField(obj, fieldID);
}

INLINE void jniSetObjectField(JNIEnv *env, jobject obj, jfieldID fieldID, jobject val) {
	env->SetObjectField(obj, fieldID, val);
}
INLINE void jniSetBooleanField(JNIEnv *env, jobject obj, jfieldID fieldID, jboolean val) {
	env->SetBooleanField(obj, fieldID, val);
}
INLINE void jniSetByteField(JNIEnv *env, jobject obj, jfieldID fieldID, jbyte val) {
	env->SetByteField(obj, fieldID, val);
}
INLINE void jniSetCharField(JNIEnv *env, jobject obj, jfieldID fieldID, jchar val) {
	env->SetCharField(obj, fieldID, val);
}
INLINE void jniSetShortField(JNIEnv *env, jobject obj, jfieldID fieldID, jshort val) {
	env->SetShortField(obj, fieldID, val);
}
INLINE void jniSetIntField(JNIEnv *env, jobject obj, jfieldID fieldID, jint val) {
	env->SetIntField(obj, fieldID, val);
}
INLINE void jniSetLongField(JNIEnv *env, jobject obj, jfieldID fieldID, jlong val) {
	env->SetLongField(obj, fieldID, val);
}
INLINE void jniSetFloatField(JNIEnv *env, jobject obj, jfieldID fieldID, jfloat val) {
	env->SetFloatField(obj, fieldID, val);
}
INLINE void jniSetDoubleField(JNIEnv *env, jobject obj, jfieldID fieldID, jdouble val) {
	env->SetDoubleField(obj, fieldID, val);
}

INLINE jmethodID jniGetStaticMethodID(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
	return env->GetStaticMethodID(clazz, name, sig);
}

INLINE jobject jniCallStaticObjectMethod(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jobject result;
	va_start(args, methodID);
	result = env->CallStaticObjectMethodV(clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jobject jniCallStaticObjectMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args) {
	return env->CallStaticObjectMethodV(clazz, methodID, args);
}
INLINE jobject jniCallStaticObjectMethodA(JNIEnv *env, jclass clazz, jmethodID methodID,
		const jvalue *args) {
	return env->CallStaticObjectMethodA(clazz, methodID, args);
}

INLINE jboolean jniCallStaticBooleanMethod(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jboolean result;
	va_start(args, methodID);
	result = env->CallStaticBooleanMethodV(clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jboolean jniCallStaticBooleanMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args) {
	return env->CallStaticBooleanMethodV(clazz, methodID, args);
}
INLINE jboolean jniCallStaticBooleanMethodA(JNIEnv *env, jclass clazz, jmethodID methodID,
		const jvalue *args) {
	return env->CallStaticBooleanMethodA(clazz, methodID, args);
}

INLINE jbyte jniCallStaticByteMethod(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jbyte result;
	va_start(args, methodID);
	result = env->CallStaticByteMethodV(clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jbyte jniCallStaticByteMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args) {
	return env->CallStaticByteMethodV(clazz, methodID, args);
}
INLINE jbyte jniCallStaticByteMethodA(JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args) {
	return env->CallStaticByteMethodA(clazz, methodID, args);
}

INLINE jchar jniCallStaticCharMethod(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jchar result;
	va_start(args, methodID);
	result = env->CallStaticCharMethodV(clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jchar jniCallStaticCharMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args) {
	return env->CallStaticCharMethodV(clazz, methodID, args);
}
INLINE jchar jniCallStaticCharMethodA(JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args) {
	return env->CallStaticCharMethodA(clazz, methodID, args);
}

INLINE jshort jniCallStaticShortMethod(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jshort result;
	va_start(args, methodID);
	result = env->CallStaticShortMethodV(clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jshort jniCallStaticShortMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args) {
	return env->CallStaticShortMethodV(clazz, methodID, args);
}
INLINE jshort jniCallStaticShortMethodA(JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args) {
	return env->CallStaticShortMethodA(clazz, methodID, args);
}

INLINE jint jniCallStaticIntMethod(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jint result;
	va_start(args, methodID);
	result = env->CallStaticIntMethodV(clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jint jniCallStaticIntMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args) {
	return env->CallStaticIntMethodV(clazz, methodID, args);
}
INLINE jint jniCallStaticIntMethodA(JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args) {
	return env->CallStaticIntMethodA(clazz, methodID, args);
}

INLINE jlong jniCallStaticLongMethod(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jlong result;
	va_start(args, methodID);
	result = env->CallStaticLongMethodV(clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jlong jniCallStaticLongMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args) {
	return env->CallStaticLongMethodV(clazz, methodID, args);
}
INLINE jlong jniCallStaticLongMethodA(JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args) {
	return env->CallStaticLongMethodA(clazz, methodID, args);
}

INLINE jfloat jniCallStaticFloatMethod(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jfloat result;
	va_start(args, methodID);
	result = env->CallStaticFloatMethodV(clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jfloat jniCallStaticFloatMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args) {
	return env->CallStaticFloatMethodV(clazz, methodID, args);
}
INLINE jfloat jniCallStaticFloatMethodA(JNIEnv *env, jclass clazz, jmethodID methodID, const jvalue *args) {
	return env->CallStaticFloatMethodA(clazz, methodID, args);
}

INLINE jdouble jniCallStaticDoubleMethod(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jdouble result;
	va_start(args, methodID);
	result = env->CallStaticDoubleMethodV(clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jdouble jniCallStaticDoubleMethodV(JNIEnv *env, jclass clazz, jmethodID methodID, va_list args) {
	return env->CallStaticDoubleMethodV(clazz, methodID, args);
}
INLINE jdouble jniCallStaticDoubleMethodA(JNIEnv *env, jclass clazz, jmethodID methodID,
		const jvalue *args) {
	return env->CallStaticDoubleMethodA(clazz, methodID, args);
}

INLINE void jniCallStaticVoidMethod(JNIEnv *env, jclass cls, jmethodID methodID, ...) {
	va_list args;
	va_start(args, methodID);
	env->CallStaticVoidMethodV(cls, methodID, args);
	va_end(args);
}
INLINE void jniCallStaticVoidMethodV(JNIEnv *env, jclass cls, jmethodID methodID, va_list args) {
	env->CallStaticVoidMethodV(cls, methodID, args);
}
INLINE void jniCallStaticVoidMethodA(JNIEnv *env, jclass cls, jmethodID methodID, const jvalue * args) {
	env->CallStaticVoidMethodA(cls, methodID, args);
}

INLINE jfieldID jniGetStaticFieldID(JNIEnv *env, jclass clazz, const char *name, const char *sig) {
	return env->GetStaticFieldID(clazz, name, sig);
}
INLINE jobject jniGetStaticObjectField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return env->GetStaticObjectField(clazz, fieldID);
}
INLINE jboolean jniGetStaticBooleanField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return env->GetStaticBooleanField(clazz, fieldID);
}
INLINE jbyte jniGetStaticByteField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return env->GetStaticByteField(clazz, fieldID);
}
INLINE jchar jniGetStaticCharField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return env->GetStaticCharField(clazz, fieldID);
}
INLINE jshort jniGetStaticShortField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return env->GetStaticShortField(clazz, fieldID);
}
INLINE jint jniGetStaticIntField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return env->GetStaticIntField(clazz, fieldID);
}
INLINE jlong jniGetStaticLongField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return env->GetStaticLongField(clazz, fieldID);
}
INLINE jfloat jniGetStaticFloatField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return env->GetStaticFloatField(clazz, fieldID);
}
INLINE jdouble jniGetStaticDoubleField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return env->GetStaticDoubleField(clazz, fieldID);
}

INLINE void jniSetStaticObjectField(JNIEnv *env, jclass clazz, jfieldID fieldID, jobject value) {
	env->SetStaticObjectField(clazz, fieldID, value);
}
INLINE void jniSetStaticBooleanField(JNIEnv *env, jclass clazz, jfieldID fieldID, jboolean value) {
	env->SetStaticBooleanField(clazz, fieldID, value);
}
INLINE void jniSetStaticByteField(JNIEnv *env, jclass clazz, jfieldID fieldID, jbyte value) {
	env->SetStaticByteField(clazz, fieldID, value);
}
INLINE void jniSetStaticCharField(JNIEnv *env, jclass clazz, jfieldID fieldID, jchar value) {
	env->SetStaticCharField(clazz, fieldID, value);
}
INLINE void jniSetStaticShortField(JNIEnv *env, jclass clazz, jfieldID fieldID, jshort value) {
	env->SetStaticShortField(clazz, fieldID, value);
}
INLINE void jniSetStaticIntField(JNIEnv *env, jclass clazz, jfieldID fieldID, jint value) {
	env->SetStaticIntField(clazz, fieldID, value);
}
INLINE void jniSetStaticLongField(JNIEnv *env, jclass clazz, jfieldID fieldID, jlong value) {
	env->SetStaticLongField(clazz, fieldID, value);
}
INLINE void jniSetStaticFloatField(JNIEnv *env, jclass clazz, jfieldID fieldID, jfloat value) {
	env->SetStaticFloatField(clazz, fieldID, value);
}
INLINE void jniSetStaticDoubleField(JNIEnv *env, jclass clazz, jfieldID fieldID, jdouble value) {
	env->SetStaticDoubleField(clazz, fieldID, value);
}

INLINE jstring jniNewString(JNIEnv *env, const jchar *unicode, jsize len) {
	return env->NewString(unicode, len);
}
INLINE jsize jniGetStringLength(JNIEnv *env, jstring str) {
	return env->GetStringLength(str);
}
INLINE const jchar *jniGetStringChars(JNIEnv *env, jstring str, jboolean *isCopy) {
	return env->GetStringChars(str, isCopy);
}
INLINE void jniReleaseStringChars(JNIEnv *env, jstring str, const jchar *chars) {
	env->ReleaseStringChars(str, chars);
}

INLINE jstring jniNewStringUTF(JNIEnv *env, const char *utf) {
	return env->NewStringUTF(utf);
}
INLINE jsize jniGetStringUTFLength(JNIEnv *env, jstring str) {
	return env->GetStringUTFLength(str);
}
INLINE const char* jniGetStringUTFChars(JNIEnv *env, jstring str, jboolean *isCopy) {
	return env->GetStringUTFChars(str, isCopy);
}
INLINE void jniReleaseStringUTFChars(JNIEnv *env, jstring str, const char* chars) {
	env->ReleaseStringUTFChars(str, chars);
}

INLINE jsize jniGetArrayLength(JNIEnv *env, jarray array) {
	return env->GetArrayLength(array);
}

/*
 NewObjectArray
 jobjectArray NewObjectArray(JNIEnv *env, jsize length,
 jclass elementClass, jobject initialElement);

 Constructs a new array holding objects in class elementClass. All elements are initially set to initialElement.


 LINKAGE:
 Index 172 in the JNIEnv interface function table.
 PARAMETERS:
 env: the JNI interface pointer.

 length: array size.

 elementClass: array element class.

 initialElement: initialization value.

 RETURNS:
 Returns a Java array object, or NULL if the array cannot be constructed.

 THROWS:
 OutOfMemoryError: if the system runs out of memory.
 */
INLINE jobjectArray jniNewObjectArray(JNIEnv *env, jsize len, jclass clazz, jobject init) {
	return env->NewObjectArray(len, clazz, init);
}
INLINE jobject jniGetObjectArrayElement(JNIEnv *env, jobjectArray array, jsize index) {
	return env->GetObjectArrayElement(array, index);
}
INLINE void jniSetObjectArrayElement(JNIEnv *env, jobjectArray array, jsize index, jobject val) {
	env->SetObjectArrayElement(array, index, val);
}

INLINE jbooleanArray jniNewBooleanArray(JNIEnv *env, jsize len) {
	return env->NewBooleanArray(len);
}
INLINE jbyteArray jniNewByteArray(JNIEnv *env, jsize len) {
	return env->NewByteArray(len);
}
INLINE jcharArray jniNewCharArray(JNIEnv *env, jsize len) {
	return env->NewCharArray(len);
}
INLINE jshortArray jniNewShortArray(JNIEnv *env, jsize len) {
	return env->NewShortArray(len);
}
INLINE jintArray jniNewIntArray(JNIEnv *env, jsize len) {
	return env->NewIntArray(len);
}
INLINE jlongArray jniNewLongArray(JNIEnv *env, jsize len) {
	return env->NewLongArray(len);
}
INLINE jfloatArray jniNewFloatArray(JNIEnv *env, jsize len) {
	return env->NewFloatArray(len);
}
INLINE jdoubleArray jniNewDoubleArray(JNIEnv *env, jsize len) {
	return env->NewDoubleArray(len);
}

INLINE jboolean *jniGetBooleanArrayElements(JNIEnv *env, jbooleanArray array, jboolean *isCopy) {
	return env->GetBooleanArrayElements(array, isCopy);
}
INLINE jbyte *jniGetByteArrayElements(JNIEnv *env, jbyteArray array, jboolean *isCopy) {
	return env->GetByteArrayElements(array, isCopy);
}
INLINE jchar *jniGetCharArrayElements(JNIEnv *env, jcharArray array, jboolean *isCopy) {
	return env->GetCharArrayElements(array, isCopy);
}
INLINE jshort *jniGetShortArrayElements(JNIEnv *env, jshortArray array, jboolean *isCopy) {
	return env->GetShortArrayElements(array, isCopy);
}
INLINE jint *jniGetIntArrayElements(JNIEnv *env, jintArray array, jboolean *isCopy) {
	return env->GetIntArrayElements(array, isCopy);
}
INLINE jlong *jniGetLongArrayElements(JNIEnv *env, jlongArray array, jboolean *isCopy) {
	return env->GetLongArrayElements(array, isCopy);
}
INLINE jfloat *jniGetFloatArrayElements(JNIEnv *env, jfloatArray array, jboolean *isCopy) {
	return env->GetFloatArrayElements(array, isCopy);
}
INLINE jdouble *jniGetDoubleArrayElements(JNIEnv *env, jdoubleArray array, jboolean *isCopy) {
	return env->GetDoubleArrayElements(array, isCopy);
}

INLINE void jniReleaseBooleanArrayElements(JNIEnv *env, jbooleanArray array, jboolean *elems, jint mode) {
	env->ReleaseBooleanArrayElements(array, elems, mode);
}
INLINE void jniReleaseByteArrayElements(JNIEnv *env, jbyteArray array, jbyte *elems, jint mode) {
	env->ReleaseByteArrayElements(array, elems, mode);
}
INLINE void jniReleaseCharArrayElements(JNIEnv *env, jcharArray array, jchar *elems, jint mode) {
	env->ReleaseCharArrayElements(array, elems, mode);
}
INLINE void jniReleaseShortArrayElements(JNIEnv *env, jshortArray array, jshort *elems, jint mode) {
	env->ReleaseShortArrayElements(array, elems, mode);
}
INLINE void jniReleaseIntArrayElements(JNIEnv *env, jintArray array, jint *elems, jint mode) {
	env->ReleaseIntArrayElements(array, elems, mode);
}
INLINE void jniReleaseLongArrayElements(JNIEnv *env, jlongArray array, jlong *elems, jint mode) {
	env->ReleaseLongArrayElements(array, elems, mode);
}
INLINE void jniReleaseFloatArrayElements(JNIEnv *env, jfloatArray array, jfloat *elems, jint mode) {
	env->ReleaseFloatArrayElements(array, elems, mode);
}
INLINE void jniReleaseDoubleArrayElements(JNIEnv *env, jdoubleArray array, jdouble *elems, jint mode) {
	env->ReleaseDoubleArrayElements(array, elems, mode);
}

INLINE void jniGetBooleanArrayRegion(JNIEnv *env, jbooleanArray array, jsize start, jsize len,
		jboolean *buf) {
	env->GetBooleanArrayRegion(array, start, len, buf);
}
INLINE void jniGetByteArrayRegion(JNIEnv *env, jbyteArray array, jsize start, jsize len, jbyte *buf) {
	env->GetByteArrayRegion(array, start, len, buf);
}
INLINE void jniGetCharArrayRegion(JNIEnv *env, jcharArray array, jsize start, jsize len, jchar *buf) {
	env->GetCharArrayRegion(array, start, len, buf);
}
INLINE void jniGetShortArrayRegion(JNIEnv *env, jshortArray array, jsize start, jsize len, jshort *buf) {
	env->GetShortArrayRegion(array, start, len, buf);
}
INLINE void jniGetIntArrayRegion(JNIEnv *env, jintArray array, jsize start, jsize len, jint *buf) {
	env->GetIntArrayRegion(array, start, len, buf);
}
INLINE void jniGetLongArrayRegion(JNIEnv *env, jlongArray array, jsize start, jsize len, jlong *buf) {
	env->GetLongArrayRegion(array, start, len, buf);
}
INLINE void jniGetFloatArrayRegion(JNIEnv *env, jfloatArray array, jsize start, jsize len, jfloat *buf) {
	env->GetFloatArrayRegion(array, start, len, buf);
}
INLINE void jniGetDoubleArrayRegion(JNIEnv *env, jdoubleArray array, jsize start, jsize len, jdouble *buf) {
	env->GetDoubleArrayRegion(array, start, len, buf);
}

INLINE void jniSetBooleanArrayRegion(JNIEnv *env, jbooleanArray array, jsize start, jsize len,
		const jboolean *buf) {
	env->SetBooleanArrayRegion(array, start, len, buf);
}
INLINE void jniSetByteArrayRegion(JNIEnv *env, jbyteArray array, jsize start, jsize len, const jbyte *buf) {
	env->SetByteArrayRegion(array, start, len, buf);
}
INLINE void jniSetCharArrayRegion(JNIEnv *env, jcharArray array, jsize start, jsize len, const jchar *buf) {
	env->SetCharArrayRegion(array, start, len, buf);
}
INLINE void jniSetShortArrayRegion(JNIEnv *env, jshortArray array, jsize start, jsize len,
		const jshort *buf) {
	env->SetShortArrayRegion(array, start, len, buf);
}
INLINE void jniSetIntArrayRegion(JNIEnv *env, jintArray array, jsize start, jsize len, const jint *buf) {
	env->SetIntArrayRegion(array, start, len, buf);
}
INLINE void jniSetLongArrayRegion(JNIEnv *env, jlongArray array, jsize start, jsize len, const jlong *buf) {
	env->SetLongArrayRegion(array, start, len, buf);
}
INLINE void jniSetFloatArrayRegion(JNIEnv *env, jfloatArray array, jsize start, jsize len,
		const jfloat *buf) {
	env->SetFloatArrayRegion(array, start, len, buf);
}
INLINE void jniSetDoubleArrayRegion(JNIEnv *env, jdoubleArray array, jsize start, jsize len,
		const jdouble *buf) {
	env->SetDoubleArrayRegion(array, start, len, buf);
}

INLINE jint jniRegisterNatives(JNIEnv *env, jclass clazz, const JNINativeMethod *methods, jint nMethods) {
	return env->RegisterNatives(clazz, methods, nMethods);
}
INLINE jint jniUnregisterNatives(JNIEnv *env, jclass clazz) {
	return env->UnregisterNatives(clazz);
}

INLINE jint jniMonitorEnter(JNIEnv *env, jobject obj) {
	return env->MonitorEnter(obj);
}
INLINE jint jniMonitorExit(JNIEnv *env, jobject obj) {
	return env->MonitorExit(obj);
}

INLINE jint jniGetJavaVM(JNIEnv *env, JavaVM **vm) {
	return env->GetJavaVM(vm);
}

INLINE void jniGetStringRegion(JNIEnv *env, jstring str, jsize start, jsize len, jchar *buf) {
	env->GetStringRegion(str, start, len, buf);
}
INLINE void jniGetStringUTFRegion(JNIEnv *env, jstring str, jsize start, jsize len, char *buf) {
	env->GetStringUTFRegion(str, start, len, buf);
}

INLINE void *jniGetPrimitiveArrayCritical(JNIEnv *env, jarray array, jboolean *isCopy) {
	return env->GetPrimitiveArrayCritical(array, isCopy);
}
INLINE void jniReleasePrimitiveArrayCritical(JNIEnv *env, jarray array, void *carray, jint mode) {
	env->ReleasePrimitiveArrayCritical(array, carray, mode);
}

INLINE const jchar *jniGetStringCritical(JNIEnv *env, jstring string, jboolean *isCopy) {
	return env->GetStringCritical(string, isCopy);
}
INLINE void jniReleaseStringCritical(JNIEnv *env, jstring string, const jchar *cstring) {
	env->ReleaseStringCritical(string, cstring);
}

INLINE jweak jniNewWeakGlobalRef(JNIEnv *env, jobject obj) {
	return env->NewWeakGlobalRef(obj);
}
INLINE void jniDeleteWeakGlobalRef(JNIEnv *env, jweak ref) {
	env->DeleteWeakGlobalRef(ref);
}

/*
We introduce a convenience function to check for pending exceptions without creating
a local reference to the exception object.
jboolean ExceptionCheck(JNIEnv *env);

Returns JNI_TRUE when there is a pending exception; otherwise, returns JNI_FALSE.

LINKAGE:
Index 228 in the JNIEnv interface function table.
SINCE:
JDK/JRE 1.2
*/
INLINE jboolean jniExceptionCheck(JNIEnv *env) {
	return env->ExceptionCheck();
}

INLINE jobject jniNewDirectByteBuffer(JNIEnv *env, void* address, jlong capacity) {
	return env->NewDirectByteBuffer(address, capacity);
}
INLINE void* jniGetDirectBufferAddress(JNIEnv *env, jobject buf) {
	return env->GetDirectBufferAddress(buf);
}

INLINE jlong jniGetDirectBufferCapacity(JNIEnv *env, jobject buf) {
	return env->GetDirectBufferCapacity(buf);
}
INLINE jobjectRefType jniGetObjectRefType(JNIEnv *env, jobject obj) {
	return env->GetObjectRefType(obj);
}

#else

INLINE jint jniGetVersion(JNIEnv *env) {
	return (*env)->GetVersion(env);
}
INLINE jclass jniDefineClass(JNIEnv *env, const char *name, jobject loader, const jbyte *buf,
		jsize len) {
	return (*env)->DefineClass(env, name, loader, buf, len);
}
INLINE jclass jniFindClass(JNIEnv *env, const char *name) {
	return (*env)->FindClass(env, name);
}
INLINE jmethodID jniFromReflectedMethod(JNIEnv *env, jobject method) {
	return (*env)->FromReflectedMethod(env, method);
}
INLINE jfieldID jniFromReflectedField(JNIEnv *env, jobject field) {
	return (*env)->FromReflectedField(env, field);
}

INLINE jobject jniToReflectedMethod(JNIEnv *env, jclass cls, jmethodID methodID, jboolean isStatic) {
	return (*env)->ToReflectedMethod(env, cls, methodID, isStatic);
}

INLINE jclass jniGetSuperclass(JNIEnv *env, jclass sub) {
	return (*env)->GetSuperclass(env, sub);
}
INLINE jboolean jniIsAssignableFrom(JNIEnv *env, jclass sub, jclass sup) {
	return (*env)->IsAssignableFrom(env, sub, sup);
}

INLINE jobject jniToReflectedField(JNIEnv *env, jclass cls, jfieldID fieldID, jboolean isStatic) {
	return (*env)->ToReflectedField(env, cls, fieldID, isStatic);
}

INLINE jint jniThrow(JNIEnv *env, jthrowable obj) {
	return (*env)->Throw(env, obj);
}
INLINE jint jniThrowNew(JNIEnv *env, jclass clazz, const char *msg) {
	return (*env)->ThrowNew(env, clazz, msg);
}
INLINE jthrowable jniExceptionOccurred(JNIEnv *env) {
	return (*env)->ExceptionOccurred(env);
}
INLINE void jniExceptionDescribe(JNIEnv *env) {
	(*env)->ExceptionDescribe(env);
}
INLINE void jniExceptionClear(JNIEnv *env) {
	(*env)->ExceptionClear(env);
}
INLINE void jniFatalError(JNIEnv *env, const char *msg) {
	(*env)->FatalError(env, msg);
}

INLINE jint jniPushLocalFrame(JNIEnv *env, jint capacity) {
	return (*env)->PushLocalFrame(env, capacity);
}
INLINE jobject jniPopLocalFrame(JNIEnv *env, jobject result) {
	return (*env)->PopLocalFrame(env, result);
}

INLINE jobject jniNewGlobalRef(JNIEnv *env, jobject lobj) {
	return (*env)->NewGlobalRef(env, lobj);
}
INLINE void jniDeleteGlobalRef(JNIEnv *env, jobject gref) {
	(*env)->DeleteGlobalRef(env, gref);
}
INLINE void jniDeleteLocalRef(JNIEnv *env, jobject obj) {
	(*env)->DeleteLocalRef(env, obj);
}

INLINE jboolean jniIsSameObject(JNIEnv *env, jobject obj1, jobject obj2) {
	return (*env)->IsSameObject(env, obj1, obj2);
}

INLINE jobject jniNewLocalRef(JNIEnv *env, jobject ref) {
	return (*env)->NewLocalRef(env, ref);
}
INLINE jint jniEnsureLocalCapacity(JNIEnv *env, jint capacity) {
	return (*env)->EnsureLocalCapacity(env, capacity);
}

INLINE jobject jniAllocObject(JNIEnv *env, jclass clazz) {
	return (*env)->AllocObject(env, clazz);
}
INLINE jobject jniNewObject(JNIEnv *env, jclass clazz, jmethodID methodID, ...) {
	va_list args;
	jobject result;
	va_start(args, methodID);
	result = (*env)->NewObjectV(env, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jobject jniNewObjectV(JNIEnv *env, jclass clazz, jmethodID methodID,
		va_list args) {
	return (*env)->NewObjectV(env, clazz, methodID, args);
}
INLINE jobject jniNewObjectA(JNIEnv *env, jclass clazz, jmethodID methodID,
		const jvalue *args) {
	return (*env)->NewObjectA(env, clazz, methodID, args);
}

INLINE jclass jniGetObjectClass(JNIEnv *env, jobject obj) {
	return (*env)->GetObjectClass(env, obj);
}
INLINE jboolean jniIsInstanceOf(JNIEnv *env, jobject obj, jclass clazz) {
	return (*env)->IsInstanceOf(env, obj, clazz);
}

INLINE jmethodID jniGetMethodID(JNIEnv *env, jclass clazz, const char *name,
		const char *sig) {
	return (*env)->GetMethodID(env, clazz, name, sig);
}

INLINE jobject jniCallObjectMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jobject result;
	va_start(args, methodID);
	result = (*env)->CallObjectMethodV(env, obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jobject jniCallObjectMethodV(JNIEnv *env, jobject obj, jmethodID methodID,
		va_list args) {
	return (*env)->CallObjectMethodV(env, obj, methodID, args);
}
INLINE jobject jniCallObjectMethodA(JNIEnv *env, jobject obj, jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallObjectMethodA(env, obj, methodID, args);
}

INLINE jboolean jniCallBooleanMethod(JNIEnv *env, jobject obj,
		jmethodID methodID, ...) {
	va_list args;
	jboolean result;
	va_start(args, methodID);
	result = (*env)->CallBooleanMethodV(env, obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jboolean jniCallBooleanMethodV(JNIEnv *env, jobject obj, jmethodID methodID,
		va_list args) {
	return (*env)->CallBooleanMethodV(env, obj, methodID, args);
}
INLINE jboolean jniCallBooleanMethodA(JNIEnv *env, jobject obj, jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallBooleanMethodA(env, obj, methodID, args);
}

INLINE jbyte jniCallByteMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jbyte result;
	va_start(args, methodID);
	result = (*env)->CallByteMethodV(env, obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jbyte jniCallByteMethodV(JNIEnv *env, jobject obj, jmethodID methodID,
		va_list args) {
	return (*env)->CallByteMethodV(env, obj, methodID, args);
}
INLINE jbyte jniCallByteMethodA(JNIEnv *env, jobject obj, jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallByteMethodA(env, obj, methodID, args);
}

INLINE jchar jniCallCharMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jchar result;
	va_start(args, methodID);
	result = (*env)->CallCharMethodV(env, obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jchar jniCallCharMethodV(JNIEnv *env, jobject obj, jmethodID methodID,
		va_list args) {
	return (*env)->CallCharMethodV(env, obj, methodID, args);
}
INLINE jchar jniCallCharMethodA(JNIEnv *env, jobject obj, jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallCharMethodA(env, obj, methodID, args);
}

INLINE jshort jniCallShortMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jshort result;
	va_start(args, methodID);
	result = (*env)->CallShortMethodV(env, obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jshort jniCallShortMethodV(JNIEnv *env, jobject obj, jmethodID methodID,
		va_list args) {
	return (*env)->CallShortMethodV(env, obj, methodID, args);
}
INLINE jshort jniCallShortMethodA(JNIEnv *env, jobject obj, jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallShortMethodA(env, obj, methodID, args);
}

INLINE jint jniCallIntMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jint result;
	va_start(args, methodID);
	result = (*env)->CallIntMethodV(env, obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jint jniCallIntMethodV(JNIEnv *env, jobject obj, jmethodID methodID,
		va_list args) {
	return (*env)->CallIntMethodV(env, obj, methodID, args);
}
INLINE jint jniCallIntMethodA(JNIEnv *env, jobject obj, jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallIntMethodA(env, obj, methodID, args);
}

INLINE jlong jniCallLongMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jlong result;
	va_start(args, methodID);
	result = (*env)->CallLongMethodV(env, obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jlong jniCallLongMethodV(JNIEnv *env, jobject obj, jmethodID methodID,
		va_list args) {
	return (*env)->CallLongMethodV(env, obj, methodID, args);
}
INLINE jlong jniCallLongMethodA(JNIEnv *env, jobject obj, jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallLongMethodA(env, obj, methodID, args);
}

INLINE jfloat jniCallFloatMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jfloat result;
	va_start(args, methodID);
	result = (*env)->CallFloatMethodV(env, obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jfloat jniCallFloatMethodV(JNIEnv *env, jobject obj, jmethodID methodID,
		va_list args) {
	return (*env)->CallFloatMethodV(env, obj, methodID, args);
}
INLINE jfloat jniCallFloatMethodA(JNIEnv *env, jobject obj, jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallFloatMethodA(env, obj, methodID, args);
}

INLINE jdouble jniCallDoubleMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	jdouble result;
	va_start(args, methodID);
	result = (*env)->CallDoubleMethodV(env, obj, methodID, args);
	va_end(args);
	return result;
}
INLINE jdouble jniCallDoubleMethodV(JNIEnv *env, jobject obj, jmethodID methodID,
		va_list args) {
	return (*env)->CallDoubleMethodV(env, obj, methodID, args);
}
INLINE jdouble jniCallDoubleMethodA(JNIEnv *env, jobject obj, jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallDoubleMethodA(env, obj, methodID, args);
}

INLINE void jniCallVoidMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...) {
	va_list args;
	va_start(args, methodID);
	(*env)->CallVoidMethodV(env, obj, methodID, args);
	va_end(args);
}
INLINE void jniCallVoidMethodV(JNIEnv *env, jobject obj, jmethodID methodID,
		va_list args) {
	(*env)->CallVoidMethodV(env, obj, methodID, args);
}
INLINE void jniCallVoidMethodA(JNIEnv *env, jobject obj, jmethodID methodID,
		const jvalue * args) {
	(*env)->CallVoidMethodA(env, obj, methodID, args);
}

INLINE jobject jniCallNonvirtualObjectMethod(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jobject result;
	va_start(args, methodID);
	result = (*env)->CallNonvirtualObjectMethodV(env, obj, clazz,
			methodID, args);
	va_end(args);
	return result;
}
INLINE jobject jniCallNonvirtualObjectMethodV(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallNonvirtualObjectMethodV(env, obj, clazz,
			methodID, args);
}
INLINE jobject jniCallNonvirtualObjectMethodA(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, const jvalue * args) {
	return (*env)->CallNonvirtualObjectMethodA(env, obj, clazz,
			methodID, args);
}

INLINE jboolean jniCallNonvirtualBooleanMethod(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jboolean result;
	va_start(args, methodID);
	result = (*env)->CallNonvirtualBooleanMethodV(env, obj, clazz,
			methodID, args);
	va_end(args);
	return result;
}
INLINE jboolean jniCallNonvirtualBooleanMethodV(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallNonvirtualBooleanMethodV(env, obj, clazz,
			methodID, args);
}
INLINE jboolean jniCallNonvirtualBooleanMethodA(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, const jvalue * args) {
	return (*env)->CallNonvirtualBooleanMethodA(env, obj, clazz,
			methodID, args);
}

INLINE jbyte jniCallNonvirtualByteMethod(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jbyte result;
	va_start(args, methodID);
	result = (*env)->CallNonvirtualByteMethodV(env, obj, clazz,
			methodID, args);
	va_end(args);
	return result;
}
INLINE jbyte jniCallNonvirtualByteMethodV(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallNonvirtualByteMethodV(env, obj, clazz,
			methodID, args);
}
INLINE jbyte jniCallNonvirtualByteMethodA(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, const jvalue * args) {
	return (*env)->CallNonvirtualByteMethodA(env, obj, clazz,
			methodID, args);
}

INLINE jchar jniCallNonvirtualCharMethod(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jchar result;
	va_start(args, methodID);
	result = (*env)->CallNonvirtualCharMethodV(env, obj, clazz,
			methodID, args);
	va_end(args);
	return result;
}
INLINE jchar jniCallNonvirtualCharMethodV(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallNonvirtualCharMethodV(env, obj, clazz,
			methodID, args);
}
INLINE jchar jniCallNonvirtualCharMethodA(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, const jvalue * args) {
	return (*env)->CallNonvirtualCharMethodA(env, obj, clazz,
			methodID, args);
}

INLINE jshort jniCallNonvirtualShortMethod(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jshort result;
	va_start(args, methodID);
	result = (*env)->CallNonvirtualShortMethodV(env, obj, clazz,
			methodID, args);
	va_end(args);
	return result;
}
INLINE jshort jniCallNonvirtualShortMethodV(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallNonvirtualShortMethodV(env, obj, clazz,
			methodID, args);
}
INLINE jshort jniCallNonvirtualShortMethodA(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, const jvalue * args) {
	return (*env)->CallNonvirtualShortMethodA(env, obj, clazz,
			methodID, args);
}

INLINE jint jniCallNonvirtualIntMethod(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jint result;
	va_start(args, methodID);
	result = (*env)->CallNonvirtualIntMethodV(env, obj, clazz,
			methodID, args);
	va_end(args);
	return result;
}
INLINE jint jniCallNonvirtualIntMethodV(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallNonvirtualIntMethodV(env, obj, clazz,
			methodID, args);
}
INLINE jint jniCallNonvirtualIntMethodA(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, const jvalue * args) {
	return (*env)->CallNonvirtualIntMethodA(env, obj, clazz,
			methodID, args);
}

INLINE jlong jniCallNonvirtualLongMethod(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jlong result;
	va_start(args, methodID);
	result = (*env)->CallNonvirtualLongMethodV(env, obj, clazz,
			methodID, args);
	va_end(args);
	return result;
}
INLINE jlong jniCallNonvirtualLongMethodV(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallNonvirtualLongMethodV(env, obj, clazz,
			methodID, args);
}
INLINE jlong jniCallNonvirtualLongMethodA(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, const jvalue * args) {
	return (*env)->CallNonvirtualLongMethodA(env, obj, clazz,
			methodID, args);
}

INLINE jfloat jniCallNonvirtualFloatMethod(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jfloat result;
	va_start(args, methodID);
	result = (*env)->CallNonvirtualFloatMethodV(env, obj, clazz,
			methodID, args);
	va_end(args);
	return result;
}
INLINE jfloat jniCallNonvirtualFloatMethodV(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID,
		va_list args) {
	return (*env)->CallNonvirtualFloatMethodV(env, obj, clazz,
			methodID, args);
}
INLINE jfloat jniCallNonvirtualFloatMethodA(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallNonvirtualFloatMethodA(env, obj, clazz,
			methodID, args);
}

INLINE jdouble jniCallNonvirtualDoubleMethod(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jdouble result;
	va_start(args, methodID);
	result = (*env)->CallNonvirtualDoubleMethodV(env, obj, clazz,
			methodID, args);
	va_end(args);
	return result;
}
INLINE jdouble jniCallNonvirtualDoubleMethodV(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID,
		va_list args) {
	return (*env)->CallNonvirtualDoubleMethodV(env, obj, clazz,
			methodID, args);
}
INLINE jdouble jniCallNonvirtualDoubleMethodA(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID,
		const jvalue * args) {
	return (*env)->CallNonvirtualDoubleMethodA(env, obj, clazz,
			methodID, args);
}

INLINE void jniCallNonvirtualVoidMethod(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	va_start(args, methodID);
	(*env)->CallNonvirtualVoidMethodV(env, obj, clazz, methodID, args);
	va_end(args);
}
INLINE void jniCallNonvirtualVoidMethodV(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID,
		va_list args) {
	(*env)->CallNonvirtualVoidMethodV(env, obj, clazz, methodID, args);
}
INLINE void jniCallNonvirtualVoidMethodA(JNIEnv *env, jobject obj, jclass clazz,
		jmethodID methodID,
		const jvalue * args) {
	(*env)->CallNonvirtualVoidMethodA(env, obj, clazz, methodID, args);
}

INLINE jfieldID jniGetFieldID(JNIEnv *env, jclass clazz, const char *name,
		const char *sig) {
	return (*env)->GetFieldID(env, clazz, name, sig);
}

INLINE jobject jniGetObjectField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return (*env)->GetObjectField(env, obj, fieldID);
}
INLINE jboolean jniGetBooleanField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return (*env)->GetBooleanField(env, obj, fieldID);
}
INLINE jbyte jniGetByteField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return (*env)->GetByteField(env, obj, fieldID);
}
INLINE jchar jniGetCharField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return (*env)->GetCharField(env, obj, fieldID);
}
INLINE jshort jniGetShortField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return (*env)->GetShortField(env, obj, fieldID);
}
INLINE jint jniGetIntField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return (*env)->GetIntField(env, obj, fieldID);
}
INLINE jlong jniGetLongField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return (*env)->GetLongField(env, obj, fieldID);
}
INLINE jfloat jniGetFloatField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return (*env)->GetFloatField(env, obj, fieldID);
}
INLINE jdouble jniGetDoubleField(JNIEnv *env, jobject obj, jfieldID fieldID) {
	return (*env)->GetDoubleField(env, obj, fieldID);
}

INLINE void jniSetObjectField(JNIEnv *env, jobject obj, jfieldID fieldID, jobject val) {
	(*env)->SetObjectField(env, obj, fieldID, val);
}
INLINE void jniSetBooleanField(JNIEnv *env, jobject obj, jfieldID fieldID,
		jboolean val) {
	(*env)->SetBooleanField(env, obj, fieldID, val);
}
INLINE void jniSetByteField(JNIEnv *env, jobject obj, jfieldID fieldID,
		jbyte val) {
	(*env)->SetByteField(env, obj, fieldID, val);
}
INLINE void jniSetCharField(JNIEnv *env, jobject obj, jfieldID fieldID,
		jchar val) {
	(*env)->SetCharField(env, obj, fieldID, val);
}
INLINE void jniSetShortField(JNIEnv *env, jobject obj, jfieldID fieldID,
		jshort val) {
	(*env)->SetShortField(env, obj, fieldID, val);
}
INLINE void jniSetIntField(JNIEnv *env, jobject obj, jfieldID fieldID,
		jint val) {
	(*env)->SetIntField(env, obj, fieldID, val);
}
INLINE void jniSetLongField(JNIEnv *env, jobject obj, jfieldID fieldID,
		jlong val) {
	(*env)->SetLongField(env, obj, fieldID, val);
}
INLINE void jniSetFloatField(JNIEnv *env, jobject obj, jfieldID fieldID,
		jfloat val) {
	(*env)->SetFloatField(env, obj, fieldID, val);
}
INLINE void jniSetDoubleField(JNIEnv *env, jobject obj, jfieldID fieldID,
		jdouble val) {
	(*env)->SetDoubleField(env, obj, fieldID, val);
}

INLINE jmethodID jniGetStaticMethodID(JNIEnv *env, jclass clazz, const char *name,
		const char *sig) {
	return (*env)->GetStaticMethodID(env, clazz, name, sig);
}

INLINE jobject jniCallStaticObjectMethod(JNIEnv *env, jclass clazz, jmethodID methodID,
		...) {
	va_list args;
	jobject result;
	va_start(args, methodID);
	result = (*env)->CallStaticObjectMethodV(env, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jobject jniCallStaticObjectMethodV(JNIEnv *env, jclass clazz, jmethodID methodID,
		va_list args) {
	return (*env)->CallStaticObjectMethodV(env, clazz, methodID, args);
}
INLINE jobject jniCallStaticObjectMethodA(JNIEnv *env, jclass clazz, jmethodID methodID,
		const jvalue *args) {
	return (*env)->CallStaticObjectMethodA(env, clazz, methodID, args);
}

INLINE jboolean jniCallStaticBooleanMethod(JNIEnv *env, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jboolean result;
	va_start(args, methodID);
	result = (*env)->CallStaticBooleanMethodV(env, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jboolean jniCallStaticBooleanMethodV(JNIEnv *env, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallStaticBooleanMethodV(env, clazz, methodID, args);
}
INLINE jboolean jniCallStaticBooleanMethodA(JNIEnv *env, jclass clazz,
		jmethodID methodID, const jvalue *args) {
	return (*env)->CallStaticBooleanMethodA(env, clazz, methodID, args);
}

INLINE jbyte jniCallStaticByteMethod(JNIEnv *env, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jbyte result;
	va_start(args, methodID);
	result = (*env)->CallStaticByteMethodV(env, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jbyte jniCallStaticByteMethodV(JNIEnv *env, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallStaticByteMethodV(env, clazz, methodID, args);
}
INLINE jbyte jniCallStaticByteMethodA(JNIEnv *env, jclass clazz,
		jmethodID methodID, const jvalue *args) {
	return (*env)->CallStaticByteMethodA(env, clazz, methodID, args);
}

INLINE jchar jniCallStaticCharMethod(JNIEnv *env, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jchar result;
	va_start(args, methodID);
	result = (*env)->CallStaticCharMethodV(env, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jchar jniCallStaticCharMethodV(JNIEnv *env, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallStaticCharMethodV(env, clazz, methodID, args);
}
INLINE jchar jniCallStaticCharMethodA(JNIEnv *env, jclass clazz,
		jmethodID methodID, const jvalue *args) {
	return (*env)->CallStaticCharMethodA(env, clazz, methodID, args);
}

INLINE jshort jniCallStaticShortMethod(JNIEnv *env, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jshort result;
	va_start(args, methodID);
	result = (*env)->CallStaticShortMethodV(env, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jshort jniCallStaticShortMethodV(JNIEnv *env, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallStaticShortMethodV(env, clazz, methodID, args);
}
INLINE jshort jniCallStaticShortMethodA(JNIEnv *env, jclass clazz,
		jmethodID methodID, const jvalue *args) {
	return (*env)->CallStaticShortMethodA(env, clazz, methodID, args);
}

INLINE jint jniCallStaticIntMethod(JNIEnv *env, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jint result;
	va_start(args, methodID);
	result = (*env)->CallStaticIntMethodV(env, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jint jniCallStaticIntMethodV(JNIEnv *env, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallStaticIntMethodV(env, clazz, methodID, args);
}
INLINE jint jniCallStaticIntMethodA(JNIEnv *env, jclass clazz,
		jmethodID methodID, const jvalue *args) {
	return (*env)->CallStaticIntMethodA(env, clazz, methodID, args);
}

INLINE jlong jniCallStaticLongMethod(JNIEnv *env, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jlong result;
	va_start(args, methodID);
	result = (*env)->CallStaticLongMethodV(env, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jlong jniCallStaticLongMethodV(JNIEnv *env, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallStaticLongMethodV(env, clazz, methodID, args);
}
INLINE jlong jniCallStaticLongMethodA(JNIEnv *env, jclass clazz,
		jmethodID methodID, const jvalue *args) {
	return (*env)->CallStaticLongMethodA(env, clazz, methodID, args);
}

INLINE jfloat jniCallStaticFloatMethod(JNIEnv *env, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jfloat result;
	va_start(args, methodID);
	result = (*env)->CallStaticFloatMethodV(env, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jfloat jniCallStaticFloatMethodV(JNIEnv *env, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallStaticFloatMethodV(env, clazz, methodID, args);
}
INLINE jfloat jniCallStaticFloatMethodA(JNIEnv *env, jclass clazz,
		jmethodID methodID, const jvalue *args) {
	return (*env)->CallStaticFloatMethodA(env, clazz, methodID, args);
}

INLINE jdouble jniCallStaticDoubleMethod(JNIEnv *env, jclass clazz,
		jmethodID methodID, ...) {
	va_list args;
	jdouble result;
	va_start(args, methodID);
	result = (*env)->CallStaticDoubleMethodV(env, clazz, methodID, args);
	va_end(args);
	return result;
}
INLINE jdouble jniCallStaticDoubleMethodV(JNIEnv *env, jclass clazz,
		jmethodID methodID, va_list args) {
	return (*env)->CallStaticDoubleMethodV(env, clazz, methodID, args);
}
INLINE jdouble jniCallStaticDoubleMethodA(JNIEnv *env, jclass clazz,
		jmethodID methodID, const jvalue *args) {
	return (*env)->CallStaticDoubleMethodA(env, clazz, methodID, args);
}

INLINE void jniCallStaticVoidMethod(JNIEnv *env, jclass cls, jmethodID methodID, ...) {
	va_list args;
	va_start(args, methodID);
	(*env)->CallStaticVoidMethodV(env, cls, methodID, args);
	va_end(args);
}
INLINE void jniCallStaticVoidMethodV(JNIEnv *env, jclass cls, jmethodID methodID,
		va_list args) {
	(*env)->CallStaticVoidMethodV(env, cls, methodID, args);
}
INLINE void jniCallStaticVoidMethodA(JNIEnv *env, jclass cls, jmethodID methodID,
		const jvalue * args) {
	(*env)->CallStaticVoidMethodA(env, cls, methodID, args);
}

INLINE jfieldID jniGetStaticFieldID(JNIEnv *env, jclass clazz, const char *name,
		const char *sig) {
	return (*env)->GetStaticFieldID(env, clazz, name, sig);
}
INLINE jobject jniGetStaticObjectField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return (*env)->GetStaticObjectField(env, clazz, fieldID);
}
INLINE jboolean jniGetStaticBooleanField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return (*env)->GetStaticBooleanField(env, clazz, fieldID);
}
INLINE jbyte jniGetStaticByteField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return (*env)->GetStaticByteField(env, clazz, fieldID);
}
INLINE jchar jniGetStaticCharField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return (*env)->GetStaticCharField(env, clazz, fieldID);
}
INLINE jshort jniGetStaticShortField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return (*env)->GetStaticShortField(env, clazz, fieldID);
}
INLINE jint jniGetStaticIntField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return (*env)->GetStaticIntField(env, clazz, fieldID);
}
INLINE jlong jniGetStaticLongField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return (*env)->GetStaticLongField(env, clazz, fieldID);
}
INLINE jfloat jniGetStaticFloatField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return (*env)->GetStaticFloatField(env, clazz, fieldID);
}
INLINE jdouble jniGetStaticDoubleField(JNIEnv *env, jclass clazz, jfieldID fieldID) {
	return (*env)->GetStaticDoubleField(env, clazz, fieldID);
}

INLINE void jniSetStaticObjectField(JNIEnv *env, jclass clazz, jfieldID fieldID,
		jobject value) {
	(*env)->SetStaticObjectField(env, clazz, fieldID, value);
}
INLINE void jniSetStaticBooleanField(JNIEnv *env, jclass clazz, jfieldID fieldID,
		jboolean value) {
	(*env)->SetStaticBooleanField(env, clazz, fieldID, value);
}
INLINE void jniSetStaticByteField(JNIEnv *env, jclass clazz, jfieldID fieldID,
		jbyte value) {
	(*env)->SetStaticByteField(env, clazz, fieldID, value);
}
INLINE void jniSetStaticCharField(JNIEnv *env, jclass clazz, jfieldID fieldID,
		jchar value) {
	(*env)->SetStaticCharField(env, clazz, fieldID, value);
}
INLINE void jniSetStaticShortField(JNIEnv *env, jclass clazz, jfieldID fieldID,
		jshort value) {
	(*env)->SetStaticShortField(env, clazz, fieldID, value);
}
INLINE void jniSetStaticIntField(JNIEnv *env, jclass clazz, jfieldID fieldID,
		jint value) {
	(*env)->SetStaticIntField(env, clazz, fieldID, value);
}
INLINE void jniSetStaticLongField(JNIEnv *env, jclass clazz, jfieldID fieldID,
		jlong value) {
	(*env)->SetStaticLongField(env, clazz, fieldID, value);
}
INLINE void jniSetStaticFloatField(JNIEnv *env, jclass clazz, jfieldID fieldID,
		jfloat value) {
	(*env)->SetStaticFloatField(env, clazz, fieldID, value);
}
INLINE void jniSetStaticDoubleField(JNIEnv *env, jclass clazz, jfieldID fieldID,
		jdouble value) {
	(*env)->SetStaticDoubleField(env, clazz, fieldID, value);
}

INLINE jstring jniNewString(JNIEnv *env, const jchar *unicode, jsize len) {
	return (*env)->NewString(env, unicode, len);
}
INLINE jsize jniGetStringLength(JNIEnv *env, jstring str) {
	return (*env)->GetStringLength(env, str);
}
INLINE const jchar *jniGetStringChars(JNIEnv *env, jstring str, jboolean *isCopy) {
	return (*env)->GetStringChars(env, str, isCopy);
}
INLINE void jniReleaseStringChars(JNIEnv *env, jstring str, const jchar *chars) {
	(*env)->ReleaseStringChars(env, str, chars);
}

INLINE jstring jniNewStringUTF(JNIEnv *env, const char *utf) {
	return (*env)->NewStringUTF(env, utf);
}
INLINE jsize jniGetStringUTFLength(JNIEnv *env, jstring str) {
	return (*env)->GetStringUTFLength(env, str);
}
const char* jniGetStringUTFChars(JNIEnv *env, jstring str, jboolean *isCopy) {
	return (*env)->GetStringUTFChars(env, str, isCopy);
}
INLINE void jniReleaseStringUTFChars(JNIEnv *env, jstring str, const char* chars) {
	(*env)->ReleaseStringUTFChars(env, str, chars);
}

INLINE jsize jniGetArrayLength(JNIEnv *env, jarray array) {
	return (*env)->GetArrayLength(env, array);
}

INLINE jobjectArray jniNewObjectArray(JNIEnv *env, jsize len, jclass clazz,
		jobject init) {
	return (*env)->NewObjectArray(env, len, clazz, init);
}
INLINE jobject jniGetObjectArrayElement(JNIEnv *env, jobjectArray array, jsize index) {
	return (*env)->GetObjectArrayElement(env, array, index);
}
INLINE void jniSetObjectArrayElement(JNIEnv *env, jobjectArray array, jsize index,
		jobject val) {
	(*env)->SetObjectArrayElement(env, array, index, val);
}

INLINE jbooleanArray jniNewBooleanArray(JNIEnv *env, jsize len) {
	return (*env)->NewBooleanArray(env, len);
}
INLINE jbyteArray jniNewByteArray(JNIEnv *env, jsize len) {
	return (*env)->NewByteArray(env, len);
}
INLINE jcharArray jniNewCharArray(JNIEnv *env, jsize len) {
	return (*env)->NewCharArray(env, len);
}
INLINE jshortArray jniNewShortArray(JNIEnv *env, jsize len) {
	return (*env)->NewShortArray(env, len);
}
INLINE jintArray jniNewIntArray(JNIEnv *env, jsize len) {
	return (*env)->NewIntArray(env, len);
}
INLINE jlongArray jniNewLongArray(JNIEnv *env, jsize len) {
	return (*env)->NewLongArray(env, len);
}
INLINE jfloatArray jniNewFloatArray(JNIEnv *env, jsize len) {
	return (*env)->NewFloatArray(env, len);
}
INLINE jdoubleArray jniNewDoubleArray(JNIEnv *env, jsize len) {
	return (*env)->NewDoubleArray(env, len);
}

INLINE jboolean *jniGetBooleanArrayElements(JNIEnv *env, jbooleanArray array, jboolean *isCopy) {
	return (*env)->GetBooleanArrayElements(env, array, isCopy);
}
INLINE jbyte *jniGetByteArrayElements(JNIEnv *env, jbyteArray array, jboolean *isCopy) {
	return (*env)->GetByteArrayElements(env, array, isCopy);
}
INLINE jchar *jniGetCharArrayElements(JNIEnv *env, jcharArray array, jboolean *isCopy) {
	return (*env)->GetCharArrayElements(env, array, isCopy);
}
INLINE jshort *jniGetShortArrayElements(JNIEnv *env, jshortArray array, jboolean *isCopy) {
	return (*env)->GetShortArrayElements(env, array, isCopy);
}
INLINE jint *jniGetIntArrayElements(JNIEnv *env, jintArray array, jboolean *isCopy) {
	return (*env)->GetIntArrayElements(env, array, isCopy);
}
INLINE jlong *jniGetLongArrayElements(JNIEnv *env, jlongArray array, jboolean *isCopy) {
	return (*env)->GetLongArrayElements(env, array, isCopy);
}
INLINE jfloat *jniGetFloatArrayElements(JNIEnv *env, jfloatArray array, jboolean *isCopy) {
	return (*env)->GetFloatArrayElements(env, array, isCopy);
}
INLINE jdouble *jniGetDoubleArrayElements(JNIEnv *env, jdoubleArray array, jboolean *isCopy) {
	return (*env)->GetDoubleArrayElements(env, array, isCopy);
}

INLINE void jniReleaseBooleanArrayElements(JNIEnv *env, jbooleanArray array,
		jboolean *elems,
		jint mode) {
	(*env)->ReleaseBooleanArrayElements(env, array, elems, mode);
}
INLINE void jniReleaseByteArrayElements(JNIEnv *env, jbyteArray array,
		jbyte *elems,
		jint mode) {
	(*env)->ReleaseByteArrayElements(env, array, elems, mode);
}
INLINE void jniReleaseCharArrayElements(JNIEnv *env, jcharArray array,
		jchar *elems,
		jint mode) {
	(*env)->ReleaseCharArrayElements(env, array, elems, mode);
}
INLINE void jniReleaseShortArrayElements(JNIEnv *env, jshortArray array,
		jshort *elems,
		jint mode) {
	(*env)->ReleaseShortArrayElements(env, array, elems, mode);
}
INLINE void jniReleaseIntArrayElements(JNIEnv *env, jintArray array,
		jint *elems,
		jint mode) {
	(*env)->ReleaseIntArrayElements(env, array, elems, mode);
}
INLINE void jniReleaseLongArrayElements(JNIEnv *env, jlongArray array,
		jlong *elems,
		jint mode) {
	(*env)->ReleaseLongArrayElements(env, array, elems, mode);
}
INLINE void jniReleaseFloatArrayElements(JNIEnv *env, jfloatArray array,
		jfloat *elems,
		jint mode) {
	(*env)->ReleaseFloatArrayElements(env, array, elems, mode);
}
INLINE void jniReleaseDoubleArrayElements(JNIEnv *env, jdoubleArray array,
		jdouble *elems,
		jint mode) {
	(*env)->ReleaseDoubleArrayElements(env, array, elems, mode);
}

INLINE void jniGetBooleanArrayRegion(JNIEnv *env, jbooleanArray array,
		jsize start, jsize len, jboolean *buf) {
	(*env)->GetBooleanArrayRegion(env, array, start, len, buf);
}
INLINE void jniGetByteArrayRegion(JNIEnv *env, jbyteArray array,
		jsize start, jsize len, jbyte *buf) {
	(*env)->GetByteArrayRegion(env, array, start, len, buf);
}
INLINE void jniGetCharArrayRegion(JNIEnv *env, jcharArray array,
		jsize start, jsize len, jchar *buf) {
	(*env)->GetCharArrayRegion(env, array, start, len, buf);
}
INLINE void jniGetShortArrayRegion(JNIEnv *env, jshortArray array,
		jsize start, jsize len, jshort *buf) {
	(*env)->GetShortArrayRegion(env, array, start, len, buf);
}
INLINE void jniGetIntArrayRegion(JNIEnv *env, jintArray array,
		jsize start, jsize len, jint *buf) {
	(*env)->GetIntArrayRegion(env, array, start, len, buf);
}
INLINE void jniGetLongArrayRegion(JNIEnv *env, jlongArray array,
		jsize start, jsize len, jlong *buf) {
	(*env)->GetLongArrayRegion(env, array, start, len, buf);
}
INLINE void jniGetFloatArrayRegion(JNIEnv *env, jfloatArray array,
		jsize start, jsize len, jfloat *buf) {
	(*env)->GetFloatArrayRegion(env, array, start, len, buf);
}
INLINE void jniGetDoubleArrayRegion(JNIEnv *env, jdoubleArray array,
		jsize start, jsize len, jdouble *buf) {
	(*env)->GetDoubleArrayRegion(env, array, start, len, buf);
}

INLINE void jniSetBooleanArrayRegion(JNIEnv *env, jbooleanArray array, jsize start, jsize len,
		const jboolean *buf) {
	(*env)->SetBooleanArrayRegion(env, array, start, len, buf);
}
INLINE void jniSetByteArrayRegion(JNIEnv *env, jbyteArray array, jsize start, jsize len,
		const jbyte *buf) {
	(*env)->SetByteArrayRegion(env, array, start, len, buf);
}
INLINE void jniSetCharArrayRegion(JNIEnv *env, jcharArray array, jsize start, jsize len,
		const jchar *buf) {
	(*env)->SetCharArrayRegion(env, array, start, len, buf);
}
INLINE void jniSetShortArrayRegion(JNIEnv *env, jshortArray array, jsize start, jsize len,
		const jshort *buf) {
	(*env)->SetShortArrayRegion(env, array, start, len, buf);
}
INLINE void jniSetIntArrayRegion(JNIEnv *env, jintArray array, jsize start, jsize len,
		const jint *buf) {
	(*env)->SetIntArrayRegion(env, array, start, len, buf);
}
INLINE void jniSetLongArrayRegion(JNIEnv *env, jlongArray array, jsize start, jsize len,
		const jlong *buf) {
	(*env)->SetLongArrayRegion(env, array, start, len, buf);
}
INLINE void jniSetFloatArrayRegion(JNIEnv *env, jfloatArray array, jsize start, jsize len,
		const jfloat *buf) {
	(*env)->SetFloatArrayRegion(env, array, start, len, buf);
}
INLINE void jniSetDoubleArrayRegion(JNIEnv *env, jdoubleArray array, jsize start, jsize len,
		const jdouble *buf) {
	(*env)->SetDoubleArrayRegion(env, array, start, len, buf);
}

INLINE jint jniRegisterNatives(JNIEnv *env, jclass clazz, const JNINativeMethod *methods,
		jint nMethods) {
	return (*env)->RegisterNatives(env, clazz, methods, nMethods);
}
INLINE jint jniUnregisterNatives(JNIEnv *env, jclass clazz) {
	return (*env)->UnregisterNatives(env, clazz);
}

INLINE jint jniMonitorEnter(JNIEnv *env, jobject obj) {
	return (*env)->MonitorEnter(env, obj);
}
INLINE jint jniMonitorExit(JNIEnv *env, jobject obj) {
	return (*env)->MonitorExit(env, obj);
}

INLINE jint jniGetJavaVM(JNIEnv *env, JavaVM **vm) {
	return (*env)->GetJavaVM(env, vm);
}

INLINE void jniGetStringRegion(JNIEnv *env, jstring str, jsize start, jsize len, jchar *buf) {
	(*env)->GetStringRegion(env, str, start, len, buf);
}
INLINE void jniGetStringUTFRegion(JNIEnv *env, jstring str, jsize start, jsize len, char *buf) {
	(*env)->GetStringUTFRegion(env, str, start, len, buf);
}

INLINE void *jniGetPrimitiveArrayCritical(JNIEnv *env, jarray array, jboolean *isCopy) {
	return (*env)->GetPrimitiveArrayCritical(env, array, isCopy);
}
INLINE void jniReleasePrimitiveArrayCritical(JNIEnv *env, jarray array, void *carray, jint mode) {
	(*env)->ReleasePrimitiveArrayCritical(env, array, carray, mode);
}

INLINE const jchar *jniGetStringCritical(JNIEnv *env, jstring string, jboolean *isCopy) {
	return (*env)->GetStringCritical(env, string, isCopy);
}
INLINE void jniReleaseStringCritical(JNIEnv *env, jstring string, const jchar *cstring) {
	(*env)->ReleaseStringCritical(env, string, cstring);
}

INLINE jweak jniNewWeakGlobalRef(JNIEnv *env, jobject obj) {
	return (*env)->NewWeakGlobalRef(env, obj);
}
INLINE void jniDeleteWeakGlobalRef(JNIEnv *env, jweak ref) {
	(*env)->DeleteWeakGlobalRef(env, ref);
}

INLINE jboolean jniExceptionCheck(JNIEnv *env) {
	return (*env)->ExceptionCheck(env);
}

INLINE jobject jniNewDirectByteBuffer(JNIEnv *env, void* address, jlong capacity) {
	return (*env)->NewDirectByteBuffer(env, address, capacity);
}
void* GetDirectBufferAddress(JNIEnv *env, jobject buf) {
	return (*env)->GetDirectBufferAddress(env, buf);
}
INLINE jlong jniGetDirectBufferCapacity(JNIEnv *env, jobject buf) {
	return (*env)->GetDirectBufferCapacity(env, buf);
}
INLINE jobjectRefType jniGetObjectRefType(JNIEnv *env, jobject obj) {
	return (*env)->GetObjectRefType(env, obj);
}
#endif

#endif

#define MSG_SIZE 1024

// Avoid typos in class names
#define EIllegalArgument "java/lang/IllegalArgumentException"
#define EOutOfMemory "java/lang/OutOfMemoryError"
#define EUnsatisfiedLink "java/lang/UnsatisfiedLinkError"
#define EIllegalState "java/lang/IllegalStateException"
#define EUnsupportedOperation "java/lang/UnsupportedOperationException"
#define ERuntime "java/lang/RuntimeException"
#define EError "java/lang/Error"
