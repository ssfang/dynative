#include "jn.h"
#include "jni.h"

extern "C" {
jclass C_ss_Jn;
jmethodID M_ss_Jn_resolveNativeMethod;
}

//http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/functions.html#registering_native_methods
JNIEXPORT jint JNICALL Java_ss_Jn_registerNativeMethodStub(JNIEnv *env, jclass cls, jobject method,
		jclass clazz, jstring name, jstring signature) {

	const char* utfName = jniGetStringUTFChars(env, name, 0);
	const char* utfSignature = jniGetStringUTFChars(env, signature, 0);
	// TODO release stubthunk *stub
	stubthunk *stub = (stubthunk*) alloc_code(sizeof(stubthunk));
	jmethodID mid = jniFromReflectedMethod(env, method);
	nativetrace("Java_ss_Jn_registerNativeMethodStub for jmethodID = %p\n", mid);
	stubthunk_init(stub, mid);

	JNINativeMethod methods = { CONST_CAST(char*, utfName), CONST_CAST(char*, utfSignature), stub };

	jint retval = jniRegisterNatives(env, clazz, &methods, 1);

	jniReleaseStringUTFChars(env, name, utfName);
	jniReleaseStringUTFChars(env, signature, utfSignature);
	return retval;
}

JNIEXPORT void JNICALL Java_ss_Jn_init(JNIEnv *env, jclass cls, jobject method) {
	M_ss_Jn_resolveNativeMethod = jniFromReflectedMethod(env, method);
	C_ss_Jn = REINTERPRET_CAST(jclass, jniNewGlobalRef(env, cls));
}

//JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void *reserved) {
//	UNUSED(reserved);
//
//	return JNI_VERSION_1_4;
//}

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

	void MarshalToJObject(JNIEnv *env, const StMeta *meta, jobject jobj) {
		asserts(NULL != jobj);
		if (UNLIKELY(NULL == jobj)) {
			return;
		}
		//D_PRINT("StMeta: len = %d\n", meta->len);
		jobject joMember;
		jfieldID* pMemberFieldId = (jfieldID*) meta->mem;
		const char *pmeta = meta->mem + meta->len * sizeof(jfieldID);
		for (int idx = 0; idx < meta->len; idx++) {
			char jsig = ((primitivetype*) pmeta)->sig;
			// D_PRINT("Field[%d]: sig = %c\n", idx, jsig);
			switch (jsig) {
#define SET_FIELD_VALUE(NAME) jniSet##NAME##Field(env, jobj, pMemberFieldId[idx], read<J##NAME>())
			case 'Z':
				SET_FIELD_VALUE(Boolean);
				break;
			case 'B':
				SET_FIELD_VALUE(Byte);
				break;
			case 'C':
				SET_FIELD_VALUE(Char);
				break;
			case 'S':
				SET_FIELD_VALUE(Short);
				break;
			case 'I':
				SET_FIELD_VALUE(Int);
				break;
			case 'J':
				SET_FIELD_VALUE(Long);
				break;
			case 'F':
				SET_FIELD_VALUE(Float);
				break;
			case 'D':
				SET_FIELD_VALUE(Double);
				break;
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
			case 'Z':
				write(jniGetBooleanField(env, jobj, pMemberFieldId[idx]));
				break;
			case 'B':
				write(jniGetByteField(env, jobj, pMemberFieldId[idx]));
				break;
			case 'C':
				write(jniGetCharField(env, jobj, pMemberFieldId[idx]));
				break;
			case 'S':
				write(jniGetShortField(env, jobj, pMemberFieldId[idx]));
				break;
			case 'I':
				write(jniGetIntField(env, jobj, pMemberFieldId[idx]));
				break;
			case 'J':
				write(jniGetLongField(env, jobj, pMemberFieldId[idx]));
				break;
			case 'F':
				write(jniGetFloatField(env, jobj, pMemberFieldId[idx]));
				break;
			case 'D':
				write(jniGetDoubleField(env, jobj, pMemberFieldId[idx]));
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

/*
 lower addresses
 rsp +--------------------+
 | return_address     |
 +--------------------+
 | ...                |
 args->+--------------------+
 | marshaled_param1   | args[0] will be put in rcx and xmm0
 +--------------------+
 | marshaled_param2   | args[1] will be put in rdx and xmm1
 +--------------------+
 | marshaled_param3   | args[2] will be put in r8 and xmm2
 +--------------------+
 | marshaled_param4   | args[3] will be put in r9 and xmm3, args[3] = [rsp] and then rsp = &args[3]
 +--------------------+
 | marshaled_param5   |
 +--------------------+
 | ...                |
 +--------------------+
 | marshaled_paramN   |
 +--------------------+
 higher addresses

 call asm code, it can be cast to a function pointer with any return type that you should know in advance.
 */
extern "C" void st_call_win64(void *args, void *target) ASM("st_call_win64");

/* define some function signatures for st_call_win64, ... */
typedef int (*FnCallIntTarget)(void *args, void *func);
typedef long (*FnCallLongTarget)(void *args, void *func);
typedef float (*FnCallFloatTarget)(void *args, void *func);
typedef double (*FnCallDoubleTarget)(void *args, void *func);
typedef void* (*FnCallPointerTarget)(void *args, void *func);

typedef struct _CallMeta CallMeta;
typedef union _Slot Slot;
typedef struct _MetaHeader MetaHeader;
typedef struct _MetaObj MetaObj;

union _Slot {
	void *ptr;
	intptr_t i;
	char *pb;
	double d;
	Slot *offset(size_t off) {
		return REINTERPRET_CAST(Slot *, pb + off);
	}
};

struct _MetaHeader {
	const char sig;
	union {
		const char type;
		const unsigned char flag;
	};
	union {
		const short size;
		const short extra_offset;
	};
};

struct _CallMeta {
	const unsigned short nargs;
	const char retsig;
	const char rettype;
	const int stack_args_size;
	const StMeta *retmeta;
	void* target;
	MetaHeader argmetas[0];
};

struct _MetaObj {
	int size;
	StMeta *meta;
};

/* This is a stack frame after called, or rather, the call instruction is executed. */
typedef struct _CalledStackFrame {
	Slot returnAddress;

	/* The parameter area(it will always be adjacent to the return address during any function call)*/

	/* Register parameter stack area / C's register parameters shadow space,
	 * which, the four entries space is always allocated regardless of the number
	 * of parameters(that is, even if the called function has fewer than 4 parameters,
	 * these 4 stack locations are effectively owned by the called function), and can
	 * be used by the called function for any purpose(such as saving parameter register
	 * values).Thus the caller may not save information in this region of stack across
	 * a function call.
	 */
	union {
		struct {
			Slot rcxHome;
			Slot rdxHome;
			Slot r8Home;
			Slot r9Home;
		};
		Slot shadow[4];
	};

	/* the 5th, 6th, ... parameter */
	Slot stackParameters[];
} CalledStackFrame;

#define ASM_END_STACK(rsp, retaddr)
//https://gcc.gnu.org/onlinedocs/gcc/Constraints.html
#define ASM_RET_INT(i) __asm__ __volatile("mov %0, %%rax\n\t""ret\n\t": :"m" (i))
#define ASM_RET_FLT(f) __asm__ __volatile("mov %0, %%xmm0\n\t""ret\n\t": :"m" (f))

void interpret_win64(JNIEnv *env, jclass receiver, CallMeta *callmeta, CalledStackFrame *frame) {

	Slot *pslot = frame->stackParameters + callmeta->stack_args_size;
	Slot *args = (Slot *)alloca(callmeta->nargs);
	int idx;
	for (idx = callmeta->nargs; --idx >= 0;) {
		char sig = callmeta->argmetas[idx].sig;
		switch (sig) {
		case 'L': {
			const MetaHeader *pmeta = callmeta->argmetas + idx;
			const MetaObj *pExtraMeta = (MetaObj*) ((const char*) pmeta + 4 * pmeta->extra_offset);
			void *buf = alloca(pExtraMeta->size);
			CRaw(buf).MarshalFromJObject(env, pExtraMeta->meta, (jobject) pslot->ptr);
			pslot->ptr = buf;
		}

			break;
		case '[':
			break;
		case 'F':
		case 'D':
			if (idx < 2) {
				args[idx] = frame->shadow[idx];
			}
			break;
		default:
			break;
		}

	}

	if (callmeta->retsig == 'L' || callmeta->retsig == '[') {
		//void *ret = ((void (*)(void*, void*))call_target)(pslot, callmeta->target);
		void *ret = ((FnCallPointerTarget) st_call_win64)(pslot, callmeta->target);
		const StMeta *meta = callmeta->retmeta;
		jobject jobj;
		if (meta->ctor) {
			jobj = jniNewObject(env, meta->cls, meta->ctor);
		} else {
			jobj = jniAllocObject(env, meta->cls);
		}
		CRaw(ret).MarshalToJObject(env, callmeta->retmeta, jobj);
		ASM_END_STACK(0, slots[0]);
		ASM_RET_INT(jobj);
	} else {

	}

}

void just_interpret_win64(JNIEnv *env, jclass receiver, jmethodID mid, CalledStackFrame *frame) {
	nativetrace(_T("resolve_interpret_win64(env@%p, receiver@%p,  mid@%p, frame@%p)\n"), env, receiver, mid, frame);
	jclass jcls = jniGetObjectClass(env, receiver);
	jclass jcls2 = jniGetObjectClass(env, jcls);
	jboolean isStaticMethod = jniIsSameObject(env, jcls, jcls2);
	jobject jmethod = jniToReflectedMethod(env, isStaticMethod ? receiver : jcls, mid, isStaticMethod);
	CallMeta *callmeta = REINTERPRET_CAST(CallMeta*,
			jniCallStaticLongMethod(env, C_ss_Jn, M_ss_Jn_resolveNativeMethod, jmethod, receiver));
	nativetrace(_T("-resolve_interpret_win64, %p, isStaticMethod = %d, CallMeta@%p\n"),
			mid, isStaticMethod, callmeta);
	interpret_win64(env, receiver, callmeta, frame);
}

