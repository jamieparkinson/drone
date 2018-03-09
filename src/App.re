open BsReactNative;

type action =
  | PlayNote(Notes.note)
  | SetSounds(Notes.association)
  | LoadSounds(Notes.octave);

type state = {
  note: Notes.note,
  sounds: Notes.association
};

let ticks = Notes.all |> Array.map(Notes.getIndex) |> Array.map(Layout.getThetaFromIndex);

let component = ReasonReact.reducerComponent("App");

let playNote = (self, note: Notes.note) => self.ReasonReact.send @@ PlayNote(note);

let playSound =
  SoundPool.play({loop: (-1), leftVolume: 1.0, rightVolume: 1.0, rate: 1.0, priority: 1});

let make = (_children) => {
  ...component,
  initialState: () => {note: C, sounds: []},
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
    | PlayNote(note) =>
      ReasonReact.UpdateWithSideEffects(
        {...state, note},
        (({state}) => playSound @@ Notes.getSound(note, state.sounds))
      )
    },
  didMount: (self) => {
    SoundPool.init({
      maxStreams: 1,
      usage: 1,
      contentType: 2
    });
    self.send @@ LoadSounds(2);
    ReasonReact.NoUpdate
  },
  render: (self) =>
    <View style=Style.(style([flex(1.), justifyContent(Center), alignItems(Center)]))>
      <Tick />
      <RotatableDial ticks=Notes.all onRelease=(playNote(self))> <Dial /> </RotatableDial>
    </View>
};
