[@bs.deriving {jsConverter: newType}]
type assetSource = {
  width: float,
  height: float,
  uri: string,
  scale: float,
  __packager_asset: bool
};

[@bs.module]
external js_resolveAssetSource : BsReactNative.Packager.required => abs_assetSource =
  "react-native/Libraries/Image/resolveAssetSource";

let resolveAssetSource = (asset: BsReactNative.Packager.required) => assetSourceFromJs @@ js_resolveAssetSource(asset);
