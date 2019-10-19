# Bullet Visualization
A web based interface to visualize and debug rigid bodies created using Bullet SDK.

# Building
## Server
- Integrate the following dependencies into your own C++ code:
  - (nlohmann::json)[https://github.com/nlohmann/json]
  - (net11)[https://github.com/whizzter/net11]
  - (glm)[https://github.com/g-truc/glm]
  - And (Bullet)[https://github.com/bulletphysics/bullet3] (obviously).
- Clone the repository using the command `git clone https://github.com/malhotraprateek/bullet-visualization.git`.
- Navigate to bullet-server-code-reference folder.
- Integrate files present into your own C++ application. (See the todo comments for more info)
- Make a test run by entering `http://localhost:4321/physicsdata` into your browser to check if the physics server is up.

**NOTE**: It is not a requirement to use the server reference files or create the server using C++, provided you follow the steps mentioned in the todo comments and have a good working knowledge of websocket communications.

## Client
- Clone the repository using the command `git clone https://github.com/malhotraprateek/bullet-visualization.git`.
- Navigate to bullet-visualization folder.
- Run `npm install`.
- Run `npm run build`.
- Double click on index.html to open the visualizer in your default browser.
