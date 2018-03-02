open BsReactNative;
open RNSvg;

let component = ReasonReact.statelessComponent("Dial");

let circleRadius = 115.;
let circleBorderWidth = 8.;
let textSize = 25.;
let circlePosition = Layout.getCirclePosition(circleRadius -. textSize);
let dialWidth: float = (2. *. circleRadius) +. circleBorderWidth;
let dialHeight: float = dialWidth;

let noteNameStyle = (index: Layout.clockIndex) => {
    let { x, y }: Layout.circlePosition = circlePosition(index);
    let rotation = (Layout.getThetaFromIndex(index) |> string_of_float) ++ "rad";

    let topPosition = (circleRadius -. 9.) -. y;
    let leftPosition = (circleRadius -. 9.) +. x;

    Style.(style([
        position(Absolute),
        width(Pt(textSize)),
        height(Pt(textSize)),
        lineHeight(textSize),
        textAlign(Center),
        color("black"),
        fontSize(Float(textSize)),
        top(Pt(topPosition)),
        left(Pt(leftPosition)),
        Transform.make(
            ~rotate=rotation,
            ()
        )
    ]));
};

let make = (_children) => {
    ...component,
    render: (_self) =>
        <View style={Style.(style([ position(Relative) ]))}>
            <Svg
                width=dialWidth
                height=dialHeight
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

            {Notes.all
                |> Array.map(Notes.getDisplayString)
                |> Array.mapi((i, note) =>
                    <Text
                        key=note
                        value=note
                        style=noteNameStyle(i)
                    />
                )
                |> ReasonReact.arrayToElement
            }
        </View>
};
