import React from 'react';
import {
    View,
    StyleSheet,
    Button,
    Text
} from 'react-native';
import SoundPool from './SoundPool';

const options = {
    maxStreams: 1,
    usage: SoundPool.USAGE.MEDIA,
    contentType: SoundPool.CONTENT_TYPE.MUSIC
};

SoundPool.init(options);

class App extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            sound: -1,
            isPlaying: false
        };
    }

    async componentDidMount() {
        const { soundId } = await SoundPool.load('note');
        this.setState({ sound: soundId })
    }

    togglePlayback = () => {
        if (this.state.isPlaying) {
            SoundPool.stop(this.state.sound);
        } else {
            SoundPool.play(this.state.sound, {
                loop: -1,
                leftVolume: 1.0,
                rightVolume: 1.0,
                rate: 1.0,
                priority: 1
            });
            this.setState({ isPlaying: true });
        }
    };

    render() {
        return (
            <View style={styles.container}>
                {this.state.sound === -1 ? (
                    <Text>Loading...</Text>
                ) : (
                    <Button
                        onPress={this.togglePlayback}
                        title="Play!"
                    />
                )}
            </View>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: '#F5FCFF',
    }
});

export default App;
