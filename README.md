# Bullet Visualization
A web based interface to visualize and debug rigid bodies created using Bullet SDK.

# Building
## Server
- Integrate the following dependencies into your own C++ code:
  - [nlohmann::json](https://github.com/nlohmann/json)
  - [net11](https://github.com/whizzter/net11)
  - [glm](https://github.com/g-truc/glm)
  - And [Bullet](https://github.com/bulletphysics/bullet3) (obviously).
- Clone the repository using the command `git clone https://github.com/malhotraprateek/bullet-visualization.git`.
- Navigate to **bullet-server-code-reference** folder.
- Integrate files present into your own C++ application. (See the todo comments for more info)
- Make a test run by entering `http://localhost:4321/physicsdata` into your browser to check if the physics server is up.

**NOTE**: It is not a requirement to use the server reference files or create the server using C++, provided you follow the steps mentioned in the todo comments and have a good working knowledge of websocket communications.

## Client
- Clone the repository using the command `git clone https://github.com/malhotraprateek/bullet-visualization.git`.
- Navigate to bullet-visualization folder.
- Run `npm install`.
- Run `npm run build`.
- Double click on **index.html** to open the visualizer in your default browser.
- Click on **Start Polling** button to start rendering debug data from Bullet. Ensure the server is already up and running.

# Web View

![Image](bullet-visualization/bullet-visualization.png)

# Internals

- Client-Server communication uses web sockets.
- The visualizer can only handle debug lines for now.
- [Three.js](https://github.com/mrdoob/three.js) is used to render to the browser (WebGL).
- Polling rate can be controlled by the slider provided. This functionality has been given to avoid framerate drops and stuttering on the server.

# Disclaimers
- This project is not built for use in production environments.
- The client and server has not been tested for running on separate machines. Even so, I do not think there should be anything inhibiting it.
