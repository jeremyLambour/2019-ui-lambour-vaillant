type action =
  | Register
  | UpdateEmailField(string)
  | UpdatePasswordField(string);

type state = {
  email: string,
  password: string,
};
let str = ReasonReact.string;
let component = ReasonReact.reducerComponent("Register");

let make = _children => {
  ...component,
  initialState: () => {email: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | Register =>
      ReasonReact.Update({email: state.email, password: state.password})
    | UpdateEmailField(email) => ReasonReact.Update({...state, email})
    | UpdatePasswordField(password) =>
      ReasonReact.Update({...state, password})
    },
  render: self =>
    <div className="align-middle mx-auto w-50 p-3 text-center">
      <form>
        <div className="card-header"> {ReasonReact.string("Register")} </div>
        <div className="card-body">
          <div className="input-group mb-3">
            <input
              className="form-control"
              type_="text"
              value={self.state.email}
              placeholder="Email"
              onChange={event =>
                self.send(
                  UpdateEmailField(ReactEvent.Form.target(event)##value),
                )
              }
            />
          </div>
          <div className="input-group mb-3">
            <input
              className="form-control"
              type_="password"
              value={self.state.password}
              onChange={event =>
                self.send(
                  UpdatePasswordField(ReactEvent.Form.target(event)##value),
                )
              }
              placeholder="password"
            />
          </div>
          <div className="justify-content-center">
            <button
              className="btn btn-outline-primary"
              onClick={_ => self.send({Register})}>
              {ReasonReact.string("S'enregistrer")}
            </button>
          </div>
        </div>
        <div className="
          card-footer text-muted">
          <label> {ReasonReact.string("Deja un compte ?")} </label>
          <a href="login"> {str("Se connecter")} </a>
        </div>
      </form>
      <div> {ReasonReact.string(self.state.email)} </div>
      <div> {ReasonReact.string(self.state.password)} </div>
    </div>,
};