open BsReactNative;

type action =
  | SetNote(Notes.note)
  | SetSounds(Notes.association)
  | LoadSounds(Notes.octave)
  | IncrementNShowing
  | TogglePlayState;

type state = {
  note: Notes.note,
  nShowingNotes: int,
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
  let handleAppStateChange = (_, self) =>
    switch (self.ReasonReact.state.playState, AppState.currentState()) {
    | (Playing, Background) => self.send(TogglePlayState)
    | (_, _) => ()
    };
  {
    ...component,
    initialState: () => {note: C, nShowingNotes: 0, sounds: [], playState: Loading},
    reducer: (action, state) =>
      switch action {
      | LoadSounds(octave) =>
        ReasonReact.SideEffects(
          (
            (self) =>
              Notes.loadAssociation(
                octave,
                Notes.all,
                ~onProgress=() => self.send(IncrementNShowing)
              )
              |> Js.Promise.(
                   then_((association) => self.send @@ SetSounds(association) |> resolve)
                 )
              |> ignore
          )
        )
      | IncrementNShowing => ReasonReact.Update({...state, nShowingNotes: state.nShowingNotes + 1})
      | SetSounds(association) =>
        ReasonReact.Update({
          ...state,
          sounds: association,
          playState:
            switch state.playState {
            | Loading => Paused
            | Paused => Paused
            | Playing => Playing
            }
        })
      | SetNote(note) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, note},
          (
            ({state}) =>
              switch state.playState {
              | Playing => playSound @@ Notes.getSound(note, state.sounds)
              | Paused => ()
              }
          )
        )
      | TogglePlayState =>
        ReasonReact.UpdateWithSideEffects(
          {
            ...state,
            playState:
              switch state.playState {
              | Paused => Playing
              | Playing => Paused
              | Loading => Loading
              }
          },
          (
            ({state}) =>
              switch state.playState {
              | Playing => playSound @@ Notes.getSound(state.note, state.sounds)
              | Paused => SoundPool.pauseAll()
              | _ => ()
              }
          )
        )
      },
    didMount: (self) => {
      SoundPool.init({maxStreams: 1, usage: 1, contentType: 2});
      self.send @@ LoadSounds(2);
      AppState.addEventListener("change", self.handle(handleAppStateChange));
      ReasonReact.NoUpdate
    },
    render: (self) =>
      <View style=Style.(style([flex(1.), justifyContent(Center), alignItems(Center)]))>
        <OctaveSelector
          position=Style.(style([position(Absolute), top(Pt(30.)), left(Pt(30.))]))
          handleOctaveChanged=1
        />
        <Tick visible=(self.state.playState != Loading) />
        <RotatableDial ticks=Notes.all onRelease=((note) => self.send(SetNote(note)))>
          <Dial nShowing=self.state.nShowingNotes />
        </RotatableDial>
        <TouchableOpacity style=playPauseStyle onPress=(() => self.send(TogglePlayState))>
          (
            self.state.playState != Loading ?
              <PlayPause state=self.state.playState /> : ReasonReact.nullElement
          )
        </TouchableOpacity>
      </View>
  }
};
