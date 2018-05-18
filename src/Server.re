module InnerServer = BsSocket.Server.Make(TickTackCommon);

let startSocketIOServer = http => {
  let io = InnerServer.createWithHttp(http);
  print_endline("starting socket server");
  let initialState = ref(TickTackCommon.initState);


  InnerServer.onConnect(
  io,
  (socket) => {
    open InnerServer;
    print_endline("Got a connection!");
    let socket = Socket.join(socket, "someRoom", (e) => print_endline(e));
    let pipe = (typ, data) => {
      initialState := data;
      Js.log(data);
      Socket.broadcast(socket, typ, data);
    };
    Js.log(TickTackCommon.Data(initialState^));
    Socket.emit(socket, TickTackCommon.WelcomeMessage, Data(initialState^));

    /* Polymorphic pipe which actually knows about ExampleCommon.t from InnerServer */
    Socket.on(socket, TickTackCommon.Message, pipe(TickTackCommon.Message));
  }
);
}
