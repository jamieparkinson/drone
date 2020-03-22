open RNSvg;

let component = ReasonReact.statelessComponent("Tick");

let height = 19.;

let width = 0.5 *. height;

let shape: array(Polygon.point) = [|
  (0., 0.),
  (width, 0.),
  (0.5 *. width, height -. 3.),
|];

let make = (~visible=true, _children) => {
  ...component,
  render: _self =>
    <Svg width height>
      (visible ? <Polygon points=shape fill="black" /> : ReasonReact.null)
    </Svg>,
};
