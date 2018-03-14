type note =
  | C
  | Cis
  | D
  | Ees
  | E
  | F
  | Fis
  | G
  | Aes
  | A
  | Bes
  | B;

type sound = {
  soundId: SoundPool.soundId,
  note
};

type association = list((note, int));

type octave = int;

let all = [|C, Cis, D, Ees, E, F, Fis, G, Aes, A, Bes, B|];

let getIndex = (note) =>
  switch note {
  | C => 0
  | Cis => 1
  | D => 2
  | Ees => 3
  | E => 4
  | F => 5
  | Fis => 6
  | G => 7
  | Aes => 8
  | A => 9
  | Bes => 10
  | B => 11
  };

let getName = (note) =>
  switch note {
  | C => "c"
  | Cis => "cis"
  | D => "d"
  | Ees => "ees"
  | E => "e"
  | F => "f"
  | Fis => "fis"
  | G => "g"
  | Aes => "aes"
  | A => "a"
  | Bes => "bes"
  | B => "b"
  };

let getImage = (note) =>
  switch note {
  | C => BsReactNative.Packager.require("../../../../static/c.png")
  | Cis => BsReactNative.Packager.require("../../../../static/cis.png")
  | D => BsReactNative.Packager.require("../../../../static/d.png")
  | Ees => BsReactNative.Packager.require("../../../../static/ees.png")
  | E => BsReactNative.Packager.require("../../../../static/e.png")
  | F => BsReactNative.Packager.require("../../../../static/f.png")
  | Fis => BsReactNative.Packager.require("../../../../static/fis.png")
  | G => BsReactNative.Packager.require("../../../../static/g.png")
  | Aes => BsReactNative.Packager.require("../../../../static/aes.png")
  | A => BsReactNative.Packager.require("../../../../static/a.png")
  | Bes => BsReactNative.Packager.require("../../../../static/bes.png")
  | B => BsReactNative.Packager.require("../../../../static/b.png")
  };

let getResource = (octave, note) => getName(note) ++ string_of_int(octave);

let getDisplayString = (note) =>
  switch note {
  | C => {js|C|js}
  | Cis => {js|C♯|js}
  | D => {js|D|js}
  | Ees => {js|E♭|js}
  | E => {js|E|js}
  | F => {js|F|js}
  | Fis => {js|F♯|js}
  | G => {js|G|js}
  | Aes => {js|A♭|js}
  | A => {js|A|js}
  | Bes => {js|B♭|js}
  | B => {js|B|js}
  };

type progressCallback = unit => unit;

let loadAssociation = (octave, notes: array(note), ~onProgress: option(progressCallback)=?) =>
  notes
  |> Array.map(getResource(octave))
  |> Array.map(SoundPool.load)
  |> Array.map(
       Js.Promise.(
         then_(
           (result) => {
             switch onProgress {
             | Some(callback) => callback()
             | None => ()
             };
             resolve(result)
           }
         )
       )
     )
  |> Js.Promise.all
  |> Js.Promise.(
       then_(
         (sounds) =>
           sounds
           |> Array.map(({soundId}: SoundPool.loadResult) => soundId)
           |> Array.to_list
           |> List.combine(Array.to_list(notes))
           |> resolve
       )
     );

let getSound = (note, soundAssociation: association) => List.assoc(note, soundAssociation);
