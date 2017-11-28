[@bs.scope "DatePickerAndroid"] [@bs.module "react-native"] external dateSet : string =
  "dateSetAction";

[@bs.scope "DatePickerAndroid"] [@bs.module "react-native"] external dismissed : string =
  "dismissedAction";

type response = {
  year: int,
  month: int,
  day: int
};

type action =
  | Dismissed
  | Set(response);

type mode =
  | Calendar
  | Spinner
  | Default;

let open_:
  (~date: Js.Date.t, ~minDate: Js.Date.t=?, ~maxDate: Js.Date.t=?, ~mode: mode=?, unit) =>
  Js.Promise.t(action);
