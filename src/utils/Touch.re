open BsReactNative;

type angle = AnimatedRe.Value.t;

let releaseAngle_ = ref(0.);
let offset_ = ref(0.);

let handleStart =
    (
        ~origin: Layout.position,
        ~angle: angle,
        event: RNEvent.NativeEvent.t,
        gesture: PanResponder.gestureState
    ) => {
    let x0 = RNEvent.NativeEvent.pageX(event);
    let y0 = RNEvent.NativeEvent.pageY(event);
    let startAngle = Layout.getThetaFromPosition(origin, { x: x0, y: y0 });

    offset_ := releaseAngle_^ -. startAngle;
    AnimatedRe.Value.setOffset(angle, offset_^);

    gesture.numberActiveTouches == 1;
};

/* TODO: this should allow the dial to spring to its closest note position */
let handleRelease =
    (
        ~origin: Layout.position,
        ~angle: angle,
        _event,
        { moveX, moveY }: PanResponder.gestureState
    ) => {
    AnimatedRe.Value.flattenOffset(angle);
    let currentAngle = Layout.getThetaFromPosition(origin, { x: moveX, y: moveY });
    releaseAngle_ := currentAngle +. offset_^;
};

let handleMove =
    (
        ~origin: Layout.position,
        ~angle: angle,
        _event,
        { moveX, moveY, x0, y0 }: PanResponder.gestureState
    ) => {
    let currentAngle = Layout.getThetaFromPosition(origin, { x: moveX, y: moveY });
    AnimatedRe.Value.setValue(angle, currentAngle);
};

let panResponderAboutOrigin = (~origin: Layout.position, ~angle: angle) =>
    PanResponder.create(
        ~onStartShouldSetPanResponder=PanResponder.callback(handleStart(~origin=origin, ~angle=angle)),
        ~onPanResponderRelease=PanResponder.callback(handleRelease(~origin=origin, ~angle=angle)),
        ~onPanResponderMove=`callback(PanResponder.callback(handleMove(~origin=origin, ~angle=angle))),
        ()
    );
