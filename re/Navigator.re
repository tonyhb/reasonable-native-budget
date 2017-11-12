
type globalArgs = {
	budget: array(Budget.group),
};

let initialArgs = {
	budget: [||]
};

let c = ReasonReact.statelessComponent("Navigator");

let make = (~budget, _children) => {
	...c,
	render: _self => 
		<RRNavigation.NativeRouter>
			<RRNavigation.Navigation hideNavBar=true>
				<RRNavigation.Card
					exact=true
					path="/"
					render=(({ push }) => <Home push budget={budget} />)
				/>
			</RRNavigation.Navigation>
		</RRNavigation.NativeRouter>
};