This Node.js server posts videos and photos to a Page on Facebook.

## configuring

configuration settings go in config.json. 
A sample is in [config.json.sample]() with sensitive data removed.
In order to get the appropriate Access Tokens from Facebook, 
you need to do the following.

### setup [ngrok](https://ngrok.com/)

ngrok is used for running a localhost server 
and having it be accessible from the internet. An altertative to using ngrok
would be to configure the FB App's allowed domains to be a domain you own,
and host the server at that address.

* download & unzip [ngrok](https://ngrok.com/download)
* create an [ngrok](https://ngrok.com/) account
* following the instructions after you sign in, authenticate the ngrok app using your authtoken
    - eg. `~/Downloads/ngrok authtoken [YOUR AUTHTOKEN HERE]`

### startup the fetchToken server

* cd to [fetchToken]()
* `npm start`

### startup ngrok

* `~/Downloads/ngrok http -subdomain=momi-auth 8012`
    - you need to register for an account in order to use the `-subdomain` flag

### register app and select Page

* visit [http://momi-auth.ngrok.io/]()
