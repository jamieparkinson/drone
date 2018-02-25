open RNSvg;

let component = ReasonReact.statelessComponent("Tick");

let height = 18.;
let width = 0.5 *. height;

let make = (_children) => {
    ...component,
    render: (_self) =>
        <Svg
            width=width
            height=height
        >
            <Polygon
                points=[|
                    (0., 0.),
                    (width, 0.),
                    (0.5 *. width, height)
                |]
                fill="black"
            />
        </Svg>
};
