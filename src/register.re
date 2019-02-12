open Decoder;

let url_dev: string = "http://localhost:8080/";
type state = {
  email: string,
  password: string,
  error: string,
};

type action =
  | UpdateEmailField(string)
  | UpdatePasswordField(string)
  | Register
  | Registered
  | RegisteredFailed(string);

let component = ReasonReact.reducerComponent("Register");

let register = state => {
  let payload = Js.Dict.empty();
  Js.Dict.set(payload, "email", Js.Json.string(state.email));
  Js.Dict.set(payload, "password", Js.Json.string(state.password));
  Js.Promise.(
    Fetch.fetchWithInit(
      url_dev ++ "api/v1/users/",
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
         json |> Decoder.decodeResponse |> (user => Some(user) |> resolve)
       )
  );
};

let make = _children => {
  ...component,
  initialState: () => {email: "", password: "", error: ""},
  reducer: (action, state) =>
    switch (action) {
    | UpdateEmailField(email) => ReasonReact.Update({...state, email})
    | UpdatePasswordField(password) =>
      ReasonReact.Update({...state, password})
    | Register =>
      ReasonReact.UpdateWithSideEffects(
        state,
        self =>
          Js.Promise.(
            register(state)
            |> then_(result =>
                 switch (result) {
                 | Some(user) => resolve(self.send(Registered))
                 }
               )
            |> catch(_err =>
                 Js.Promise.resolve(
                   self.send(RegisteredFailed("User not registered")),
                 )
               )
            |> ignore
          ),
      )
    | Registered =>
      ReasonReact.SideEffects(_ => ReasonReact.Router.push("score"))
    | RegisteredFailed(err) => ReasonReact.Update({...state, error: err})
    | _ => ReasonReact.NoUpdate
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
        </div>
      </form>
      <div className="justify-content-center">
        <button
          className="btn btn-outline-primary"
          onClick={_ => self.send({Register})}>
          {ReasonReact.string("S'enregistrer")}
        </button>
      </div>
      <div className="
          card-footer text-muted">
        <label> {ReasonReact.string("Deja un compte ?")} </label>
        <a href="login"> {ReasonReact.string("Se connecter")} </a>
      </div>
      <div> {ReasonReact.string(self.state.error)} </div>
    </div>,
};