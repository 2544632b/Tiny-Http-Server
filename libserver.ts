// @ts-nocheck
// ExportModule
function createHttpServer(host: string, port: number, msg: string) {
        const server: socket = createSocket();
        let size = integer2String(stringSize(msg));
        setSocketMessage("HTTP/1.1 200" + crlf() + 
            "Server: libuv" + crlf() +
            "Content-Type: text/html" + crlf() +
            "Content-Length: " + size + crlf() + crlf() + msg);
        socketListen(server, host, port);
}
