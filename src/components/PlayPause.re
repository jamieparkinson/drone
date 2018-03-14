open BsReactNative;

open RNSvg;

type playState =
  | Playing
  | Paused
  | Loading;

let component = ReasonReact.statelessComponent("PlayPause");

let playWidth = 25.;

let playHeight = 29.;

let pauseBarWidth = 7.;

let pauseBarHeight = 24.;

let playIcon: array(Polygon.point) = [|
  (0., 0.),
  (playWidth, 0.5 *. playHeight),
  (0., playHeight)
|];

let make = (~x=0., ~y=0., ~state=Paused, _children) => {
  ...component,
  render: (_self) =>
    <Svg width=30. height=30.>
      (
        switch state {
        | Paused => <Polygon points=playIcon fill="black" />
        | Playing =>
          <G>
            <Rect x=0. y=2.5 width=pauseBarWidth height=pauseBarHeight fill="black" />
            <Rect
              x=(1.75 *. pauseBarWidth)
              y=2.5
              width=pauseBarWidth
              height=pauseBarHeight
              fill="black"
            />
          </G>
        }
      )
    </Svg>
};
