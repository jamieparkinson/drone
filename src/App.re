open BsReactNative;

type action =
    | SetNote(Notes.note);

type state = {
    note: Notes.note
};

let component = ReasonReact.reducerComponent("App");

let make = (_children) => {
    ...component,
    initialState: () => {
        note: Aes
    },
    reducer: (action, state) =>
        switch (action) {
        | SetNote(note) =>
            ReasonReact.Update({ note: note })
        },
    render: ({ state }) =>
        <View style=Style.(style([flex(1.), justifyContent(Center), alignItems(Center)]))>
            <Rotatable>
                <Dial />
            </Rotatable>
        </View>
};
