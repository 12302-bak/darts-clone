//
// Created by 12302 on 2025/4/16.
//

#include "DoubleArrayNative.h"
#include "darts-config.h"
#include <darts.h>
#include <fstream>
#include <sstream>

extern jlong get_handle_value(JNIEnv *env, jobject pthis, const char* class_name);
extern void set_handle_value(JNIEnv *env, jobject pthis, const char* class_name, jlong handle_value);
extern void throw_exception(JNIEnv *env, const char *class_name, const char *message);
extern void throw_NullPointerException(JNIEnv *env, const char* message);
//
Darts::DoubleArray* ensure_create_darts(JNIEnv *env, jobject pthis) {
	jlong handle = get_handle_value(env, pthis, "Ljp/darts/DoubleArrayNative;");
	if (handle == -1) {
		Darts::DoubleArray* darts = new Darts::DoubleArray();
		set_handle_value(env, pthis, "Ljp/darts/DoubleArrayNative;", (jlong)darts);
		return darts;
	}

	return (Darts::DoubleArray*)(handle);
}

Darts::DoubleArray* get_darts(JNIEnv *env, jobject pthis) {
	jlong handle = get_handle_value(env, pthis, "Ljp/darts/DoubleArrayNative;");
	if (handle == -1) {
		return nullptr;
	}

	return (Darts::DoubleArray*)(handle);
}

void darts_search(const Darts::DartsConfig &config,
    const Darts::DoubleArray &dic, std::istream *lexicon) {
  std::vector<Darts::DoubleArray::result_pair_type> result_pairs(1024);

  std::string query;
  while (std::getline(*lexicon, query)) {
    if (config.has_values()) {
      std::string::size_type tab_pos = query.find_last_of('\t');
      if (tab_pos != std::string::npos) {
        query = query.substr(0, tab_pos);
      }
    }

    std::size_t num_results = dic.commonPrefixSearch(
        query.c_str(), &result_pairs[0], result_pairs.size());
    if (num_results > 0) {
      std::cout << query << ": found, num = " << num_results;
      for (std::size_t i = 0; i < num_results; ++i) {
        std::cout << ' ' << result_pairs[i].value
            << ':' << result_pairs[i].length;
      }
      std::cout << std::endl;
    } else {
      std::cout << query << ": not found" << std::endl;
    }
  }
}

extern "C"
JNIEXPORT void JNICALL Java_jp_darts_DoubleArrayNative_createInstance(JNIEnv *env, jobject pthis){
    Darts::DoubleArray* native_darts = ensure_create_darts(env, pthis);
	if (native_darts == nullptr) {
		throw_exception(env, "Ljp/darts/DoubleArrayException;","handle is null");
		return;
	}

//    try {
//        Darts::DartsConfig config;
//
//        std::ifstream file("/Users/stevenobelia/Documents/project_clion_test/darts-clone/data/lexicon.txt");
//        if (!file) {
//            std::cerr << "error: failed to open lexicon file: " << config.lexicon_file_name() << std::endl;
//            std::exit(1);
//        }
//        native_darts->open("/Users/stevenobelia/Documents/project_clion_test/darts-clone/data/darts.bin");
//        darts_search(config, *native_darts, &file);
//		//native_trie->build(*native_keyset, config_flags);
//	} catch ( ... ) {
//		throw_exception(env, "Ljp/darts/DoubleArrayException;","unknown exception");
//		return;
//	}
}

extern "C"
JNIEXPORT void JNICALL Java_jp_darts_DoubleArrayNative_destroyInstance(JNIEnv *env, jobject pthis){
    Darts::DoubleArray* native_darts = get_darts(env, pthis);
	if (native_darts == nullptr) {
		return;
	}

	delete native_darts;
	set_handle_value(env, pthis, "Ljp/darts/DoubleArrayNative;", (jlong)-1);
}

extern "C"
JNIEXPORT void JNICALL Java_jp_darts_DoubleArrayNative_setArray(JNIEnv *env, jobject pthis, jbyteArray ptr, jint size){
    Darts::DoubleArray* native_darts = ensure_create_darts(env, pthis);
	if (native_darts == nullptr) {
		throw_exception(env, "Ljp/darts/DoubleArrayException;","handle is null");
		return;
	}

    if (ptr == nullptr) {
		throw_NullPointerException(env, "ptr is null");
		return;
	}

	jbyte* native_ptr = env->GetByteArrayElements(ptr, nullptr);

	try {
        native_darts->set_array(ptr, size);
	} catch ( ... ) {
		throw_exception(env, "Ljp/darts/DoubleArrayException;","unknown exception");
		return;
	}

	env->ReleaseByteArrayElements(ptr, native_ptr, JNI_ABORT);
}

extern "C"
JNIEXPORT jbyteArray JNICALL Java_jp_darts_DoubleArrayNative_getArray(JNIEnv *env, jobject pthis) {
    Darts::DoubleArray* native_darts = ensure_create_darts(env, pthis);
	if (native_darts == nullptr) {
		throw_exception(env, "Ljp/darts/DoubleArrayException;","handle is null");
		return nullptr;
	}

	const void* ptr = native_darts->array();
	const std::size_t length = native_darts->size();

	jbyteArray java_array = env->NewByteArray(length);
	jbyte* native_array = env->GetByteArrayElements(java_array, nullptr);
	memcpy(native_array, ptr, length);
	env->ReleaseByteArrayElements(java_array, native_array, 0);

	return java_array;
}

extern "C"
JNIEXPORT jint JNICALL Java_jp_darts_DoubleArrayNative_size(JNIEnv *env, jobject pthis){
    Darts::DoubleArray* native_darts = ensure_create_darts(env, pthis);
	if (native_darts == nullptr) {
		throw_exception(env, "Ljp/darts/DoubleArrayException;","handle is null");
		return -1;
	}

	const std::size_t length = native_darts->size();
	return static_cast<jint>(length);
}


extern "C"
JNIEXPORT jint JNICALL Java_jp_darts_DoubleArrayNative_build(JNIEnv *env, jobject pthis, jint numKeys, jobjectArray jKeys){
    Darts::DoubleArray* native_darts = ensure_create_darts(env, pthis);
	if (native_darts == nullptr) {
		throw_exception(env, "Ljp/darts/DoubleArrayException;","handle is null");
		return -1;
	}

    //std::vector<size_t> lengths(numKeys);
    //std::vector<int> values(numKeys);

    const char** keyArray = new const char*[numKeys];
    std::vector<std::string> cppKeys;
    cppKeys.reserve(numKeys);

    for (int i = 0; i < numKeys; ++i) {
        jstring jKey = (jstring)env->GetObjectArrayElement(jKeys, i);
        const char* key = env->GetStringUTFChars(jKey, nullptr);
        cppKeys.emplace_back(key);
        keyArray[i] = cppKeys.back().c_str();
        env->ReleaseStringUTFChars(jKey, key);
        env->DeleteLocalRef(jKey);
    }

    try {
        size_t build = native_darts->build(numKeys, keyArray);
        delete[] keyArray;
        return static_cast<jint>(build);
    } catch ( ... ) {
        delete[] keyArray;
        throw_exception(env, "Ljp/darts/DoubleArrayException;","maybe keys conflict or malformed");
        return -1;
    }
}

extern "C"
JNIEXPORT jint JNICALL Java_jp_darts_DoubleArrayNative_open(JNIEnv *env, jobject pthis, jbyteArray fileName, jint offset, jint size){
    Darts::DoubleArray* native_darts = ensure_create_darts(env, pthis);
	if (native_darts == nullptr) {
		throw_exception(env, "Ljp/darts/DoubleArrayException;","handle is null");
		return -1;
	}

    jbyte* native_fileName = env->GetByteArrayElements(fileName, NULL);
	jsize fileNameLength = env->GetArrayLength(fileName);

	char* tmp = (char *)alloca(fileNameLength + 1);
	memcpy(tmp, native_fileName, fileNameLength);
	tmp[fileNameLength] = 0;

	env->ReleaseByteArrayElements(fileName, native_fileName, JNI_ABORT);

    try {
        size_t open = native_darts->open(tmp,"rb", offset, size);
        return static_cast<jint>(open);
    } catch ( ... ) {
        std::cerr << "error: failed to save file: " << std::endl;
        throw_exception(env, "Ljp/darts/DoubleArrayException;","unknown exception");
        return -1;
    }
}

extern "C"
JNIEXPORT jint JNICALL Java_jp_darts_DoubleArrayNative_save(JNIEnv *env, jobject pthis, jbyteArray fileName, jint offset){
    Darts::DoubleArray* native_darts = ensure_create_darts(env, pthis);
	if (native_darts == nullptr) {
		throw_exception(env, "Ljp/darts/DoubleArrayException;","handle is null");
		return -1;
	}

    jbyte* native_fileName = env->GetByteArrayElements(fileName, NULL);
	jsize fileNameLength = env->GetArrayLength(fileName);

	char* tmp = (char *)alloca(fileNameLength + 1);
	memcpy(tmp, native_fileName, fileNameLength);
	tmp[fileNameLength] = 0;

	env->ReleaseByteArrayElements(fileName, native_fileName, JNI_ABORT);

    try {
        size_t save = native_darts->save(tmp, "wb", offset);
        return static_cast<jint>(save);
    } catch ( ... ) {
        throw_exception(env, "Ljp/darts/DoubleArrayException;","unknown exception");
        return -1;
    }
}

extern "C"
JNIEXPORT jint JNICALL Java_jp_darts_DoubleArrayNative_exactMatchSearch(JNIEnv *env, jobject pthis, jstring jKey, jint jLength, jint jNodePos){
    Darts::DoubleArray* native_darts = ensure_create_darts(env, pthis);
	if (native_darts == nullptr) {
		throw_exception(env, "Ljp/darts/DoubleArrayException;","handle is null");
		return -1;
	}

    const char* cKey = env->GetStringUTFChars(jKey, nullptr);
    if (cKey == nullptr) {
        std::cerr << "Failed to get key from JNI string." << std::endl;
        return -1;
    }
    std::string cppKey(cKey);
    env->ReleaseStringUTFChars(jKey, cKey);

    size_t result = native_darts ->exactMatchSearch<int>(cppKey.c_str());
    return static_cast<jint>(result);
}


extern "C"
JNIEXPORT jobjectArray JNICALL Java_jp_darts_DoubleArrayNative_commonPrefixSearch(JNIEnv *env, jobject pthis, jstring jKey, jint jMaxNumResults, jint jLength, jint jNodePos){
    Darts::DoubleArray* native_darts = ensure_create_darts(env, pthis);
	if (native_darts == nullptr) {
		throw_exception(env, "Ljp/darts/DoubleArrayException;","handle is null");
		return nullptr;
	}

    // 将 Java 字符串转换为 C++ 字符串
    const char* cKey = env->GetStringUTFChars(jKey, nullptr);
    if (cKey == nullptr) {
        std::cerr << "Failed to get key from JNI string." << std::endl;
        return nullptr;
    }

    std::string cppKey(cKey);
    env->ReleaseStringUTFChars(jKey, cKey);

    std::vector<Darts::DoubleArray::result_pair_type> results;
    try {
        results.resize(jMaxNumResults);
        size_t nums = native_darts->commonPrefixSearch(cppKey.c_str(), &results.front(), results.size(), jLength, jNodePos);
        results.resize(nums);
    } catch ( ... ) {
        throw_exception(env, "Ljp/darts/DoubleArrayException;","unknown exception");
        return nullptr;
    }

    jclass matchClass = env->FindClass("jp/darts/DoubleArrayNative$Match");
    if (matchClass == nullptr) {
        std::cerr << "Failed to find SearchResult class." << std::endl;
        return nullptr;
    }

    jmethodID constructor = env->GetMethodID(matchClass, "<init>", "()V");
    if (constructor == nullptr) {
        std::cerr << "Failed to find SearchResult constructor." << std::endl;
        return nullptr;
    }

    jobjectArray resultArray = env->NewObjectArray(results.size(), matchClass, nullptr);
    if (resultArray == nullptr) {
        std::cerr << "Failed to create Match array." << std::endl;
        return nullptr;
    }

    for (size_t i = 0; i < results.size(); ++i) {
        jobject matchObj = env->NewObject(matchClass, constructor);
        if (matchObj == nullptr) {
            std::cerr << "Failed to create Match object." << std::endl;
            return nullptr;
        }

        jfieldID valueField = env->GetFieldID(matchClass, "value", "I");
        jfieldID lengthField = env->GetFieldID(matchClass, "length", "I");

        env->SetIntField(matchObj, valueField, results[i].value);
        env->SetIntField(matchObj, lengthField, results[i].length);

        env->SetObjectArrayElement(resultArray, i, matchObj);
        env->DeleteLocalRef(matchObj);
    }
    return resultArray;
}
