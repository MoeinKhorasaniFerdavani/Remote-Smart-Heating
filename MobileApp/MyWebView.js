import React, { Component } from 'react';
import { WebView } from 'react-native-webview';

export default class MyWebView extends Component {
  render() {
    return (
      <WebView
        source={{
          uri: 'https://yekta.iut.ac.ir',
        }}
        style={{ marginTop: 20 }}
      />
    );
  }
}
