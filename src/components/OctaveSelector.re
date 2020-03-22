open BsReactNative;

open RNSvg;

let selectorColor = "rgb(205, 205, 205)";

let disabledSelectorColor = "rgb(230, 230, 230)";

let labelColor = "rgb(180, 180, 180)";

let layout = Style.(style([alignItems(Center)]));

let labelStyle =
  Style.(
    style([
      position(Relative),
      top(Pt(41.)),
      left(Pt(0.)),
      color(String(labelColor)),
      fontFamily("Lato-Italic"),
      fontSize(Float(14.))
    ])
  );

type direction =
  | Up
  | Down;

type pressState =
  | In
  | Out;

type action =
  | SetPressState(pressState, direction);

type state = {pressState: option((pressState, direction))};

let component = ReasonReact.reducerComponent("OctaveSelector");

let make =
    (
      ~position,
      ~isMax: bool=false,
      ~isMin: bool=false,
      ~handleOctaveChanged: direction => unit,
      _children
    ) => {
  ...component,
  initialState: () => {pressState: None},
  reducer: (action, state) =>
    switch action {
    | SetPressState(newState, direction) =>
      ReasonReact.Update({pressState: Some((newState, direction))})
    },
  render: (self) =>
    <View style=(Style.combine(position, layout))>
      <Text value="8ve" style=labelStyle />
      <Svg width=30. height=75.>
        <Rect
          x=0.
          y=0.
          width=30.
          height=26.
          fill="transparent"
          onPress=(() => isMax ? () : handleOctaveChanged(Up))
          onPressIn=(() => self.send(SetPressState(In, Up)))
          onPressOut=(() => self.send(SetPressState(Out, Up)))
        />
        <Polygon
          points=[|(15., 4.), (24., 22.), (6., 22.)|]
          fill=(
            self.state.pressState == Some((In, Up)) || isMax ?
              disabledSelectorColor : selectorColor
          )
        />
        <Rect
          x=0.
          y=38.
          width=30.
          height=26.
          fill="transparent"
          onPress=(() => isMin ? () : handleOctaveChanged(Down))
          onPressIn=(() => self.send(SetPressState(In, Down)))
          onPressOut=(() => self.send(SetPressState(Out, Down)))
        />
        <Polygon
          points=[|(6., 42.), (24., 42.), (15., 60.)|]
          fill=(
            self.state.pressState == Some((In, Down)) || isMin ?
              disabledSelectorColor : selectorColor
          )
        />
      </Svg>
    </View>
};
