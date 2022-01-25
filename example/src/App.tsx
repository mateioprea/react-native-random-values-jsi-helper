import * as React from 'react';

import { StyleSheet, View, Text, TouchableOpacity } from 'react-native';
import 'react-native-random-values-jsi-helper';
import { useCallback, useMemo } from 'react';
import { v4 as uuid } from 'uuid';

export default function App() {
  const [_uuid, setUuid] = React.useState(undefined);

  const generateUuid = useCallback(() => {
    setUuid(uuid());
  }, []);

  return (
    <View style={styles.container}>
      <TouchableOpacity onPress={generateUuid}>
        <Text>Generate UUID</Text>
      </TouchableOpacity>
      <Text>{_uuid}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    alignItems: 'center',
    justifyContent: 'center',
  },
  box: {
    width: 60,
    height: 60,
    marginVertical: 20,
  },
});
