#include "PixelForge/include/PixelForge/core/fb_context.hpp"
#include "PixelForge/include/PixelForge/primitives/canvas.hpp"
#include "iostream"

int main(){
    // Creating FrameBuffer context and canvas
    PixelForge::FBContext fb("/dev/fb0");
    if(!fb.init()) {
        std::cout << "Error Initializing FrameBuffer!\n";
        return 1;
    }
    PixelForge::Canvas canvas(fb);
    // Clearing screen
    canvas.clear(PixelForge::Color::Black.toARGB32());
    // Painting
    canvas.fillRect(100, 100, 200, 150, 0xFFFF0000);
    canvas.fillCircle(400, 300, 80, 0xFF00FF00);

    canvas.drawLine(50, 50, 750, 550, 0xFF0000FF);

    std::vector<std::pair<int, int>> triangle = {
        {600, 100},
        {700, 200},
        {500, 200}
    };
    canvas.drawPolygon(triangle, 0xFFFFFFFF);
    canvas.drawPolygon(triangle, 0xFFFFFFFF);
    
    return 0;

}
