open BsReactNative;

open RNSvg;

let component = ReasonReact.statelessComponent("Dial");

let circleRadius = 115.;

let circleBorderWidth = 8.;

let circlePadding = 8.;

let dialWidth: float = 2. *. circleRadius +. circleBorderWidth;

let dialHeight: float = dialWidth;

let notePosition = (index: int) => {
  let angle =
    float_of_int(index)
    *. Geom.twoPi
    /. float_of_int(Array.length(Notes.all));
  let notesRadius =
    circleRadius -. (circlePadding +. 0.5 *. NoteName.displayMax.width);
  let untranslatedCoords = Geom.coordsOnClock(~r=notesRadius, ~theta=angle);
  let translation = circlePadding +. 0.5 *. circlePadding;
  let coords =
    Geom.add(untranslatedCoords, {x: translation, y: translation});
  let angleString = string_of_float(angle) ++ "rad";
  Style.(
    style([
      position(Absolute),
      top(Pt(coords.y)),
      left(Pt(coords.x)),
      Transform.make(~rotate=angleString, ()),
    ])
  );
};

let make = (~nShowing=12, _children) => {
  ...component,
  render: _self =>
    <View style=Style.(style([position(Relative)]))>
      <Svg width=dialWidth height=dialHeight>
        <Circle
          cx={circleRadius +. 0.5 *. circleBorderWidth}
          cy={circleRadius +. 0.5 *. circleBorderWidth}
          r=circleRadius
          fill="transparent"
          stroke="black"
          strokeWidth=circleBorderWidth
        />
      </Svg>
      {Array.sub(Notes.all, 0, nShowing)
       |> Array.mapi((i, note) =>
            <NoteName
              note
              position={notePosition(i)}
              key={Notes.getName(note)}
            />
          )
       |> ReasonReact.array}
    </View>,
};
