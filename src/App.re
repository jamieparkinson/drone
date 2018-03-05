open BsReactNative;

type action =
    | SetNote(Notes.note);

type state = {
    note: Notes.note
};

let ticks = Notes.all
    |> Array.map(Notes.getIndex)
    |> Array.map(Layout.getThetaFromIndex);

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
    ...component,
    initialState: () => {
        note: Aes
    },
    reducer: (action, _state) =>
        switch (action) {
        | SetNote(note) =>
            ReasonReact.Update({ note: note })
        },
    render: (_self) =>
        <View style=Style.(style([flex(1.), justifyContent(Center), alignItems(Center)]))>
            <RotatableDial
                ticks=Notes.all
                onRelease={(note) => Js.log(Notes.getResource(note, 2))}
            >
                <Dial />
            </RotatableDial>
        </View>
};
