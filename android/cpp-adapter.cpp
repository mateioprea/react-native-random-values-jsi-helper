#include <jni.h>
#include <jsi/jsi.h>
#include <string>
#include <vector>
#include "../cpp/TypedArray.hpp"
#include <android/log.h>

using namespace facebook;

typedef u_int8_t byte;

constexpr const char *OnJSRuntimeDestroyPropertyName = "__RandomValuesOnJsRuntimeDestroy";

void registerOnJSRuntimeDestroy(jsi::Runtime &runtime) {
    runtime.global().setProperty(
            runtime,
            OnJSRuntimeDestroyPropertyName,
            jsi::Object::createFromHostObject(
                    runtime, std::make_shared<InvalidateCacheOnDestroy>(runtime)));
}

void install(jsi::Runtime& jsiRuntime, std::function<byte*(int size)> createRandomBytes) {


    registerOnJSRuntimeDestroy(jsiRuntime);

    auto getRandomValues = jsi::Function::createFromHostFunction(jsiRuntime,
                                                                 jsi::PropNameID::forUtf8(jsiRuntime, "getRandomValues"),
                                                                 1,
                                                                 [=](jsi::Runtime& runtime,
                                                                    const jsi::Value& thisArg,
                                                                    const jsi::Value* args,
                                                                    size_t count) -> jsi::Value {

        if (count != 1) {
            throw jsi::JSError(runtime, "getRandomValues(..) expects one argument (object)!");
        }
        auto byteLength = args[0].asNumber();
        auto randomBytes = createRandomBytes(byteLength);

        auto typedArray = TypedArray<TypedArrayKind::Uint8Array>(runtime, byteLength);
        auto arrayBuffer = typedArray.getBuffer(runtime);
        memcpy(arrayBuffer.data(runtime), reinterpret_cast<byte*>(randomBytes), byteLength);
        return typedArray;
    });

    jsiRuntime.global().setProperty(jsiRuntime, "getRandomValues", getRandomValues);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_reactnativerandomvaluesjsihelper_RandomValuesJsiHelperModule_nativeInstall(JNIEnv *env, jclass _, jlong jsiPtr, jobject instance) {
    auto instanceGlobal = env->NewGlobalRef(instance);
    auto createRandomBytes = [=](int size) -> byte* {
        if (!env) throw std::runtime_error("JNI Environment is gone!");

        jclass clazz = env->GetObjectClass(instanceGlobal);
        /*
         * https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html
         * 4.3.2. Field Descriptors
         */
        jmethodID getRandomBytes = env->GetMethodID(clazz, "getRandomBytes", "(I)[B");
        auto b = (jbyteArray) env->CallObjectMethod(instanceGlobal,
                                                    getRandomBytes,
                                                    size);
        jboolean isCopy = true;
        jbyte* bytes = env->GetByteArrayElements(b, &isCopy);
        env->DeleteLocalRef(b);
        return reinterpret_cast<byte*>(bytes);
    };

    auto runtime = reinterpret_cast<jsi::Runtime*>(jsiPtr);
    if (runtime) {
        install(*runtime, createRandomBytes);
    }
}
