open BsReactNative;

type action =
    | SetSoundId(SoundPool.soundId)
    | LoadSound(SoundPool.fileId)
    | PlaySound;

type state = {
    sound: SoundPool.soundId
};

let opts: SoundPool.playOptions = {
    loop: -1,
    leftVolume: 1.0,
    rightVolume: 1.0,
    rate: 1.0,
    priority: 1
};

let poolOpts: SoundPool.poolOptions = {
    maxStreams: 1,
    usage: 0,
    contentType: 0
};

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
    ...component,
    initialState: () => {
        sound: -1
    },
    reducer: (action, state) =>
        switch (action) {
            | SetSoundId(id) => ReasonReact.Update({ sound: id })
            | PlaySound => ReasonReact.SideEffects((_self) => SoundPool.play(state.sound, opts))
            | LoadSound(name) => ReasonReact.SideEffects((self) =>
                SoundPool.loadNote(name)
                |> Js.Promise.(then_((result: SoundPool.loadResult) => resolve(
                    self.send(SetSoundId(result.soundId))
                )))
                |> ignore
                )
        },
    didMount: (self) => {
        SoundPool.init(poolOpts);
        self.send(LoadSound("note"));
        ReasonReact.NoUpdate;
    },
    render: (self) =>
        <View style=Style.(style([flex(1.), justifyContent(Center), alignItems(Center)]))>
            {switch (self.state.sound) {
                | -1 =>
                    <Text value="Loading..." />

                | _ =>
                    <Button
                        title="Play!"
                        onPress={() => self.send(PlaySound)}
                    />
            }}
        </View>
};
