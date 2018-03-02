[@bs.val] external pi: float = "Math.PI";
[@bs.val] external atan2: (float, float) => float = "Math.atan2";

type position = {
    x: float,
    y: float
};

type clockIndex = int;
type radius = float;
type circlePosition = position;

let deltaTheta: float = (2. *. pi) /. 12.;

let getThetaFromIndex = (index: clockIndex) => (float_of_int(index) *. deltaTheta);

let getCirclePosition = (r: radius, index: clockIndex): circlePosition => {
    let theta = getThetaFromIndex(index);
    {
        x: r *. sin(theta),
        y: r *. cos(theta)
    };
};

let getThetaFromPosition = (origin: position, position: position) => {
    let deltaX = position.x -. origin.x;
    let deltaY = origin.y -. position.y;

    let theta = -.atan2(deltaY, deltaX) +. (pi /. 2.);
    let positiveTheta = if (theta < 0.) {
        theta +. (2. *. pi);
    } else if (theta > (2. *. pi)) {
        theta -. (2. *. pi);
    } else {
        theta;
    };

    positiveTheta;
};
