open BsReactNative;

type action =
  | SetNote(Notes.note)
  | SetSounds(Notes.association)
  | SetOctave(Notes.Octave.t)
  | LoadSounds(Notes.Octave.t)
  | IncrementNShowing
  | SetPlayState(PlayPause.playState);

type state = {
  note: Notes.note,
  octave: Notes.Octave.t,
  nShowingNotes: int,
  sounds: Notes.association,
  playState: PlayPause.playState,
};

let ticks =
  Notes.all
  |> Array.map(Notes.getIndex)
  |> Array.map(Layout.getThetaFromIndex);

let component = ReasonReact.reducerComponent("App");

let playSound =
  SoundPool.play({
    loop: (-1),
    leftVolume: 1.0,
    rightVolume: 1.0,
    rate: 1.0,
    priority: 1,
  });

let playPauseStyle =
  Style.(
    style([
      position(Absolute),
      top(Pct(50.)),
      left(Pct(50.)),
      Transform.make(~translateX=-8., ~translateY=-7., ()),
    ])
  );

let make = _children => {
  let handleAppStateChange = (_, self) =>
    switch (AppState.currentState()) {
    | Background => self.ReasonReact.send(SetPlayState(Paused))
    | _ => ()
    };
  {
    ...component,
    initialState: () => {
      note: C,
      octave: Notes.Octave.setInt(2),
      nShowingNotes: 0,
      sounds: [],
      playState: Loading,
    },
    reducer: (action, state) =>
      switch (action) {
      | LoadSounds(octave) =>
        ReasonReact.SideEffects(
          (
            self =>
              Notes.loadAssociation(octave, Notes.all, ~onProgress=() =>
                self.send(IncrementNShowing)
              )
              |> Js.Promise.(
                   then_(association =>
                     self.send @@ SetSounds(association) |> resolve
                   )
                 )
              |> ignore
          ),
        )
      | IncrementNShowing =>
        ReasonReact.Update({...state, nShowingNotes: state.nShowingNotes + 1})
      | SetSounds(association) =>
        ReasonReact.Update({
          ...state,
          sounds: association,
          playState:
            switch (state.playState) {
            | Loading => Paused
            | Paused => Paused
            | Playing => Playing
            },
        })
      | SetNote(note) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, note},
          (
            ({state}) =>
              switch (state.playState) {
              | Playing => playSound @@ Notes.getSound(note, state.sounds)
              | Paused => ()
              }
          ),
        )
      | SetOctave(octave) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, nShowingNotes: 0, octave},
          (self => self.send(LoadSounds(self.state.octave))),
        )
      | SetPlayState(newState) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, playState: newState},
          (
            ({state}) =>
              switch (state.playState) {
              | Playing =>
                playSound @@ Notes.getSound(state.note, state.sounds);
                RNKeepAwake.activate();
              | Paused =>
                SoundPool.pauseAll();
                RNKeepAwake.deactivate();
              | _ => ()
              }
          ),
        )
      },
    didMount: self => {
      SoundPool.init({maxStreams: 1, usage: 1, contentType: 2});
      self.send @@ LoadSounds(self.state.octave);
      AppState.addEventListener("change", self.handle(handleAppStateChange));
    },
    render: self =>
      <View
        style=Style.(
          style([flex(1.), justifyContent(Center), alignItems(Center)])
        )>
        <OctaveSelector
          position=Style.(
            style([position(Absolute), bottom(Pt(10.)), right(Pt(20.))])
          )
          isMax=(Notes.Octave.isMax(self.state.octave))
          isMin=(Notes.Octave.isMin(self.state.octave))
          handleOctaveChanged=(
            direction => {
              self.send @@ SetPlayState(Paused);
              switch (direction) {
              | Up =>
                self.send @@
                SetOctave(self.state.octave |> Notes.Octave.increment)
              | Down =>
                self.send @@
                SetOctave(self.state.octave |> Notes.Octave.decrement)
              };
            }
          )
        />
        <Tick visible=(self.state.playState != Loading) />
        <RotatableDial
          ticks=Notes.all onRelease=(note => self.send(SetNote(note)))>
          <Dial nShowing=self.state.nShowingNotes />
        </RotatableDial>
        <TouchableOpacity
          style=playPauseStyle
          onPress=(
            () =>
              self.send(
                SetPlayState(
                  switch (self.state.playState) {
                  | Playing => Paused
                  | Paused => Playing
                  | Loading => Loading
                  },
                ),
              )
          )>
          (
            self.state.playState != Loading ?
              <PlayPause state=self.state.playState /> : ReasonReact.null
          )
        </TouchableOpacity>
      </View>,
  };
};
