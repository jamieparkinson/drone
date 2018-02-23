open BsReactNative;

[@bs.deriving {jsConverter: newType}]
type poolOptions = {
    maxStreams: int,
    usage: int,
    contentType: int
};

type fileId = string;
type soundId = int;

[@bs.deriving {jsConverter: newType}]
type loadResult = {
    file: string,
    soundId: soundId
};

[@bs.deriving {jsConverter: newType}]
type playOptions = {
    leftVolume: float,
    rightVolume: float,
    rate: float,
    priority: int,
    loop: int
};

type nativePool = {
    .
    [@bs.meth] "init": abs_poolOptions => unit,
    [@bs.meth] "load": fileId => Js.Promise.t(abs_loadResult),
    [@bs.meth] "play": (soundId, abs_playOptions) => unit,
    [@bs.meth] "stop": soundId => unit,
    [@bs.meth] "pauseAll": unit => unit,
    [@bs.meth] "resumeAll": unit => unit,
    [@bs.meth] "releasePool": unit => unit,
};

let _pool: nativePool = NativeModules.get("SoundPool");

let init = (options: poolOptions) => options |> poolOptionsToJs |> (o => _pool##init(o));

let loadNote = (file: fileId) => _pool##load(file)
    |> Js.Promise.(then_(value => value |> loadResultFromJs |> resolve));

let loadNotes = (files: array(fileId)) => Array.map((fileId) => _pool##load(fileId), files)
    |> Js.Promise.all
    |> Js.Promise.(then_(value => value |> Array.map(loadResultFromJs) |> resolve));

let play = (sound: soundId, options: playOptions) => options
    |> playOptionsToJs
    |> (o => _pool##play(sound, o));

let pauseAll = () => _pool##pauseAll();
let resumeAll = () => _pool##resumeAll();
let releasePool = () => _pool##releasePool();
