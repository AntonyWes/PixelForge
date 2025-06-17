#include "../PixelForge/PixelForge/include/PixelForge/core/fb_context.hpp"
#include "../PixelForge/PixelForge/include/PixelForge/primitives/canvas.hpp"
#include "iostream"

int main(){
    // Creating FrameBuffer context and canvas
    PixelForge::FBContext fb("/dev/fb0");
    if(!fb.init()) {
        std::cout << "Error Initializing FrameBuffer!\n";
        return 1;
    }
    PixelForge::Canvas canvas(fb);
    PixelForge::Color backgroundColor = PixelForge::Color(242, 242, 247);
    PixelForge::Color primaryColor = PixelForge::Color(0, 122, 255);
    PixelForge::Color secondPrimaryColor = PixelForge::Color(142, 142, 147);
    PixelForge::Color splitColor = PixelForge::Color(60, 60, 67, 74);
    // Clearing screen
    canvas.clear(backgroundColor.toARGB32());
    // Painting
    canvas.fillGradientRect(0, 0, 480, 800, backgroundColor.toXRGB8888(), splitColor.toXRGB8888());

    canvas.fillRoundedRect(5, 730, 230, 65, 20, secondPrimaryColor.toARGB32());
    canvas.fillRoundedRect(240, 730, 235, 65, 20, secondPrimaryColor.toARGB32());

    return 0;

}
