type angle = float;

type length = float;

type point = {
  x: length,
  y: length
};

[@bs.scope "Math"] [@bs.val] external pi : float = "PI";

[@bs.scope "Math"] [@bs.val] external cos : angle => length = "cos";

[@bs.scope "Math"] [@bs.val] external sin : angle => length = "sin";

[@bs.scope "Math"] [@bs.val] external atan2 : (length, length) => angle = "atan2";

let twoPi = 2. *. pi;

let halfPi = 0.5 *. pi;

let dot = (a: point, b: point) => {x: a.x *. b.x, y: a.y *. b.y};

let add = (a: point, b: point) => {x: a.x +. b.x, y: a.y +. b.y};

let coordsOnClock = (~r: length, ~theta: angle) : point =>
  dot({x: r, y: r}, {x: 1. +. sin(theta), y: 1. -. cos(theta)});

let getPrincipalValue = (theta: angle) : angle =>
  mod_float(mod_float(theta, twoPi) +. twoPi, twoPi);

let getClockAngleFromPoint = (~origin: point, {x, y}: point) : angle =>
  getPrincipalValue @@ atan2(x -. origin.x, origin.y -. y);
