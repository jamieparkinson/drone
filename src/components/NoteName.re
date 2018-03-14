open BsReactNative;

type dims = {
  width: float,
  height: float
};

let fileDimsMax =
  Notes.all
  |> Array.map(Notes.getImage)
  |> Array.map(RNPrivate.resolveAssetSource)
  |> Array.fold_left(
       (maxDims: dims, asset: RNPrivate.assetSource) => {
         width: max(asset.width, maxDims.width),
         height: max(asset.height, maxDims.height)
       },
       {width: 0., height: 0.}
     );

let displayMax = {width: 22., height: 22.};

let getNormalised = (norm: float, max: float, input: float) => norm *. (input /. max);

let normalisedWidth = getNormalised(displayMax.width, fileDimsMax.width);

let normalisedHeight = getNormalised(displayMax.height, fileDimsMax.height);

let wrapperStyle =
  Style.(
    style([
      width(Pt(displayMax.width)),
      height(Pt(displayMax.height)),
      alignItems(Center),
      justifyContent(FlexEnd)
    ])
  );

let imageStyle = (asset: RNPrivate.assetSource) =>
  Style.(
    style([
      resizeMode(Contain),
      width(Pt(normalisedWidth(asset.width))),
      height(Pt(normalisedHeight(asset.height)))
    ])
  );

let component = ReasonReact.statelessComponent("NoteName");

let make = (~note: Notes.note, ~position: Style.t, _children) => {
  ...component,
  render: (_self) =>
    <View style=Style.combine(wrapperStyle, position)>
      <Image
        source=(Required(Notes.getImage(note)))
        style=(note |> Notes.getImage |> RNPrivate.resolveAssetSource |> imageStyle)
      />
    </View>
};
