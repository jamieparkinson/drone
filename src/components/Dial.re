open RNSvg;

let component = ReasonReact.statelessComponent("Dial");

let circleRadius = 115.;
let circleBorderWidth = 8.;

let make = (_children) => {
    ...component,
    render: (_self) =>
        <Svg
            width={(2. *. circleRadius) +. circleBorderWidth}
            height={(2. *. circleRadius) +. circleBorderWidth}
        >
            <Circle
                cx={circleRadius +. (0.5 *. circleBorderWidth)}
                cy={circleRadius +. (0.5 *. circleBorderWidth)}
                r=circleRadius
                fill="transparent"
                stroke="black"
                strokeWidth=circleBorderWidth
            />
        </Svg>
};
