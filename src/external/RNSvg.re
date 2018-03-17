module Svg = {
  [@bs.module "react-native-svg"] external svg : ReasonReact.reactClass = "Svg";
  let make = (~width: float, ~height: float, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=svg,
      ~props={"width": width, "height": height},
      children
    );
};

module Circle = {
  [@bs.module "react-native-svg"] external circle : ReasonReact.reactClass = "Circle";
  let make =
      (
        ~cx: float,
        ~cy: float,
        ~r: float,
        ~fill: option(string)=?,
        ~stroke: option(string)=?,
        ~strokeWidth: option(float)=?,
        children
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=circle,
      ~props=
        Js.Nullable.(
          {
            "cx": cx,
            "cy": cy,
            "r": r,
            "fill": from_opt(fill),
            "stroke": from_opt(stroke),
            "strokeWidth": from_opt(strokeWidth)
          }
        ),
      children
    );
};

module Polygon = {
  [@bs.module "react-native-svg"] external polygon : ReasonReact.reactClass = "Polygon";
  type point = (float, float);
  let pointsToPathString = (points: array(point)) =>
    points
    |> Array.map(((x, y)) => string_of_float(x) ++ "," ++ string_of_float(y))
    |> Array.fold_left((path, point) => path ++ " " ++ point, "");
  let make =
      (
        ~points: array(point),
        ~fill: option(string)=?,
        ~stroke: option(string)=?,
        ~strokeWidth: option(float)=?,
        ~onPress: option(unit => unit)=?,
        ~onPressIn: option(unit => unit)=?,
        ~onPressOut: option(unit => unit)=?,
        children
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=polygon,
      ~props=
        Js.Nullable.(
          {
            "points": pointsToPathString(points),
            "fill": from_opt(fill),
            "stroke": from_opt(stroke),
            "strokeWidth": from_opt(strokeWidth),
            "onPress": from_opt(onPress),
            "onPressIn": from_opt(onPressIn),
            "onPressOut": from_opt(onPressOut)
          }
        ),
      children
    );
};

module G = {
  [@bs.module "react-native-svg"] external g : ReasonReact.reactClass = "G";
  let make = (~x=0., ~y=0., ~translate: option(string)=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=g,
      ~props=Js.Nullable.({"x": x, "y": y, "translate": from_opt(translate)}),
      children
    );
};

module Rect = {
  [@bs.module "react-native-svg"] external rect : ReasonReact.reactClass = "Rect";
  let make =
      (
        ~x: float,
        ~y: float,
        ~width: float,
        ~height: float,
        ~fill: option(string)=?,
        ~stroke: option(string)=?,
        ~strokeWidth: option(float)=?,
        ~onPress: option(unit => unit)=?,
        ~onPressIn: option(unit => unit)=?,
        ~onPressOut: option(unit => unit)=?,
        children
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=rect,
      ~props=
        Js.Nullable.(
          {
            "x": x,
            "y": y,
            "width": width,
            "height": height,
            "fill": from_opt(fill),
            "stroke": from_opt(stroke),
            "strokeWidth": from_opt(strokeWidth),
            "onPress": from_opt(onPress),
            "onPressIn": from_opt(onPressIn),
            "onPressOut": from_opt(onPressOut)
          }
        ),
      children
    );
};
