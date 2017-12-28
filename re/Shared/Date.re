/*
   month is a yyyy, mm float tuple.  We use float because bucklescript
   provides support for using floats only.

   Note that months are zero indexed and can be greater than 11; these
   are wrapped modulu 11 and increase the year.
 */
type month = (float, float);

let dateOfMonth = (d: month) : Js.Date.t => {
  let (y, m) = d;
  Js.Date.makeWithYM(~year=y, ~month=m, ())
};

let isBeforeMonth = (date: Js.Date.t, d: month) : bool => {
  let comparator = dateOfMonth(d);
  date < comparator
};

let isAfterMonth = (date: Js.Date.t, d: month) : bool => ! isBeforeMonth(date, d);
