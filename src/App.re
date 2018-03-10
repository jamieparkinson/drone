open BsReactNative;

type action =
  | SetNote(Notes.note)
  | SetSounds(Notes.association)
  | LoadSounds(Notes.octave)
  | TogglePlayState;

type state = {
  note: Notes.note,
  sounds: Notes.association,
  playState: PlayPause.playState
};

let ticks = Notes.all |> Array.map(Notes.getIndex) |> Array.map(Layout.getThetaFromIndex);

let component = ReasonReact.reducerComponent("App");

let playSound =
  SoundPool.play({loop: (-1), leftVolume: 1.0, rightVolume: 1.0, rate: 1.0, priority: 1});

let playPauseStyle =
  Style.(
    style([
      position(Absolute),
      top(Pct(50.)),
      left(Pct(50.)),
      Transform.make(~translateX=-8., ~translateY=-7., ())
    ])
  );

let make = (_children) => {
  ...component,
  initialState: () => {note: C, sounds: [], playState: Paused},
  reducer: (action, state) =>
    switch action {
    | LoadSounds(octave) =>
      ReasonReact.SideEffects(
        (
          (self) =>
            Notes.loadAssociation(octave, Notes.all)
            |> Js.Promise.(then_((association) => self.send @@ SetSounds(association) |> resolve))
            |> ignore
        )
      )
    | SetSounds(association) => ReasonReact.Update({...state, sounds: association})
    | SetNote(note) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, note},
        ({state}) => switch (state.playState) {
          | Playing => playSound @@ Notes.getSound(note, state.sounds)
          | Paused => ()
        }
      )
    | TogglePlayState =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          playState:
            switch state.playState {
            | Paused => Playing
            | Playing => Paused
            }
        },
        (({state}) => {
          switch (state.playState) {
          | Playing => playSound @@ Notes.getSound(state.note, state.sounds)
          | Paused => SoundPool.pauseAll()
          }
        })
      )
    },
  didMount: (self) => {
    SoundPool.init({maxStreams: 1, usage: 1, contentType: 2});
    self.send @@ LoadSounds(2);
    ReasonReact.NoUpdate
  },
  render: (self) =>
    <View style=Style.(style([flex(1.), justifyContent(Center), alignItems(Center)]))>
      <Tick />
      <RotatableDial ticks=Notes.all onRelease=((note) => self.send(SetNote(note)))> <Dial /> </RotatableDial>
      <TouchableOpacity style=playPauseStyle onPress=(() => self.send(TogglePlayState))>
        <PlayPause state=self.state.playState />
      </TouchableOpacity>
    </View>
};
