let saveUser = value => Dom.Storage.(localStorage |> setItem("ConnectedUser", value));
let getUser = () => Dom.Storage.(localStorage |> getItem("ConnectedUser"));
