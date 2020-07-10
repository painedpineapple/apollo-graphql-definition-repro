[@bs.val] external document: Js.t({..}) = "document";

let makeContainer = () => {
  let container = document##createElement("div");
  container##className #= "container";

  let () = document##body##appendChild(container);

  container;
};

/* Create an InMemoryCache */
let inMemoryCache = ApolloInMemoryCache.createInMemoryCache();

/* Create an HTTP Link */
let httpLink =
  ApolloLinks.createHttpLink(~uri="https://countries.trevorblades.com", ());

let client =
  ReasonApollo.createApolloClient(~link=httpLink, ~cache=inMemoryCache, ());

module App = {
  open ApolloHooks;

  module ContriesQuery = [%graphql
    {|
  query ContriesQuery {
    countries {
    code
    name
  }
  }
|}
  ];

  [@react.component]
  let make = () => {
    /* Both variant and records available */
    let (simple, _full) = useQuery(ContriesQuery.definition);

    <div>
      {switch (simple) {
       | Loading => <p> {React.string("Loading...")} </p>
       | Data(data) => <p> {React.string(data##currentUser##name)} </p>
       | NoData
       | Error(_) => <p> {React.string("Get off my lawn!")} </p>
       }}
    </div>;
  };
};

// All 4 examples.
ReactDOMRe.render(
  <ReasonApollo.Provider client>
    <ApolloHooks.Provider client> <App /> </ApolloHooks.Provider>
  </ReasonApollo.Provider>,
  makeContainer(),
);
