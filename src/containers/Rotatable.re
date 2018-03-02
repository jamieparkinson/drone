open BsReactNative;

type action =
    | SetOrigin(Layout.position);

type state = {
    panResponder: PanResponder.t,
    angle: Touch.angle,
};

let component = ReasonReact.reducerComponent("Rotatable");

let handleLayout = (event, self) => {
    let layout = event |> RNEvent.NativeLayoutEvent.layout;
    SetOrigin({
        x: layout.x +. 0.5 *. layout.width,
        y: layout.y +. 0.5 *. layout.height
    }) |> self.ReasonReact.send;
};

let rotatingViewStyle = (angle: Touch.angle) => Style.(style([
    Transform.makeInterpolated(
        ~rotate=
            Animated.Value.interpolate(
                angle,
                ~inputRange=[0., 2. *. Layout.pi],
                ~outputRange=`string(["0deg", "360deg"]),
                ()
            ),
        ()
    )
]));

let make = (~ticks: Touch.ticks, children) => {
    ...component,
    initialState: () => {
        let angle = AnimatedRe.Value.create(0.);
        AnimatedRe.Value.addListener(angle, (v) => Js.log(v##value *. 57.2958));
        {
            panResponder: Touch.panResponderAboutOrigin(~origin={ x: 0., y: 0. }, ~angle=angle, ~ticks=ticks),
            angle
        };
    },
    reducer: (action, state) =>
        switch (action) {
        | SetOrigin(origin) => ReasonReact.Update({
            ...state,
            panResponder: Touch.panResponderAboutOrigin(
                ~origin=origin,
                ~angle=state.angle,
                ~ticks=ticks
            )
         })
        },
    render: (self) =>
        <View
            responderHandlers=PanResponder.panHandlers(self.state.panResponder)
            onLayout=self.handle(handleLayout)
        >
            <Animated.View style=rotatingViewStyle(self.state.angle)>
                ...children
            </Animated.View>
        </View>
};
