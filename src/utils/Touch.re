open BsReactNative;

type angle = AnimatedRe.Value.t;
type ticks = array(float);
type tickRecord = {
    value: float,
    index: int
};
type releaseHandler = int => unit;

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
        ~handleRelease: option(releaseHandler)=?,
        _event,
        { moveX, moveY }: PanResponder.gestureState
    ) => {
    let currentTouchAngle = Layout.getThetaFromPosition(origin, { x: moveX, y: moveY });
    let releaseAngle = Layout.getPrincipalValue(currentTouchAngle +. offset_^);
    let getDistanceToTick = Layout.angleDistance(releaseAngle);

    let closestTick = ticks
        |> Array.mapi((i, tick) => { value: tick, index: i })
        |> Array.fold_left(
            (closestTick, tick) => {
                let isSmaller = getDistanceToTick(tick.value) < getDistanceToTick(closestTick.value);
                isSmaller ? tick : closestTick;
            },
            { index: -1, value: max_float }
        );

    let handleRelease = switch(handleRelease) {
    | None => (_n) => ()
    | Some(callback) => callback
    };

    let minTick = Array.fold_left(min, max_float, ticks);
    let closestTickAngle = closestTick.value == minTick && releaseAngle > Layout.pi
        ? (2. *. Layout.pi) +. minTick
        : closestTick.value;

    let springAnimation = Animated.Value.Spring.animate(
        ~value=angle,
        ~toValue=`raw(closestTickAngle),
        ~speed=20.,
        ~onComplete=(result) => Js.to_bool(result##finished) ? handleRelease(closestTick.index) : (),
        ()
    );
    Animated.CompositeAnimation.start(springAnimation, ());

    releaseAngle_ := closestTick.value;
};

let panResponderAboutOrigin = (~origin: Layout.position, ~angle: angle, ~ticks: ticks, ~onRelease: option(releaseHandler)=?, ()) =>
    PanResponder.create(
        ~onStartShouldSetPanResponder=PanResponder.callback(handleStart(~origin=origin, ~angle=angle)),
        ~onPanResponderRelease=PanResponder.callback(handleRelease(~origin=origin, ~angle=angle, ~ticks=ticks, ~handleRelease=?onRelease)),
        ~onPanResponderMove=`callback(PanResponder.callback(handleMove(~origin=origin, ~angle=angle))),
        ()
    );
