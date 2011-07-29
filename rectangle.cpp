#include <gtkmm.h>
#include <iostream>
#include <algorithm>
using namespace std;

class GridDrawer {
    typedef Cairo::RefPtr<Cairo::Context> context_t;
    context_t &context;
    int height;
    int width;
public:
    GridDrawer(context_t &context, int height, int width) 
        : context(context), height(height), width(width) {
    }

    void draw_block(double x_from, double y_from, double x_to, double y_to) {
        context->save();
        context->rectangle(x_from, y_from, x_to, y_to);
        context->set_source_rgba(0, 0.8, 0, 1);
        context->fill_preserve();
        context->restore();
        context->save();
        context->set_line_width(0.01);
        context->stroke();
        context->restore();
    }

    void draw() {
        context->save();
        context->scale(width, width);
        draw_block(0, 0, 0.1, 0.1);
        draw_block(0.5, 0.5, 0.4, 0.4);
        context->restore();
    }
};


class Window : public Gtk::Window {
  private:
    Gtk::DrawingArea area;
    Gtk::Allocation allocation;
    typedef Cairo::RefPtr<Cairo::Context> context_t;
    context_t context;
    int width;
    int height;

    bool on_area_expose(GdkEventExpose* event) {
        allocation = area.get_allocation();
        context = area.get_window()->create_cairo_context();
        width = allocation.get_width();
        height = allocation.get_height();
        cout << "width: " << width << ", height: " << height << endl;
        width = std::min(width, height);
        cout << "size: " << width << endl;

        GridDrawer gd(context, width, width);
        gd.draw();
        return true;
    }
  public:
    Window() : Gtk::Window() {
        area.signal_expose_event().connect(
            sigc::mem_fun(*this, &Window::on_area_expose));
        add(area);
    }
};


int main(int argc, char* argv[]) {
    Gtk::Main app(argc, argv);

    Window window;
    window.show_all();

    Gtk::Main::run(window);

    std::string filename = "image.pdf";
    int width = 400;
    int height = 400;
    Cairo::RefPtr<Cairo::PdfSurface> surface = Cairo::PdfSurface::create(filename, width, height);

    typedef Cairo::RefPtr<Cairo::Context> context_t;
    context_t cr = Cairo::Context::create(surface);
    GridDrawer gd(cr, width, height);
    
    gd.draw();

    cr->show_page();

    return 0;
}
