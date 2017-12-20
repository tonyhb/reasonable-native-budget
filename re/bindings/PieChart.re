type value = {
  value: float,
  label: string
};

type valueJs = {. "value": float, "label": string};

let values = (vals: array(value)) =>
  vals |> Array.map((v) => {"value": v.value, "label": v.label});

type pieChartDataConfigJs = {
  .
  "colors": array(string),
  "drawValues": bool,
  "highlightEnabled": bool,
  "valueTextSize": Js.Nullable.t(float),
  "valueTextColor": Js.Nullable.t(string),
  "visible": Js.boolean,
  "selectionShift": float, /* Pie only */
  "sliceSpace": float /* Pie only */
};

let config = (
      ~colors: array(string),
      ~drawValues=true,
      ~highlightEnabled=true,
      ~valueTextSize: option(float)=?,
      ~valueTextColor: option(string)=?,
      ~visible=true,
      ~sliceSpace=0.,
      ~selectionShift=0.,
      ()): pieChartDataConfigJs => {
  "colors": colors |> Array.map((v) => BsReactNative.StyleSheet.processColor(`String(v))),
  "drawValues": drawValues,
  "highlightEnabled": highlightEnabled,
  "valueTextSize": Js.Nullable.from_opt(valueTextSize),
  "valueTextColor": Js.Nullable.from_opt(valueTextColor),
  "visible": Js.Boolean.to_js_boolean(visible),
  "selectionShift": selectionShift,
  "sliceSpace": sliceSpace
};

type pieChartDataJs = {
  .
  "values": array(valueJs),
  "label": Js.Nullable.t(string),
  "config": Js.Nullable.t(pieChartDataConfigJs)
};

let pieChartData =
    (~label: option(string), ~config: option(pieChartDataConfigJs), ~data: array(valueJs))
    : pieChartDataJs => {
  "values": data,
  "label": Js.Nullable.from_opt(label),
  "config": Js.Nullable.from_opt(config)
};

[@bs.module "react-native-charts-wrapper"] external pieChart : ReasonReact.reactClass = "PieChart";

let make =
    (
      ~data: array(value),
      ~label=?,
      ~config: option(pieChartDataConfigJs)=?,
      /* These props are added to the dataSet's config */

      /* These props are passed to the component */
      ~holeRadius=0,
      ~holeColor=?,
      ~transparentCircleRadius=0,
      ~transparentCircleColor=?,
      ~rotationEnabled=false,
      ~rotationAngle=0,
      ~drawSliceText=true,
      ~usePercentValues=false,
      ~style,
      children
    ) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=pieChart,
    ~props={
      "data": {"dataSets": [|pieChartData(~label, ~config, ~data=values(data))|]},
      "style": style,
      "holeRadius": holeRadius,
      "holeColor": Js.Nullable.from_opt(holeColor),
      "transparentCircleRadius": transparentCircleRadius,
      "transparentCircleColor": Js.Nullable.from_opt(transparentCircleColor),
      "rotationEnabled": Js.Boolean.to_js_boolean(rotationEnabled),
      "rotationAngle": rotationAngle,
      "drawSliceText": Js.Boolean.to_js_boolean(drawSliceText),
      "usePercentValues": Js.Boolean.to_js_boolean(usePercentValues)
      /**
        VALUES THAT BREAK IOS
        chartDescription
        drawEntryLabels
      **/
      /*"drawEntryLabels": Js.Boolean.to_js_boolean(drawEntryLabels): TODO: BREAKS iOS */
    },
    children
  );
