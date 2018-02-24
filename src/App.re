open BsReactNative;

module NotesMap = Map.Make(String);

type action =
    | SetSoundId((int, string))
    | LoadSounds
    | PlaySound(string);


type state = {
    sounds: NotesMap.t(SoundPool.soundId),
    loaded: bool
};

let opts: SoundPool.playOptions = {
    loop: -1,
    leftVolume: 1.0,
    rightVolume: 1.0,
    rate: 1.0,
    priority: 1
};

let poolOpts: SoundPool.poolOptions = {
    maxStreams: 2,
    usage: 0,
    contentType: 0
};

let notes = [|
    "c2",
    "cis2",
    "d2",
    "ees2",
    "e2",
    "f2",
    "fis2",
    "g2",
    "aes2",
    "a2",
    "bes2",
    "b2"
|];

let renderNoteButton = (handler, note) =>
    <Button
        key={note}
        title={note}
        onPress={() => handler(note)}
    />;

let component = ReasonReact.reducerComponent("App");
let make = (_children) => {
    ...component,
    initialState: () => {
        sounds: NotesMap.empty,
        loaded: true
    },
    reducer: (action, state) =>
        switch (action) {
            | SetSoundId((id, note)) =>
                ReasonReact.Update({
                    ...state,
                    sounds: state.sounds |> NotesMap.add(note, id)
                })
            | PlaySound(name) =>
                ReasonReact.SideEffects((_self) => state.sounds
                    |> NotesMap.find(name)
                    |> SoundPool.play(opts)
                )
            | LoadSounds =>
                ReasonReact.SideEffects((self) =>
                    SoundPool.loadNotes(notes)
                        |> Js.Promise.(then_(ids => ids
                            |> Array.mapi((i, { soundId }: SoundPool.loadResult) => (soundId, notes[i]))
                            |> Array.map(soundInfo => SetSoundId(soundInfo) |> self.send)
                            |> resolve
                        ))
                        |> ignore
                )
        },
    didMount: (self) => {
        SoundPool.init(poolOpts);
        self.send(LoadSounds);
        ReasonReact.NoUpdate;
    },
    render: (self) =>
        <View style=Style.(style([flex(1.), justifyContent(Center), alignItems(Center)]))>
            {switch (self.state.sounds |> NotesMap.cardinal) {
                | 12 =>
                    <View>{ ReasonReact.arrayToElement(
                        Array.map(renderNoteButton(note => PlaySound(note) |> self.send), notes)
                    )}</View>

                | _ =>
                    <Text value="Loading..." />
            }}
        </View>
};
