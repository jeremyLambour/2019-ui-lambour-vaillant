open Page;

type state = {route: page};
type action =
  | ChangePage(page);

let component = ReasonReact.reducerComponent("AppRouter");
let make = _children => {
  ...component,
  initialState: () => {route: Login},
  reducer: (action, _state) =>
    switch (action) {
    | ChangePage(route) => ReasonReact.Update({route: route})
    },
  render: self =>
    <div>
      {switch (self.state.route) {
       | Login => <Login />
       | Register => <Register />
       }}
      <button onClick={_ => self.send({ChangePage(Register)})}>
        {ReasonReact.string("Go to Register")}
      </button>
      <button onClick={_ => self.send({ChangePage(Login)})}>
        {ReasonReact.string("Go to Login")}
      </button>
    </div>,
};