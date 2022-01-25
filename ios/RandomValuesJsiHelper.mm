#import "RandomValuesJsiHelper.h"
#import "react-native-random-values-jsi-helper.h"
#import <React/RCTBlobManager.h>
#import <React/RCTBridge+Private.h>
#import <jsi/jsi.h>
#import "TypedArray.hpp"

#import <memory>

using namespace facebook;



constexpr const char *OnJSRuntimeDestroyPropertyName = "__RandomValuesOnJsRuntimeDestroy";

void registerOnJSRuntimeDestroy(jsi::Runtime &runtime) {
    runtime.global().setProperty(
                               runtime,
                               OnJSRuntimeDestroyPropertyName,
                               jsi::Object::createFromHostObject(
                                   runtime, std::make_shared<InvalidateCacheOnDestroy>(runtime)));
}

@implementation RandomValuesJsiHelper


RCT_EXPORT_MODULE()

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
    
    NSLog(@"Installing crypto.getRandomValues polyfill Bindings..."); 
    RCTBridge* bridge = [RCTBridge currentBridge];
    RCTCxxBridge* cxxBridge = (RCTCxxBridge*)bridge;
    if (cxxBridge == nil) {
        return @false;
    }
    auto jsiRuntime = (jsi::Runtime*) cxxBridge.runtime;
    
    if (jsiRuntime == nil) {
        return @false;
    }
    
    auto& runtime = *jsiRuntime;

    registerOnJSRuntimeDestroy(runtime);
    
    auto getRandomValues = jsi::Function::createFromHostFunction(runtime,
                                                                 jsi::PropNameID::forUtf8(runtime, "getRandomValues"),
                                                                 1,
                                                                 [](jsi::Runtime& runtime,
                                                                    const jsi::Value& thisArg,
                                                                    const jsi::Value* args,
                                                                    size_t count) -> jsi::Value {
        auto byteLength = args[0].asNumber();

        NSMutableData *data = [NSMutableData dataWithLength:byteLength];
        int result = SecRandomCopyBytes(kSecRandomDefault, byteLength, data.mutableBytes);
        if (result != errSecSuccess) {
        }
        
        auto typedArray = TypedArray<TypedArrayKind::Uint8Array>(runtime, byteLength);
        auto arrayBuffer = typedArray.getBuffer(runtime);
        memcpy(arrayBuffer.data(runtime), data.bytes, data.length);
        return typedArray;
    });
    runtime.global().setProperty(runtime, "getRandomValues", getRandomValues);
    
    return @true;
    
}
@end
