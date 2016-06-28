#include "jn.h"
#include "jni.h"

//http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/functions.html#registering_native_methods
JNIEXPORT jint JNICALL Java_ss_Jn_registerNativeMethodStub(JNIEnv *env, jclass cls, jobject method,
		jclass clazz, jstring name, jstring signature, jint argsize) {

	const char* utfName = jniGetStringUTFChars(env, name, 0);
	const char* utfSignature = jniGetStringUTFChars(env, signature, 0);
	// TODO release stubthunk *stub
	stubthunk *stub = (stubthunk*) alloc_code(sizeof(stubthunk));
	stubthunk_init(stub, (mid_t) (intptr_t) argsize);

	JNINativeMethod methods = { CONST_CAST(char*, utfName), CONST_CAST(char*, utfSignature), stub };

	jint retval = jniRegisterNatives(env, clazz, &methods, 1);

	jniReleaseStringUTFChars(env, name, utfName);
	jniReleaseStringUTFChars(env, signature, utfSignature);
	return retval;
}

class CRaw {
private:
	void *p;
	template<typename T> static void moveptr(T **pp, unsigned int stride) {
		*pp = (T*) ((char*) *pp + stride);
		// p = (void*)((char*)p + stride);
	}
	template<typename T> void advance() {
		moveptr(&p, sizeof(T));
	}

	void advance(size_t length) {
		moveptr(&p, length);
	}

	template<typename T> void write(T v) {
		*(T *) p = v;
		advance<T>();
	}

	template<typename T> T read() {
		T *pc = (T*) p;
		advance<T>();
		return *pc;
	}

public:
	CRaw(void *p) {
		this->p = p;
	}

	void MarshalToJObject(JNIEnv *env, StMeta *meta, jobject jobj) {
		asserts(NULL != jobj);
		if (UNLIKELY(NULL == jobj)) {
			return;
		}
		//D_PRINT("StMeta: len = %d\n", meta->len);
		jobject joMember;
		jfieldID* pMemberFieldId = (jfieldID*) meta->mem;
		char *pmeta = meta->mem + meta->len * sizeof(jfieldID);
		for (int idx = 0; idx < meta->len; idx++) {
			char jsig = ((primitivetype*) pmeta)->sig;
			// D_PRINT("Field[%d]: sig = %c\n", idx, jsig);
			switch (jsig) {
#define SET_FIELD_VALUE(NAME) jniSet##NAME##Field(env, jobj, pMemberFieldId[idx], read<J##NAME>())
			case 'Z': SET_FIELD_VALUE(Boolean); break;
			case 'B': SET_FIELD_VALUE(Byte); break;
			case 'C': SET_FIELD_VALUE(Char); break;
			case 'S': SET_FIELD_VALUE(Short); break;
			case 'I': SET_FIELD_VALUE(Int); break;
			case 'J': SET_FIELD_VALUE(Long); break;
			case 'F': SET_FIELD_VALUE(Float); break;
			case 'D': SET_FIELD_VALUE(Double); break;
#undef SET_FIELD_VALUE
			case 't':
				//Constructs a new java.lang.String object from an array of characters in modified UTF-8 encoding.
				//Returns a Java string object, or NULL if the string cannot be constructed.
				//OutOfMemoryError: if the system runs out of memory.
				//TODO
				joMember = jniNewStringUTF(env, (char*) p);
				jniSetObjectField(env, jobj, pMemberFieldId[idx], joMember);
				advance(((plaintype*) pmeta)->size);
				break;
			case 'T':
				joMember = jniNewString(env, (jchar*) p, ((plaintype*) pmeta)->size / sizeof(jchar));
				jniSetObjectField(env, jobj, pMemberFieldId[idx], joMember);
				advance(((plaintype*) pmeta)->size);
				break;
			case '[': {
				ASSERT_UNSUPPORTED();
				joMember = jniGetObjectField(env, jobj, pMemberFieldId[idx]);
				if (1 == ((arraytype*) pmeta)->ndims) {
					switch (((arraytype*) pmeta)->elementsig) {
					case 'Z':
						break;
					default:
						break;
					}
				}
			}
				break;
			case 'L': {
				joMember = jniGetObjectField(env, jobj, pMemberFieldId[idx]);
				if (NULL == joMember) {
					asserts(0, "NullException,BufferTooSmallException");
					return;
				}
				CRaw(this).MarshalToJObject(env, ((objecttype*) pmeta)->hmeta, joMember);
			}
				break;
			case '?':
				ASSERT_UNSUPPORTED();
				break;
			default:
				break;
			}
		}
	}

	/**
	 * @param jobj needs to be marshaled into a native buffer
	 */
	void MarshalFromJObject(JNIEnv *env, StMeta *meta, const jobject jobj) {
		asserts(NULL != jobj);
		if (UNLIKELY(NULL == jobj)) {
			return;
		}
		jobject joMember;
		jfieldID* pMemberFieldId = (jfieldID*) meta->mem;
		char *pmeta = meta->mem + meta->len * sizeof(jfieldID);
		for (int idx = 0; idx < meta->len; idx++) {
			char jsig = ((primitivetype*) pmeta)->sig;
			switch (jsig) {
			case 'Z': write(jniGetBooleanField(env, jobj, pMemberFieldId[idx])); break;
			case 'B': write(jniGetByteField(env, jobj, pMemberFieldId[idx])); break;
			case 'C': write(jniGetCharField(env, jobj, pMemberFieldId[idx])); break;
			case 'S': write(jniGetShortField(env, jobj, pMemberFieldId[idx])); break;
			case 'I': write(jniGetIntField(env, jobj, pMemberFieldId[idx])); break;
			case 'J': write(jniGetLongField(env, jobj, pMemberFieldId[idx])); break;
			case 'F': write(jniGetFloatField(env, jobj, pMemberFieldId[idx])); break;
			case 'D': write(jniGetDoubleField(env, jobj, pMemberFieldId[idx])); break;
			case '[': {
				ASSERT_UNSUPPORTED();
				joMember = jniGetObjectField(env, jobj, pMemberFieldId[idx]);
				if (1 == ((arraytype*) pmeta)->ndims) {
					switch (((arraytype*) pmeta)->elementsig) {
					case 'Z':
						break;
					default:
						break;
					}
				}

			}
				break;
			case 'L': {
				joMember = jniGetObjectField(env, jobj, pMemberFieldId[idx]);
				CRaw(this).MarshalFromJObject(env, ((objecttype*) pmeta)->hmeta, joMember);
			}
				break;
			case '?':
				ASSERT_UNSUPPORTED();
				break;
			default:
				break;
			}
		}
	}
};
