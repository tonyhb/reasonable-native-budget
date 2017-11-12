
type history = {
	action: string,
	index: int,
	length: int,
	canGo: (int) => bool,
	go: (int) => unit,
	goBack: unit => unit,
	goForward: unit => unit,
	push: (string) => unit,
	replace: (string) => unit,
	/* TODO: listen, createHref, location, push with 2nd arg, replace with 2nd arg */
};

type navProps = {
	path: string,
	history,
};

module NativeRouter = {
	[@bs.module "react-router-native"] external nativeRouter : ReasonReact.reactClass =
		"NativeRouter";
	let make = (children) => ReasonReact.wrapJsForReason(~reactClass=nativeRouter, ~props=Js.Obj.empty(), children);
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
				~render: (history => ReasonReact.reactElement),
				children
			) =>
		ReasonReact.wrapJsForReason(
			~reactClass=card,
			~props={
				"path": path,
				"exact": exact,
				"component": Js.Undefined.from_opt(component),
				"render": render
			},
			children
		);
};

module Navigation = {
	[@bs.module "react-router-navigation"] external navigation : ReasonReact.reactClass = "Navigation";
	let make = (~hideNavBar: option(bool)=?, children) =>
		ReasonReact.wrapJsForReason(
			~reactClass=navigation,
			~props={
				"hideNavBar": Js.Undefined.from_opt(hideNavBar)
			},
			children
		);
};