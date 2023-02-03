import { StatusBar } from 'expo-status-bar';
import { StyleSheet, Text, View } from 'react-native';
import MyWebView from './MyWebView';

export default function App() {
  return (
    <View style={styles.container}>
      <Text>Bootan Start Home ljklj!</Text>
      <StatusBar style="auto" />
      <MyWebView />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
});
