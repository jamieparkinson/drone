open BsReactNative;

type angle = AnimatedRe.Value.t;
type ticks = array(float);

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

    gesture.numberActiveTouches == 1;
};

let handleMove =
    (
        ~origin: Layout.position,
        ~angle: angle,
        _event,
        { moveX, moveY, x0, y0 }: PanResponder.gestureState
    ) => {
    let currentTouchAngle = Layout.getThetaFromPosition(origin, { x: moveX, y: moveY });
    let newAngle = Layout.getPrincipalValue(currentTouchAngle +. offset_^);

    AnimatedRe.Value.setValue(angle, newAngle);
};

let handleRelease =
    (
        ~origin: Layout.position,
        ~angle: angle,
        ~ticks: ticks,
        _event,
        { moveX, moveY }: PanResponder.gestureState
    ) => {
    let currentTouchAngle = Layout.getThetaFromPosition(origin, { x: moveX, y: moveY });
    let releaseAngle = Layout.getPrincipalValue(currentTouchAngle +. offset_^);

    let closestTick = ticks
        |> Array.fold_left(
            (closestTick, tick) => {
                let isSmaller = abs_float(releaseAngle -. tick) < abs_float(releaseAngle -. closestTick);
                isSmaller ? tick : closestTick;
            },
            0.
        );

    Js.log(releaseAngle *. 57.2958);

    let springAnimation = Animated.Value.Spring.animate(
        ~value=angle,
        ~toValue=`raw(closestTick),
        ()
    );
    Animated.CompositeAnimation.start(springAnimation, ());

    releaseAngle_ := closestTick;
};

let panResponderAboutOrigin = (~origin: Layout.position, ~angle: angle, ~ticks: ticks) =>
    PanResponder.create(
        ~onStartShouldSetPanResponder=PanResponder.callback(handleStart(~origin=origin, ~angle=angle)),
        ~onPanResponderRelease=PanResponder.callback(handleRelease(~origin=origin, ~angle=angle, ~ticks=ticks)),
        ~onPanResponderMove=`callback(PanResponder.callback(handleMove(~origin=origin, ~angle=angle))),
        ()
    );
