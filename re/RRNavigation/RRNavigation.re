module History = {
  type push('state) = (string, 'state) => unit;
  type t('state) = {
    action: string,
    index: int,
    length: int,
    canGo: int => bool,
    go: int => unit,
    goBack: unit => unit,
    goForward: unit => unit,
    push: (string, 'state) => unit,
    replace: string => unit
    /* TODO: listen, createHref, location, push with 2nd arg, replace with 2nd arg */
  };
  type js('state) = {
    .
    "action": string,
    "index": int,
    "length": int,
    "canGo": int => bool,
    "go": int => unit,
    "goBack": unit => unit,
    "goForward": unit => unit,
    "push": (string, 'state) => unit,
    "replace": string => unit
    /* TODO: listen, createHref, location, push with 2nd arg, replace with 2nd arg */
  };
  let fromJs = (obj: js('state)) : t('state) => {
    action: obj##action,
    index: obj##index,
    length: obj##length,
    canGo: obj##canGo,
    go: obj##go,
    goBack: obj##goBack,
    goForward: obj##goForward,
    push: obj##push,
    replace: obj##replace
  };
};

module Location = {
  type t('state) = {
    hash: option(string),
    key: string,
    pathname: string,
    search: option(string),
    state: 'state
  };
  type js('state) = {
    .
    "hash": option(string),
    "key": string,
    "pathname": string,
    "search": option(string),
    "state": 'state
  };
  let fromJs = (obj: js('state)) : t('state) => {
    hash: obj##hash,
    key: obj##key,
    pathname: obj##pathname,
    search: obj##search,
    state: obj##state
  };
};

/* TODO: How do we get 'state from navprops if passed into push? */
module NavProps = {
  type t('state) = {
    path: string,
    history: History.t('state),
    location: Location.t('state)
    /* TODO: exact, component, match, render, type */
  };
  type js('state) = {
    .
    "path": string, "history": History.js('state), "location": Location.js('state)
  };
  let fromJs = (obj: js('state)) : t('state) => {
    path: obj##path,
    history: History.fromJs(obj##history),
    location: Location.fromJs(obj##location)
  };
};

module NativeRouter = {
  [@bs.module "react-router-native"] external nativeRouter : ReasonReact.reactClass =
    "NativeRouter";
  let make = (children) =>
    ReasonReact.wrapJsForReason(~reactClass=nativeRouter, ~props=Js.Obj.empty(), children);
};

module Link = {
  [@bs.module "react-router-native"] external link : ReasonReact.reactClass = "Link";
  let make = (component, ~replace: bool=false, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=link,
      ~props={"component": component, "replace": replace},
      children
    );
};

module Card = {
  [@bs.module "react-router-navigation"] external card : ReasonReact.reactClass = "Card";
  let make =
      (
        ~path: string,
        ~exact: bool=false,
        ~component: option(ReasonReact.reactClass)=None,
        ~render: NavProps.t('state) => ReasonReact.reactElement,
        children
      ) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=card,
      ~props={
        "path": path,
        "exact": exact,
        "component": Js.Undefined.from_opt(component),
        "render": (props) => render(NavProps.fromJs(props))
      },
      children
    );
};

module Navigation = {
  [@bs.module "react-router-navigation"] external navigation : ReasonReact.reactClass =
    "Navigation";
  let make = (~hideNavBar: option(bool)=?, children) =>
    ReasonReact.wrapJsForReason(
      ~reactClass=navigation,
      ~props={"hideNavBar": Js.Undefined.from_opt(hideNavBar)},
      children
    );
};
