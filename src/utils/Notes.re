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

type octave = int;

let all = [|
    C,
    Cis,
    D,
    Ees,
    E,
    F,
    Fis,
    G,
    Aes,
    A,
    Bes,
    B
|];

let getResource = (note, octave) => {
    let name =
        switch (note) {
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
    let suffix = string_of_int(octave);

    name ++ suffix;
};

let getDisplayString = (note) =>
    switch (note) {
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
