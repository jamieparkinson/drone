open BsReactNative;

open RNSvg;

let selectorColor = "rgb(205, 205, 205)";
let labelColor = "rgb(180, 180, 180)";

let layout = Style.(style([alignItems(Center)]));

let labelStyle =
  Style.(
    style([
      color(labelColor),
      marginBottom(Pt(5.)),
      fontFamily("Lato-Regular"),
      fontSize(Float(15.))
    ])
  );

type action =
  | OctaveUp
  | OctaveDown;

let component = ReasonReact.statelessComponent("OctaveSelector");

let make = (~position, ~handleOctaveChanged, _children) => {
  ...component,
  render: (_self) =>
    <View style=(Style.combine(position, layout))>
      <Text value="8ve" style=labelStyle />
      <Svg width=24. height=56.>
        <Polygon points=[|(12., 0.), (24., 26.), (0., 26.)|] fill=selectorColor />
        <Polygon points=[|(0., 30.), (24., 30.), (12., 56.)|] fill=selectorColor />
      </Svg>
    </View>
};
