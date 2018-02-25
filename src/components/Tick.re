open RNSvg;

let component = ReasonReact.statelessComponent("Tick");

let height = 18.;
let width = 0.5 *. height;

let shape: array(Polygon.point) = [|
    (0., 0.),
    (width, 0.),
    (0.5 *. width, height)
|];

let make = (_children) => {
    ...component,
    render: (_self) =>
        <Svg
            width=width
            height=height
        >
            <Polygon
                points=shape
                fill="black"
            />
        </Svg>
};
