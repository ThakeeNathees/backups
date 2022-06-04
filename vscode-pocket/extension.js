
const vscode = require("vscode");
const vslc = require("vscode-languageclient");

let client = undefined;

function activate(context) {
  console.log('pocketlang extension activated!');

  // Pocketlang language server script.
  let pkls = context.asAbsolutePath('language_server.pk');

  // FIXME: for now we're assuming that pocket binary exists in the path.
  let serverOptions = {
    command : "pocket",     // Pocket executable path (assuming it exists on path).
    args: ['"'+ pkls +'"'], // The Surrounding quote required if the path contain space.
    options : { shell : true } // We're using stdio ipc.
  };

  let clientOptions = {
		documentSelector: [
      { scheme: 'file', language: 'pocketlang' }
    ],
	};

  client = new vslc.LanguageClient(
    'PocketLS',
    'Pocketlang Language Server',
    serverOptions,
    clientOptions
  );

  // Start the client. This will also launch the server.
	client.start();
}

function deactivate() {
  if (!client) return undefined;
	return client.stop();
}

module.exports = {
	activate,
	deactivate
}
