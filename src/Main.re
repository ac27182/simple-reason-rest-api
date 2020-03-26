open Express;

let app: Express.App.t = express();

let p: int = 8080;

let listener = (e: Js.null_undefined(Js.Exn.t)): unit => {
  switch (e) {
  | exception (Js.Exn.Error(e)) =>
    Js.log(e);
    Node.Process.exit(1);
  | _ => Js.log @@ "hello world"
  };
};

let server = App.listen(app, ~port=p, ~onListen=listener, ());