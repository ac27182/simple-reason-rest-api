open Express;
open Json;

let app: Express.App.t = express();

let p: int = 8080;

type myJson = {
  name: string,
  age: int,
};

// very clunky....
// canonical way to create json using dicts
let makeJson = () => {
  let a0: myJson = {name: "alex", age: 22};
  let j = Js.Dict.empty();
  Js.Dict.set(j, "name", Js.Json.string(a0.name));
  Js.Dict.set(j, "age", Js.Json.number(a0.age |> float_of_int));
  Js.Json.object_(j);
};

// wat of encoding json using bs-json
let makeJson0 = () => {
  let a0: myJson = {name: "alex", age: 22};
  let obj =
    Json.Encode.(
      object_([("name", string(a0.name)), ("age", int(a0.age))])
    );
  obj;
};

let listener = (e: Js.null_undefined(Js.Exn.t)): unit => {
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log @@ "hello world"
  };
};

let f0 = Middleware.from((_, _) => Response.sendString("hello browser"));
let f1 = Middleware.from((_, _) => Response.sendJson(makeJson0()));

f0 |> App.get(app, ~path="/e0");
f1 |> App.get(app, ~path="/e1");

// no IO monad...
// here we have a side effect and we dont know it
let server = App.listen(app, ~port=p, ~onListen=listener, ());