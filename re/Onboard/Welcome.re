
open ReactNative;

let comp = ReasonReact.statelessComponent "Welcome";

let make _children => {
    ...comp,
    render: fun _self => {
        <View>
            <Button title="Get started">
                <Text value="Welcome!" />    
            </Button>
        </View>
    }
}