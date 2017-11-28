let days = [|"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"|];

let months = [|
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "May",
  "Jun",
  "Jul",
  "Aug",
  "Sep",
  "Oct",
  "Nov",
  "Dec"
|];

let long = (date: Js.Date.t) => {
  open Js.Date;
  let day = getDate(date) |> Printf.sprintf("%.0f");
  let month = months[getMonth(date) |> int_of_float];
  let year = getFullYear(date) |> Printf.sprintf("%.0f");
  day ++ " " ++ month ++ " " ++ year
};
