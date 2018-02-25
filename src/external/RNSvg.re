module Svg = {
    [@bs.module "react-native-svg"] external svg: ReasonReact.reactClass = "Svg";
    let make = (~width: float, ~height: float, children) =>
        ReasonReact.wrapJsForReason(
            ~reactClass=svg,
            ~props=Js.Nullable.({
                "width": width,
                "height": height
            }),
            children
        );
};

module Circle = {
    [@bs.module "react-native-svg"] external circle: ReasonReact.reactClass = "Circle";
    let make = (
        ~cx: float,
        ~cy: float,
        ~r: float,
        ~fill: option(string)=?,
        ~stroke: option(string)=?,
        ~strokeWidth: option(float)=?,
        children
    )=>
        ReasonReact.wrapJsForReason(
            ~reactClass=circle,
            ~props=Js.Nullable.({
                "cx": cx,
                "cy": cy,
                "r": r,
                "fill": from_opt(fill),
                "stroke": from_opt(stroke),
                "strokeWidth": from_opt(strokeWidth)
            }),
            children
        );
};
