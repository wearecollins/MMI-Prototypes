A simple oF program to test input into the web frontends.

This app creates a Websocket Server on port 9091.
The app uses the websocket for two things:

* Streaming a video file as binary jpg frames
* Sending events as JSON {event:{name:"...", value:"..."}}
