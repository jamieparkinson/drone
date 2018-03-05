open BsReactNative;

type angle = AnimatedRe.Value.t;

type ticks('a) = array('a);

type mappedTick('a) = {
  tick: 'a,
  angle: float
};

type tickRecord = {
  value: float,
  index: int
};

type releaseHandler('a) = 'a => unit;

let releaseAngle_ = ref(0.);

let offset_ = ref(0.);

let handleStart =
    (~origin: Layout.position, event: RNEvent.NativeEvent.t, gesture: PanResponder.gestureState) => {
  let x0 = RNEvent.NativeEvent.pageX(event);
  let y0 = RNEvent.NativeEvent.pageY(event);
  let startAngle = Layout.getThetaFromPosition(origin, {x: x0, y: y0});
  offset_ := releaseAngle_^ -. startAngle;
  gesture.numberActiveTouches == 1
};

let handleMove =
    (~origin: Layout.position, ~angle: angle, _event, {moveX, moveY}: PanResponder.gestureState) => {
  let currentTouchAngle = Layout.getThetaFromPosition(origin, {x: moveX, y: moveY});
  let newAngle = Layout.getPrincipalValue(currentTouchAngle +. offset_^);
  AnimatedRe.Value.setValue(angle, newAngle)
};

let handleRelease =
    (
      ~origin: Layout.position,
      ~angle: angle,
      ~ticks: ticks(mappedTick('a)),
      ~handleRelease: option(releaseHandler('a))=?,
      _event,
      {moveX, moveY}: PanResponder.gestureState
    ) => {
  let currentTouchAngle = Layout.getThetaFromPosition(origin, {x: moveX, y: moveY});
  let releaseAngle = Layout.getPrincipalValue(currentTouchAngle +. offset_^);
  let getDistanceToTick = Layout.angleDistance(releaseAngle);
  let closestTick: mappedTick('a) =
    ticks
    |> Array.fold_left(
         (closestTick, tick) => {
           let isSmaller = getDistanceToTick(tick.angle) < getDistanceToTick(closestTick.angle);
           isSmaller ? tick : closestTick
         },
         ticks[0]
       );
  let callback =
    switch handleRelease {
    | None => ((_n: 'a) => ())
    | Some(callback) => callback
    };
  let closestTickAngle =
    closestTick.angle == 0. && releaseAngle > Layout.pi ? 2. *. Layout.pi : closestTick.angle;
  let springAnimation =
    Animated.Value.Spring.animate(
      ~value=angle,
      ~toValue=`raw(closestTickAngle),
      ~speed=20.,
      ~onComplete=(result) => Js.to_bool(result##finished) ? callback(closestTick.tick) : (),
      ()
    );
  Animated.CompositeAnimation.start(springAnimation, ());
  releaseAngle_ := closestTick.angle
};

let panResponderAboutOrigin =
    (
      ~origin: Layout.position,
      ~angle: angle,
      ~ticks: ticks('a),
      ~onRelease: option(releaseHandler('a))=?,
      ()
    ) => {
  let mappedTicks: ticks(mappedTick('a)) =
    Array.mapi((i, tick) => {angle: Layout.(2. *. pi -. getThetaFromIndex(i)), tick}) @@ ticks;
  PanResponder.create(
    ~onStartShouldSetPanResponder=PanResponder.callback(handleStart(~origin)),
    ~onPanResponderRelease=
      PanResponder.callback(
        handleRelease(~origin, ~angle, ~ticks=mappedTicks, ~handleRelease=?onRelease)
      ),
    ~onPanResponderMove=`callback(PanResponder.callback(handleMove(~origin, ~angle))),
    ()
  )
};
