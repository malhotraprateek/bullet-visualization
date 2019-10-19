import * as THREE from "three/src/Three";
import { OrbitControls } from "three/examples/jsm/controls/OrbitControls";

interface LineInterface {
    from: [number, number, number];
    to: [number, number, number];
    srcColor: [number, number, number];
    destColor: [number, number, number];
}
interface SphereInterface {
    center: [number, number, number];
    radius: number;
    color: [number, number, number];
}
interface TriangleInterface {
    a: [number, number, number];
    b: [number, number, number];
    c: [number, number, number];
    color: [number, number, number];
}

interface PhysicsDataInterface {
    Lines: LineInterface[];
    Spheres: SphereInterface[];
    Triangles: TriangleInterface[];
    error: string;
}

let socket: WebSocket | null = null;

let waitTime: number = 0;
let polling = false;

function step() {
    if (socket) {
        socket.send("Ready");

        if (polling) {
            // Poll according to the poll rate
            setTimeout(() => {
                window.requestAnimationFrame(step);
            }, waitTime);
        } else {
            // Show default geometry
            createDefaultGeometry();
        }
    }
}

$(document).ready(function() {
    $("#startStopPolling").click(() => {
        if (!polling) {
            polling = true;
            connect();

            $("#startStopPolling").html("Stop Polling");
        } else {
            polling = false;
            $("#status")
                .css("color", "pink")
                .html("Terminating...");

            $("#startStopPolling").html("Start Polling");
    
            if (socket) {
                socket.close();
            }
    
            // Show default geometry
            createDefaultGeometry();
        }
    });

    $(function() {
        ($("#draggable") as any).draggable();
    });

    init();
    animate();
});

function connect() {
    socket = new WebSocket("ws://localhost:4321/physicsdata");

    if (socket) {
        socket.onopen = function() {
            $("#status")
                .css("color", "lime")
                .html("Polling...");
            window.requestAnimationFrame(step);
        };

        socket.onmessage = function(event: MessageEvent) {
            let data = JSON.parse(event.data) as PhysicsDataInterface;
            reCreateGeometry(data);
        };

        socket.onerror = function(ev: Event) {
            $("#status")
                .css("color", "red")
                .html("Error - Server socket closed!");
                
            // Show default geometry
            createDefaultGeometry();
        };

        socket.onclose = function(ev: Event) {
            $("#status")
                .css("color", "white")
                .html("Not Polling");

            // Show default geometry
            createDefaultGeometry();
        };
    }
}

$(function() {
    waitTime = 100;
    let minTime = 1;
    let maxTime = 1000;

    if (waitTime < minTime) {
        waitTime = minTime;
    } else if (waitTime > maxTime) {
        waitTime = maxTime;
    }

    let handle = $("#custom-handle");
    ($("#slider") as any).slider({
        value: waitTime,
        min: minTime,
        max: maxTime,
        create: function() {
            handle.text(($(this) as any).slider("value"));
        },
        slide: function(event: any, ui: any) {
            handle.text(ui.value);
            waitTime = ui.value;
        }
    });
});

let renderer: any = null;
let scene: any = null;
let camera: any = null;
let controls: any = null;

function init() {
    renderer = new THREE.WebGLRenderer();
    renderer.setSize(window.innerWidth, window.innerHeight);
    $("#renderer").append(renderer.domElement);

    camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 1, 500);
    camera.position.set(0, 0, 100);
    camera.lookAt(0, 0, 0);

    scene = new THREE.Scene();

    controls = new OrbitControls(camera, renderer.domElement);
    controls.minDistance = 1;
    controls.maxDistance = 5000;

    createDefaultGeometry();
}

function animate() {
    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}

function clearScene() {
    while (scene.children.length > 0) {
        scene.remove(scene.children[0]);
    }
}

function createDefaultGeometry() {
    clearScene();

    //create a blue LineBasicMaterial
    let material1 = new THREE.LineBasicMaterial({ color: 0x0000ff });
    let material2 = new THREE.LineBasicMaterial({ color: 0xff0000 });
    let material3 = new THREE.LineBasicMaterial({ color: 0x00ff00 });

    let geometry1 = new THREE.Geometry();
    geometry1.vertices.push(new THREE.Vector3(-10, 0, 0));
    geometry1.vertices.push(new THREE.Vector3(0, 10, 0));
    geometry1.vertices.push(new THREE.Vector3(10, 0, 0));
    geometry1.vertices.push(new THREE.Vector3(0, -10, 0));
    geometry1.vertices.push(new THREE.Vector3(-10, 0, 0));

    let lines1 = new THREE.Line(geometry1, material1);

    let geometry2 = new THREE.Geometry();
    geometry2.vertices.push(new THREE.Vector3(-1, 0, 0));
    geometry2.vertices.push(new THREE.Vector3(0, 1, 0));
    geometry2.vertices.push(new THREE.Vector3(1, 0, 0));
    geometry2.vertices.push(new THREE.Vector3(0, -1, 0));
    geometry2.vertices.push(new THREE.Vector3(-1, 0, 0));

    let lines2 = new THREE.Line(geometry2, material2);

    let geometry3 = new THREE.Geometry();
    geometry3.vertices.push(new THREE.Vector3(-5, 0, 0));
    geometry3.vertices.push(new THREE.Vector3(0, 5, 0));
    geometry3.vertices.push(new THREE.Vector3(5, 0, 0));
    geometry3.vertices.push(new THREE.Vector3(0, -5, 0));
    geometry3.vertices.push(new THREE.Vector3(-5, 0, 0));

    let lines3 = new THREE.Line(geometry3, material3);

    scene.add(lines1);
    scene.add(lines2);
    scene.add(lines3);
}

function reCreateGeometry(data: PhysicsDataInterface) {
    clearScene();

    for (let line of data.Lines) {
        //create a blue LineBasicMaterial
        let lineColor = new THREE.Color(line.srcColor[0], line.srcColor[1], line.srcColor[2]);
        let material = new THREE.LineBasicMaterial({ color: lineColor });

        let lineGeometry = new THREE.Geometry();
        lineGeometry.vertices.push(new THREE.Vector3(line.from[0], line.from[1], line.from[2]));
        lineGeometry.vertices.push(new THREE.Vector3(line.to[0], line.to[1], line.to[2]));

        let threeJsLine = new THREE.Line(lineGeometry, material);

        scene.add(threeJsLine);
    }
}
