This is a work-in-progress proof-of-concept for how to structure the frontend apps in a way that will be easy to define and manage.

This prototype expects a localhost websocket server on port 9091 to provide a video stream and external event stream. This can be tested with [wsVideoStreamer](../../../wsVideoStreamer/).

## Structure
The design is as follows:

* main.js contains the core setup, event management, and websocket communication functionality
* index.html contains a small script element that defines a list of "pages" to be loaded
* for each "page" (with name <PageName>) defined in index.html, there is expected to be the following subdirectory structure:
    - <PageName>/main.js
    - <PageName>/style.css
    - <PageName>/template.hbr
    - <PageName>/data.json
