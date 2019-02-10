type state = {
  email: string,
  password: string,
};
type action =
  | Login
  | UpdateEmailField(string)
  | UpdatePasswordField(string)
  | LoggedIn(state)
  | NotLoggedIn;
let str = ReasonReact.string;
let base_url = "http://localhost:8080/api/v1/";

let login = user => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "email", Js.Json.string(user.email));
  Js.Dict.set(payload, "password", Js.Json.string(user.password));
  Js.Promise.(
    Fetch.fetchWithInit(
      base_url ++ "users/login",
      Fetch.RequestInit.make(
        ~method_=Post,
        ~body=
          Fetch.BodyInit.make(Js.Json.stringify(Js.Json.object_(payload))),
        ~headers=Fetch.HeadersInit.make({"Content-Type": "application/json"}),
        (),
      ),
    )
    |> then_(Fetch.Response.json)
    |> then_(json =>
         Json.Decode.{email: field("email", string, json), password: ""}
         |> (users => Some(users) |> resolve)
       )
  );
};

let component = ReasonReact.reducerComponent("Login");

let make = _children => {
  ...component,
  initialState: () => {email: "", password: ""},
  reducer: (action, state) =>
    switch (action) {
    | Login =>
      ReasonReact.UpdateWithSideEffects(
        state,
        self =>
          Js.Promise.(
            login(state)
            |> then_(result =>
                 switch (result) {
                 | Some(user) => resolve(self.send(LoggedIn(user)))
                 | None => resolve(self.send(NotLoggedIn))
                 }
               )
            |> ignore
          ),
      )
    | UpdateEmailField(email) => ReasonReact.Update({...state, email})
    | UpdatePasswordField(password) =>
      ReasonReact.Update({...state, password})
    | LoggedIn(_) =>
      ReasonReact.SideEffects(_ => ReasonReact.Router.push("score"))
    | NotLoggedIn => ReasonReact.Update(state)
    },
  render: self =>
    <div className="card align-middle mx-auto w-50 p-3 text-center">
      <form>
        <div className="card-header"> {ReasonReact.string("Login")} </div>
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
              onClick={_ => self.send({Login})}>
              {ReasonReact.string("Connexion")}
            </button>
          </div>
        </div>
        <div className="
          card-footer text-muted">
          <label> {ReasonReact.string("Pas encore de compte ?")} </label>
          <a href="register"> {str("S'inscrire")} </a>
        </div>
      </form>
      <div> {ReasonReact.string(self.state.email)} </div>
      <div> {ReasonReact.string(self.state.password)} </div>
    </div>,
};