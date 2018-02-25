[@bs.val] external pi : float = "Math.PI";

type clockIndex = int;
type radius = float;
type circlePosition = {
    x: float,
    y: float
};

let deltaTheta: float = (2. *. pi) /. 12.;

let getTheta = (index: clockIndex) => (float_of_int(index) *. deltaTheta);

let getCirclePosition = (r: radius, index: clockIndex): circlePosition => {
    let theta = getTheta(index);
    {
        x: r *. sin(theta),
        y: r *. cos(theta)
    };
};
