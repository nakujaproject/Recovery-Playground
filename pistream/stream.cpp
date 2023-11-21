#include <iostream>
#include <raspicam/raspicam_cv.h>
#include <libwebsockets.h>

using namespace std;

// WebSocket server information
const char* server_address = "ws://192.168.100.9:8080";

// Camera settings
const int width = 640;
const int height = 480;

int main() {
    // Initialize the camera
    raspicam::RaspiCam_Cv Camera;
    Camera.set( CV_CAP_PROP_FORMAT, CV_8UC1 );
    Camera.set( CV_CAP_PROP_FRAME_WIDTH, width );
    Camera.set( CV_CAP_PROP_FRAME_HEIGHT, height );

    // Connect to the WebSocket server
    struct lws_context_creation_info info;
    memset(&info, 0, sizeof info);
    info.port = CONTEXT_PORT_NO_LISTEN;
    info.protocols = lws_get_ietf_version_protocol(2);
    info.iface = NULL;

    struct lws_context *context = lws_create_context(&info);
    if (!context) {
        cerr << "WebSocket context creation failed" << endl;
        return -1;
    }

    // Camera capture loop
    if (!Camera.open()) {
        cerr << "Error opening the camera" << endl;
        return -1;
    }

    while (true) {
        // Capture an image
        cv::Mat image;
        Camera.grab();
        Camera.retrieve(image);

        // TODO: Convert 'image' to JPEG format (if needed)

        // TODO: Send the JPEG image to the WebSocket server

        // Simulate a delay (adjust as needed)
        usleep(100000);
    }

    // Clean up
    Camera.release();
    lws_context_destroy(context);

    return 0;
}
