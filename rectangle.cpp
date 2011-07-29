#include <gtkmm.h>

class Window : public Gtk::Window {
  private:
    Gtk::DrawingArea area;

    bool on_area_expose(GdkEventExpose* event) {
        Gtk::Allocation allocation = area.get_allocation();
        Cairo::RefPtr<Cairo::Context> context =
            area.get_window()->create_cairo_context();
        int width = allocation.get_width();
        int height = allocation.get_height();
        context->set_source_rgba(0, 0, 0, 1);
        context->rectangle(0, 0, double(width)/10, double(height)/10);
        context->fill();
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

  return 0;
}
