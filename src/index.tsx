import { NativeModules, Platform } from 'react-native';

const LINKING_ERROR =
  `The package 'react-native-random-values-jsi-helper' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo managed workflow\n';

const RandomValuesJsiHelper = NativeModules.RandomValuesJsiHelper
  ? NativeModules.RandomValuesJsiHelper
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

RandomValuesJsiHelper.install();

// @ts-expect-error
if (typeof global.crypto !== 'object') {
  // @ts-ignore
  global.crypto = {};
}
// @ts-expect-error
global.crypto.getRandomValues = (array: ArrayBuffer) => {
  // @ts-expect-error
  return global.getRandomValues(array.byteLength);
};
