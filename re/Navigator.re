let c = ReasonReact.statelessComponent("Navigator");

let make = (~budget, ~updateBudget, _children) => {
	...c,
	render: _self => 
		<RRNavigation.NativeRouter>
			<RRNavigation.Navigation hideNavBar=true>
				<RRNavigation.Card
					exact=true
					path="/onboarding/accounts"
					render=(({ push }) => <OnboardingAccounts push budget={budget} updateBudget={updateBudget} />)
				/>
				<RRNavigation.Card
					exact=true
					path="/"
					render=(({ push }) => <Home push budget={budget} />)
				/>
			</RRNavigation.Navigation>
		</RRNavigation.NativeRouter>
};