//
// Created by 12302 on 2025/4/16.
//

#include <jni.h>
jlong get_handle_value(JNIEnv *env, jobject pthis, const char* class_name) {
	jclass clsj = env->FindClass(class_name);
	if (clsj == nullptr) {
		return -1;
	}

	jfieldID fj = env->GetFieldID(clsj, "handle", "J");
	if (fj == nullptr) {
		env->DeleteLocalRef(clsj);
		return -1;
	}

	jlong handle = env->GetLongField(pthis, fj);
	env->DeleteLocalRef(clsj);

	return handle;
}

void set_handle_value(JNIEnv *env, jobject pthis, const char* class_name, jlong handle_value) {
	jclass clsj = env->FindClass(class_name);
	if (clsj == nullptr)
		return;

	jfieldID fj = env->GetFieldID(clsj, "handle", "J");
	if (fj == nullptr) {
		env->DeleteLocalRef(clsj);
		return;
	}

	env->SetLongField(pthis, fj, handle_value);
	env->DeleteLocalRef(clsj);
}

jobject attach_handle(JNIEnv *env, const char *class_name, jlong handle_value) {
	jclass clazz = env->FindClass(class_name);
	if (clazz == nullptr)
		return nullptr;

	jmethodID default_constructor = env->GetMethodID(clazz, "<init>", "()V");
	if (default_constructor == nullptr) {
		env->DeleteLocalRef(clazz);
		return nullptr;
	}

	jobject obj = env->NewObject(clazz, default_constructor);
	set_handle_value(env, obj, class_name, handle_value);

	return obj;
}


void throw_exception(JNIEnv *env, const char *class_name, const char *message) {
	jclass clsj = env->FindClass(class_name);
	if (clsj == nullptr)
		return;

	env->ThrowNew(clsj, message);
	env->DeleteLocalRef(clsj);
}

void throw_NullPointerException(JNIEnv *env, const char* message) {
	throw_exception(env, "Ljava.lang.NullPointerException;", message);
}