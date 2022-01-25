# react-native-random-values-jsi-helper

React Native polyfill for crypto.getRandomValues. Used in libraries like [uuid](https://www.npmjs.com/package/uuid).

## Installation

```sh
yarn add react-native-random-values-jsi-helper
npx pod-install
```

## Usage

Import ```react-native-random-values-jsi-helper``` in your index.js file.

```js
//index.js
import "react-native-random-values-jsi-helper";
```

You can now use the [uuid](https://www.npmjs.com/package/uuid) package in your React Native app.

## Performance

This module is written in C++ JSI.

Basically there is no over the bridge traffic overhead and no serialization/deserialization since the random arrays are generated on native side.

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT

## Acknowledgements

[@mrousavy](https://github.com/mrousavy)

[@expo](https://github.com/expo/expo)

## Other projects

[react-native-get-random-values](https://github.com/LinusU/react-native-get-random-values)
