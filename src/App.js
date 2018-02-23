import React from 'react';
import {
    View,
    StyleSheet,
    Button
} from 'react-native';
import SoundPool from './SoundPool';

SoundPool.init();
SoundPool.load('note');

const App = () => (
    <View style={styles.container}>
        <Button
            onPress={() => SoundPool.play()}
            title="Play!"
        />
    </View>
);

const styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: '#F5FCFF',
    }
});

export default App;
