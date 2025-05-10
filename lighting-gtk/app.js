imports.gi.versions.Gtk = "3.0";
const Gtk = imports.gi.Gtk;

let app = new Gtk.Application({ application_id: 'org.micaah42.QuickLighting' });

app.connect('activate', () => {
    let window = new Gtk.ApplicationWindow({
        title: "DeLighted",
        application: app,
        default_width: 720,
    });

    let layout = new Gtk.VBox({
        spacing: 16,
        margin: 16,
    })
    window.add(layout)

    let row = new Gtk.HBox()
    layout.add(row)

    let sw = new Gtk.Switch({ halign: Gtk.Align.START, valign: Gtk.Align.START });
    row.add(sw)

    let label = new Gtk.Image({ halign: Gtk.Align.END, icon_name: 'night-light-symbolic' })
    row.add(label)

    let scale = new Gtk.Scale({ draw_value: false })
    scale.set_range(0, 1)
    scale.set_value(0.5)
    layout.add(scale)



    window.show_all();
});

app.run([]);
