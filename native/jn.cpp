#include "jn.h"
#include "jni.h"

//http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/functions.html#registering_native_methods
JNIEXPORT jint JNICALL Java_ss_Jn_registerNativeMethodStub(JNIEnv *env, jclass cls, jobject method, jclass clazz,
		jstring name, jstring signature, jint argsize) {

	const char* utfName = jniGetStringUTFChars(env, name, 0);
	const char* utfSignature = jniGetStringUTFChars(env, signature, 0);

	// TODO release stubthunk *stub
	stubthunk *stub = (stubthunk*) alloc_code(sizeof(stubthunk));
	stubthunk_init(stub, (mid_t)(intptr_t) argsize);

	JNINativeMethod methods = { CONST_CAST(char*, utfName), CONST_CAST(char*, utfSignature), stub };

	jint retval = jniRegisterNatives(env, clazz, &methods, 1);

	jniReleaseStringUTFChars(env, name, utfName);
	jniReleaseStringUTFChars(env, signature, utfSignature);

	return retval;
}
